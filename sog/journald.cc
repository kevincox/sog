#include <boost/algorithm/string/case_conv.hpp>
#include <sys/uio.h>
#include <systemd/sd-journal.h>

#include "journald.h"
#include "format.h"

namespace {

struct ToStringVisitor {
	std::string operator()(const long &s) const { return std::to_string(s); }
	std::string operator()(const double &s) const { return std::to_string(s); }
	std::string operator()(const std::string &s) const { return std::move(s); }
	std::string operator()(const std::experimental::string_view &s) const { return std::string(s); }
};

struct ToIovecVisitor {
	iovec operator()(const long &s) const { assert(false); }
	iovec operator()(const double &s) const { assert(false); }
	iovec operator()(const std::string &s) const {
		iovec r;
		r.iov_base = (void*)s.data();
		r.iov_len = s.size();
		return r;
	}
	iovec operator()(const std::experimental::string_view &s) const {
		iovec r;
		r.iov_base = (void*)s.data();
		r.iov_len = s.size();
		return r;
	}
};

std::vector<std::string> make_chunks(const sog::Source *source) {
	std::vector<std::string> r;
	r.reserve(source->value_count);
	
	for (size_t i = 0; i < source->value_count; ++i) {
		r.emplace_back(boost::algorithm::to_upper_copy(std::string(source->keys[i])));
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

std::unique_ptr<sog::SinkData> sog::JournaldSink::prepare(const sog::Source *source) {
	return std::make_unique<Data>(source);
}

void sog::JournaldSink::log(sog::SinkData *sd, sog::Message msg) {
	Data *data = (Data*)sd;
	const size_t count = msg.source->value_count + 6;
	std::string bufs[count];
	size_t i = 0;
	bufs[i++] = "MESSAGE=" + data->formatter.format(msg);
	bufs[i++] = "SOG_TMPL=" + std::string{msg.source->msg_template};
	bufs[i++] = "PRIORITY=" + std::to_string(msg.source->level);
	bufs[i++] = "CODE_FILE=" + std::string{msg.source->file};
	bufs[i++] = "CODE_FUNC=" + std::string{msg.source->function};
	bufs[i++] = "CODE_LINE=" + std::to_string(msg.source->line);
	
	for (size_t v = 0; v < msg.source->value_count; ++v) {
		bufs[i++] =
			data->chunks[v] + '=' +
			boost::apply_visitor(ToStringVisitor(), msg.values[v].data);
	}
	assert(i == count);
	
	iovec vecs[count];
	while (i--) {
		vecs[i].iov_base = (void*)bufs[i].data();
		vecs[i].iov_len = bufs[i].size();
	}
	
	sd_journal_sendv(vecs, count);
}
