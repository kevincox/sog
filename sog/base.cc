#include "init.h"
#include "pretty.h"

using string_view = std::experimental::string_view;

namespace {

sog::Sink *sink = nullptr;

}

void sog::init(sog::Sink *newsink) {
	assert(sink == nullptr);
	assert(newsink != nullptr);
	
	if (newsink)
		sink = newsink;
	else
		sink = new sog::PrettySink(&std::cerr);
}

std::unique_ptr<sog::SinkData> sog::_prepare(const Source *s) {
	return sink->prepare(s);
}

void sog::_submit(sog::SinkData *sd, Message m) {
	sink->log(sd, m);
}
