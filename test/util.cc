#include "util.h"

WithMemoryLogger::WithMemoryLogger() {
	auto logger = std::make_unique<sog::MemoryLogger>();
	memory_logger = logger.get();
	sog::init("test", std::move(logger));
}

std::vector<sog::OwnedMessage> WithMemoryLogger::take() {
	std::vector<sog::OwnedMessage> r;
	std::swap(memory_logger->messages, r);
	return r;
}

WithMemoryLogger::Pairs WithMemoryLogger::take_pairs() {
	WithMemoryLogger::Pairs r;
	
	auto &messages = memory_logger->messages;
	r.reserve(memory_logger->messages.size());
	for (auto i = messages.begin(); i != messages.end(); ++i) {
		auto &msg = *i;
		auto &src = *msg.source;
		
		r.emplace_back();
		auto &out = r.back();
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
	}
	
	messages.clear();
	
	return r;
}
