#define DONT_DEFINE_LOGGER
#include "with_memory_logger.h"

#include "sog/format.h"

WithMemoryLogger::WithMemoryLogger() {
	sog::init(&memory_logger);
}

sog::OwnedMessage WithMemoryLogger::take() {
	auto r = std::move(memory_logger.messages.front());
	memory_logger.messages.pop();
	return r;
}

Pairs WithMemoryLogger::take_pairs() {
	auto msg = take();
	auto &src = *msg.source;
	
	Pairs out;
	out.reserve(src.value_count + 4);
	
	if (!src.file.empty())
		out.emplace_back("FILE", std::string{src.file});
	if (src.line)
		out.emplace_back("LINE", src.line);
	if (!src.function.empty())
		out.emplace_back("FUNC", std::string{src.function});
	if (!src.msg_template.empty())
		out.emplace_back("MSG", std::string{src.msg_template});
	
	for (size_t i = 0; i < src.value_count; ++i)
		out.emplace_back(src.keys[i], std::move(msg.values[i]));
	
	return out;
}

void WithMemoryLogger::clear() {
	while (!memory_logger.messages.empty())
		memory_logger.messages.pop();
}

std::string WithMemoryLogger::take_formatted() {
	auto message = take();
	sog::Formatter formatter(message.source);
	return formatter.format(message.ref());
}

void Sog::SetUp() {
	logger.clear();
}
void Sog::TearDown() {
	EXPECT_NO_LOG() << "Log messages left over after test.";
	logger.clear();
}
