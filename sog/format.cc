#include "format.h"

sog::Formatter::Formatter(const Source *source) {
	auto t = source->msg_template;
	size_t i;
	while ((i = t.find('$')) != std::string::npos) {
		i++;
		assert(i < t.size());
		if (t[i] == '$') {
			chunks.emplace_back(t.substr(0, i), NONE, 0);
			t = t.substr(i+1);
			continue;
		}
		
		auto e = i;
		while (e < t.size() && (
			( 'a' <= t[e] && t[e] < 'z' ) ||
			( 'A' <= t[e] && t[e] < 'Z' ) ||
			( '0' <= t[e] && t[e] < '9' )))
			e++;
		
		auto key = t.substr(i, e-1);
		size_t ki;
		for (ki = 0; ki < source->value_count; ki++)
			if (key == source->keys[ki])
				break;
		assert(ki < source->value_count);
		
		chunks.emplace_back(t.substr(0, i-1), STREAM, ki);
		i = e;
		t = t.substr(i);
	}
}

std::string sog::Formatter::format(const OwnedMessage &m) {
	std::ostringstream r;
	
	for (size_t i = 0; i < chunks.size(); ++i) {
		auto &chunk = chunks[i];
		r << chunk.literal;
		switch (chunk.type) {
		case NONE:
			break;
		default:
			r << m.values[chunk.value];
			break;
		}
	}
	
	return r.str();
}
