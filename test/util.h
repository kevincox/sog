#include <vector>

#include "sog/init.h"
#include "sog/memory_logger.h"

struct WithMemoryLogger {
	sog::MemoryLogger *memory_logger;
	WithMemoryLogger();
	
	std::vector<sog::OwnedMessage> take();
		
	using Pairs =
		std::vector<std::vector<std::pair<std::string,sog::OwnedValue>>>;
	Pairs take_pairs();
};
