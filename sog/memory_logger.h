#ifndef SOG_MEMORY_LOGGER_H
#define SOG_MEMORY_LOGGER_H

#include "sink.h"

#include <queue>

namespace sog {
	struct MemoryLogger: Sink {
		std::queue<OwnedMessage> messages;
		void log(SinkData *, Message msg) override;
	};
}

#endif
