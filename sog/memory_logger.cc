#include "memory_logger.h"

namespace sog {
	void MemoryLogger::log(SinkData *, Message msg) {
		messages.emplace_back(msg);
	}
}
