#include "init.h"
#include "pretty.h"

using string_view = std::experimental::string_view;

namespace {

sog::Sink *sink = nullptr;

sog::Value::Data data_ref(const sog::Value &v) {
	auto *str = boost::get<std::string>(&v.data);
	if (str)
		return sog::Value::Data(std::experimental::string_view{*str});
	return v.data;
}

}

bool sog::Value::operator==(const Value &that) const {
	auto l = data_ref(*this);
	auto r = data_ref(that);
	// return data_ref(*this) == data_ref(that);
	return l == r;
}

void sog::init(sog::Sink *newsink) {
	assert(sink == nullptr);
	assert(newsink != nullptr);
	
	if (newsink)
		sink = newsink;
	else
		sink = new sog::PrettySink(&std::clog);
}

std::unique_ptr<sog::SinkData> sog::_prepare(const Source *s) {
	return sink->prepare(s);
}

void sog::_submit(sog::SinkData *sd, Message m) {
	sink->log(sd, m);
}
