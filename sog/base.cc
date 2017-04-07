#include "init.h"

using string_view = std::experimental::string_view;

namespace {

struct State {
	string_view name;
	std::unique_ptr<sog::Sink> sink;
} state;

struct Stdout: public sog::Sink {
	void log(sog::SinkData *, sog::Message msg) override {
		printf("Log\n");
	};
};

}

std::unique_ptr<sog::Sink> sog::Stdout() { return std::make_unique<::Stdout>(); }

void sog::init(
	std::experimental::string_view name,
	std::unique_ptr<sog::Sink> sink)
{
	state.name = name;
	
	if (sink)
		state.sink = std::move(sink);
	else
		state.sink = sog::Stdout();
}

sog::SinkData *sog::_prepare(const Source *s) {
	return state.sink->prepare(s);
}

void sog::_submit(sog::SinkData *sd, Message m) {
	state.sink->log(sd, m);
}
