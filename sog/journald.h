#ifndef SOG_JOURNALD_H
#define SOG_JOURNALD_H

#include "sink.h"

#include <boost/iostreams/device/file_descriptor.hpp>

namespace sog {
	struct JournaldSink: public sog::Sink {
		int socket;
		~JournaldSink();
		JournaldSink();
		
		std::unique_ptr<SinkData> prepare(const Source *source) override;
		Logged log(SinkData *sd, Message msg) override;
	};
}

#endif
