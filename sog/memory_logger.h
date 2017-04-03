#ifndef SOG___MEMORY_LOGGER_H
#define SOG___MEMORY_LOGGER_H

#include "sog.h"

namespace sog {
	struct MemoryLogger: Sink {
		std::vector<Message> messages;
		
		void log(Message msg) override;
	};
}

#endif
