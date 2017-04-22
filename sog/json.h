#ifndef SOG_JSON_H
#define SOG_JSON_H

#include "sink.h"

#include <iostream>
#include <mutex>

namespace sog {
	struct JsonSink: public sog::Sink {
		std::mutex out_lock;
		std::ostream *out;
		
		JsonSink(std::ostream *out): out{out} {}
		std::unique_ptr<SinkData> prepare(const Source *source) override;
		void log(SinkData *sd, Message msg) override;
	};
}

#endif
