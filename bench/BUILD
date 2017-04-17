[cc_binary(
	name = src[:-len(".cc")] + "_" + name,
	srcs = [ src ],
	deps = [
		"//:sog",
	],
	defines = defines,
	linkopts = ["-lbenchmark"],
)
	for src in glob(["*.cc"])
	for name, defines in {
		"pretty_null": [ "SINK_TYPE=sog::PrettySink", "SINK_ARGS=&null" ],
		"pretty_ramfs": [ "SINK_TYPE=sog::PrettySink", "SINK_ARGS=&ramfile" ],
		"pretty_disk": [ "SINK_TYPE=sog::PrettySink", "SINK_ARGS=&diskfile" ],
		"journald": [ "SINK_TYPE=sog::JournaldSink", "SINK_ARGS=" ],
	}.items()]
