#include "format.h"
#include <iostream>

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
		
		auto key = t.substr(i, e-i);
		size_t ki;
		for (ki = 0; ki < source->value_count; ki++)
			if (key == source->keys[ki])
				break;
		assert(ki < source->value_count);
		
		chunks.emplace_back(t.substr(0, i-1), STREAM, ki);
		i = e;
		t = t.substr(i);
	}
	chunks.emplace_back(t, NONE, 0);
}

void sog::Formatter::format(Message m, std::ostream *out) {
	for (size_t i = 0; i < chunks.size(); ++i) {
		auto &chunk = chunks[i];
		*out << chunk.literal;
		switch (chunk.type) {
		case NONE:
			break;
		default:
			*out << m.values[chunk.value];
			break;
		}
	}
}

std::string sog::Formatter::format(const OwnedMessage &m) {
	std::ostringstream r;
	Value values[m.values.size()];
	for (size_t i = 0; i < m.values.size(); ++i)
		values[i] = m.values[i];
	format(Message(m.source, values), &r);
	return r.str();
}

std::ostream &sog::operator<<(std::ostream& out, sog::Formatter formatter) {
	out << "Formatter(";
	for (auto &chunk: formatter.chunks) {
		out << "\n\t" << chunk.literal;
		switch (chunk.type) {
		case Formatter::NONE:
			break;
		default:
			out << "\n\tLit(" << chunk.value << ')';
			break;
		}
	}
	out << ")";
	return out;
}
