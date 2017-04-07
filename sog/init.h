#ifndef SOG___INIT_H
#define SOG___INIT_H

#include "sink.h"

namespace sog {
	std::unique_ptr<Sink> Stdout();
	
	void init(std::experimental::string_view name, std::unique_ptr<Sink> sink={});
}

#endif
