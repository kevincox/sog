#ifndef SOG___MEMORY_LOGGER_H
#define SOG___MEMORY_LOGGER_H

#include "sink.h"

#include <queue>

namespace sog {
	struct MemoryLogger: Sink {
		std::queue<OwnedMessage> messages;
		void log(SinkData *, Message msg) override;
	};
}

#endif
