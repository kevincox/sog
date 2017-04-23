#include "dynamic.h"

void sog::DynamicLog::submit() {
	source.keys = keys.data();
	message.values = values.data();
	source.value_count = values.size();
	
	auto prepare = sog::_prepare(&source);
	if (prepare.log)
		sog::_submit(prepare.sink_data.get(), std::move(message));
}
