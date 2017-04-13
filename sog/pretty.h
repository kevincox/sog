#ifndef SOG___PRETTY_H
#define SOG___PRETTY_H

#include "sink.h"

#include <iostream>

namespace sog {
	struct PrettySink: public sog::Sink {
		std::ostream *out;
		
		PrettySink(std::ostream *out): out{out} {}
		std::unique_ptr<SinkData> prepare(const Source *source) override;
		void log(SinkData *sd, Message msg) override;
	};
}

#endif
