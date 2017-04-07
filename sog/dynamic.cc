#include "dynamic.h"

void sog::DynamicLog::submit() {
	source.keys = keys.data();
	message.values = values.data();
	source.value_count = values.size();
	
	std::unique_ptr<sog::SinkData> sd { sog::_prepare(&source) };
	sog::_submit(sd.get(), std::move(message));
}
