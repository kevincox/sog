#ifndef SOG___MEMORY_LOGGER_H
#define SOG___MEMORY_LOGGER_H

#include "sink.h"

namespace sog {
	struct MemoryLogger: Sink {
		std::vector<OwnedMessage> messages;
		void log(Message msg) override;
	};
}

#endif
