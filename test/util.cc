#include "test/util.h"

WithMemoryLogger::WithMemoryLogger() {
	auto logger = std::make_unique<sog::MemoryLogger>();
	memory_logger = logger.get();
	sog::init("test", std::move(logger));
}

std::vector<sog::Message> WithMemoryLogger::take() {
	std::vector<sog::Message> r;
	std::swap(memory_logger->messages, r);
	return r;
}

WithMemoryLogger::Pairs WithMemoryLogger::take_pairs() {
	WithMemoryLogger::Pairs r;
	
	auto &messages = memory_logger->messages;
	printf("SIZE: %zu\n", messages.size());
	r.reserve(memory_logger->messages.size());
	for (auto i = messages.begin(); i != messages.end(); ++i) {
		auto &msg = *i;
		auto &src = *msg.source;
		printf("SIZE:   %zu\n", src.value_count);
		
		r.emplace_back();
		auto &out = r.back();
		out.reserve(src.value_count);
		
		for (size_t i = 0; i < src.value_count; ++i)
			out.emplace_back(src.keys[i], std::move(msg.values[i]));
	}
	
	messages.clear();
	
	return r;
}
