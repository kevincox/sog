#include <vector>
#include "sog/memory_logger.h"

struct WithMemoryLogger {
	sog::MemoryLogger *memory_logger;
	WithMemoryLogger();
	
	std::vector<sog::Message> take();
		
	using Pairs = std::vector<std::vector<std::pair<std::string,sog::Value>>>;
	Pairs take_pairs();
};
