#ifndef SOG___DYNAMIC_H
#define SOG___DYNAMIC_H

#include "sog/sog.h"

namespace sog {
	struct DynamicLog {
		std::vector<const char*> keys;
		Source source;
		
		std::vector<Value> values;
		Message message;
		
		DynamicLog() {
			message.source = &source;
		}
		
		void add(const char *k, Value v) {
			keys.push_back(k);
			values.push_back(std::move(v));
		}
		
		void submit();
	};
}

#endif
