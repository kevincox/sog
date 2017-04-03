#include "sog/dynamic.h"

void sog::DynamicLog::submit() {
	source.keys = keys.data();
	message.values = values.data();
	source.value_count = values.size();
	
	sog::_prepare(&source);
	sog::_submit(std::move(message));
}
