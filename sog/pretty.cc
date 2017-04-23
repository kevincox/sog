#include <sys/time.h>

#include "pretty.h"
#include "format.h"

namespace {

std::string make_chunk(const sog::Source *source) {
	auto level = sog::level_char(source->level);
	auto line = std::to_string(source->line);
	
	// TODO(windows)
	auto last_slash = source->file.find_last_of('/');
	if (last_slash == std::string::npos)
		last_slash = -1;
	auto basename = source->file.substr(last_slash + 1);
	
	std::string r;
	r.reserve(
		1 + 1 +
		basename.size() + 1 +
		line.size() - last_slash + 1);
	
	r += level;
	r += ' ';
	r.append(basename.data(), basename.size());
	r += ':';
	r += line;
	r += ' ';
	
	return r;
}

struct Data: public sog::SinkData {
	sog::Formatter formatter;
	const std::string chunk;
	
	Data(const sog::Source *source)
		: formatter{source}
		, chunk{make_chunk(source)}
	{
	}
};

}

std::unique_ptr<sog::SinkData> sog::PrettySink::prepare(const sog::Source *source) {
	if (source->msg_template.empty())
		return nullptr;
	
	return std::make_unique<Data>(source);
}

sog::Logged sog::PrettySink::log(sog::SinkData *sd, sog::Message msg) {
	if (!sd)
		return {};
	
	Data *data = dynamic_cast<Data*>(sd);
	
	timeval time;
	gettimeofday(&time, nullptr);
#ifdef SOG_FAKE_SEC
	time.tv_sec = SOG_FAKE_SEC;
	time.tv_usec = SOG_FAKE_USEC;
#endif
	tm tm;
	localtime_r(&time.tv_sec, &tm);
	char buf[20];
	size_t len = strftime(buf, 80, "%FT%T ", &tm);
	
	std::lock_guard<std::mutex> lock{out_lock};
	*out << std::experimental::string_view(buf, len);
	*out << data->chunk;
	data->formatter.format(msg, out);
	*out << std::endl;
	
	return {};
}
