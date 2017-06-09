#include <sys/time.h>

#include "json.h"
#include "format.h"

namespace {

constexpr char HEX_DIGITS[17] = "0123456789abcdef";

void escape(std::string *out, std::string_view s) {
	for (auto i = s.begin(); i != s.end(); ++i) {
		auto c = *i;
		if (c == '"')
			out->append("\\\"");
		else if (c == '\n')
			out->append("\\n");
		else if (c == '\\')
			out->append("\\\\");
		else if (c < 0x20) {
			out->push_back('\\');
			out->push_back(HEX_DIGITS[c >> 4]);
			out->push_back(HEX_DIGITS[c & 0x0F]);
		} else
			out->push_back(c);
	}
}

struct ToJsonVisitor {
	std::ostream *out;
	
	ToJsonVisitor(std::ostream *out): out{out} {}
	
	void operator()(const long &s) const { *out << s; }
	void operator()(const double &s) const { *out << s; }
	void operator()(const std::string &s) const {
		return this->operator()(std::string_view(s));
	}
	void operator()(const std::string_view &s) const {
		*out << '"';
		std::string buf;
		escape(&buf, s);
		*out << buf;
		*out << '"';
	}
};

std::vector<std::string> make_chunks(const sog::Source *source) {
	std::vector<std::string> r;
	r.reserve(source->value_count + 3);
	
	r.emplace_back("{\"_msg\":\"");
	
	r.emplace_back("\",\"_file\":\"");
	escape(&r.back(), source->file);
	r.back() += "\",\"_line\":";
	r.back() += std::to_string(source->line);
	r.back() += ",\"_func\":\"";
	escape(&r.back(), source->function);
	r.back() += "\",\"_level\":";
	r.back() += std::to_string(source->level);
	r.back() += ",\"_tmpl\":\"";
	escape(&r.back(), source->msg_template);
	r.back() += "\",\"_sec\":";
	r.emplace_back(",\"_usec\":");
	
	for (size_t i = 0; i < source->value_count; ++i) {
		r.emplace_back(",\"");
		escape(&r.back(), source->keys[i]);
		r.back() += "\":";
	}
	
	return r;
}

struct Data: public sog::SinkData {
	sog::Formatter formatter;
	const std::vector<std::string> chunks;
	
	Data(const sog::Source *source):
		formatter{source},
		chunks{make_chunks(source)} {}
};

}

sog::Prepared sog::JsonSink::prepare(const sog::Source *source) {
	return { std::make_unique<Data>(source) };
}

sog::Logged sog::JsonSink::log(sog::SinkData *sd, sog::Message msg) {
	Data *data = static_cast<Data*>(sd);
	
	timeval time;
	gettimeofday(&time, nullptr);
#ifdef SOG_FAKE_SEC
	time.tv_sec = SOG_FAKE_SEC;
	time.tv_usec = SOG_FAKE_USEC;
#endif
	
	auto human = data->formatter.format(msg);
	auto sec = std::to_string(time.tv_sec);
	auto usec = std::to_string(time.tv_usec);
	
	std::lock_guard<std::mutex> lock{out_lock};
	*out << data->chunks[0];
	*out << human;
	*out << data->chunks[1];
	*out << sec;
	*out << data->chunks[2];
	*out << usec;
	
	for (size_t i = 0; i < msg.source->value_count; ++i) {
		*out << data->chunks[i + 3];
		boost::apply_visitor(ToJsonVisitor(out), msg.values[i].data);
	}
	
	*out << "}\n";
	
	return {};
}
