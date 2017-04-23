#include "memory_logger.h"

namespace sog {
	Logged MemoryLogger::log(SinkData *, Message msg) {
		messages.emplace(msg);
		return {};
	}
}
