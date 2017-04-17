#ifndef SOG___JOURNALD_H
#define SOG___JOURNALD_H

#include "sink.h"

#include <systemd/sd-journal.h>

namespace sog {
	struct JournaldSink: public sog::Sink {
		JournaldSink() {}
		std::unique_ptr<SinkData> prepare(const Source *source) override;
		void log(SinkData *sd, Message msg) override;
	};
}

#endif
