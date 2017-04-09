#ifndef SOG___FORMAT_H
#define SOG___FORMAT_H

#include <experimental/string_view>
#include <initializer_list>
#include <sstream>

#include "sink.h"

namespace sog {
	class Formatter {
		enum Type { STREAM, NONE };
		struct Chunk {
			std::experimental::string_view literal;
			Type type;
			size_t value;
			
			Chunk(std::experimental::string_view l, Type t, size_t v):
				literal{l}, type{t}, value{v} {}
		};
		std::vector<Chunk> chunks;
		
	public:
		Formatter(const Source *source);
		std::string format(const OwnedMessage &m);
	};
}

#endif
