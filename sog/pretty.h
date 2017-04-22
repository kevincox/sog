#ifndef SOG_PRETTY_H
#define SOG_PRETTY_H

#include "sink.h"

#include <iostream>
#include <mutex>

namespace sog {
	struct PrettySink: public sog::Sink {
		std::mutex out_lock;
		std::ostream *out;
		
		PrettySink(std::ostream *out): out{out} {}
		std::unique_ptr<SinkData> prepare(const Source *source) override;
		void log(SinkData *sd, Message msg) override;
	};
}

#endif
