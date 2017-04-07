#include <vector>

#include "sog/init.h"
#include "sog/memory_logger.h"

#define MSG(msg, ...) { \
	{"FILE", __FILE__}, \
	{"LINE", std::to_string(__LINE__)}, \
	{"FUNC", __PRETTY_FUNCTION__}, \
	{"MSG", msg}, \
	__VA_ARGS__ }

struct WithMemoryLogger {
	sog::MemoryLogger *memory_logger;
	WithMemoryLogger();
	
	std::vector<sog::OwnedMessage> take();
	
	using Pairs = std::vector<std::vector<std::pair<std::string,sog::OwnedValue>>>;
	Pairs take_pairs();
};
