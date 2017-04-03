#include "memory_logger.h"

namespace sog {
	void MemoryLogger::log(Message msg) {
		messages.emplace_back(msg);
	}
}
