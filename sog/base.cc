#include "sog.h"

using string_view = std::experimental::string_view;

namespace {

struct State {
	string_view name;
	std::unique_ptr<sog::Sink> sink;
} state;

struct StdoutWatch: public sog::Watch {
	void watch(const sog::Message &msg) override {
		printf("Log\n");
	};
};

}

std::unique_ptr<sog::Watch> sog::StdoutWatch() { return std::make_unique<::StdoutWatch>(); }

void sog::init(
	std::experimental::string_view name,
	std::unique_ptr<sog::Sink> sink)
{
	state.name = name;
	
	if (sink)
		state.sink = std::move(sink);
	else
		state.sink = sog::StdoutWatch();
}

void sog::_prepare(Source *s) {
	state.sink->prepare(s);
}

void sog::_submit(Message m) {
	state.sink->log(m);
}
