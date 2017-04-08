#include "util.h"

WithMemoryLogger logger;

WithMemoryLogger::WithMemoryLogger() {
	auto logger = std::make_unique<sog::MemoryLogger>();
	memory_logger = logger.get();
	sog::init("test", std::move(logger));
}

Pairs WithMemoryLogger::take_pairs() {
	auto &msg = memory_logger->messages.front();
	auto &src = *msg.source;
	
	Pairs out;
	out.reserve(src.value_count + 4);
	
	if (!src.file.empty())
		out.emplace_back("FILE", src.file);
	if (src.line)
		out.emplace_back("LINE", std::to_string(src.line));
	if (!src.function.empty())
		out.emplace_back("FUNC", src.function);
	if (!src.msg_template.empty())
		out.emplace_back("MSG", src.msg_template);
	
	for (size_t i = 0; i < src.value_count; ++i)
		out.emplace_back(src.keys[i], std::move(msg.values[i]));
	
	memory_logger->messages.pop();
	
	return out;
}

void WithMemoryLogger::clear() {
	while (!memory_logger->messages.empty())
		memory_logger->messages.pop();
}

void Sog::SetUp() {
	logger.clear();
}
void Sog::TearDown() {
	EXPECT_NO_LOG() << "Log messages left over after test.";
	logger.clear();
}
