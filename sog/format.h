#ifndef SOG_FORMAT_H
#define SOG_FORMAT_H

#include <experimental/string_view>
#include <initializer_list>
#include <sstream>

#include "sink.h"

namespace sog {
	class Formatter {
		enum Type { STREAM, NONE };
		struct Chunk {
			std::string_view literal;
			Type type;
			size_t value;
			
			Chunk(std::string_view l, Type t, size_t v):
				literal{l}, type{t}, value{v} {}
		};
		std::vector<Chunk> chunks;
		
	public:
		Formatter(const Source *source);
		void format(Message m, std::ostream *out);
		std::string format(const Message &m);
		
		friend std::ostream &operator<<(std::ostream&, sog::Formatter);
	};
	
	std::ostream &operator<<(std::ostream &out, sog::Formatter formatter);
}

#endif
