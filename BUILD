cc_library(
	name = "sog",
	srcs = glob(["sog/*.cc"]),
	hdrs = glob(["sog/*.h"]),
	linkopts = ["-lboost_iostreams", "-lsystemd"],
	visibility = [ "//visibility:public" ],
)

cc_library(
	name = "sog_test",
	srcs = glob(["sog/*.cc"]),
	hdrs = glob(["sog/*.h"]),
	defines = ["SOG_FAKE_SEC=950455800", "SOG_FAKE_USEC=50000"],
	linkopts = ["-lboost_iostreams", "-lsystemd"],
)

cc_library(
	name = "testlib",
	srcs = glob(["test/*.cc"], exclude=glob(["test/*_test.cc"])),
	hdrs = glob(["test/*.h"]),
	deps = [
		":sog_test",
	],
	linkopts = ["-lgtest_main", "-lgtest"],
)

[cc_test(
	name = src[len("test/"):-len(".cc")],
	srcs = [ src ],
	deps = [
		":testlib",
	],
	defines = ["_sog_LINE=1337"] if src != "test/macro_func_test.cc" else [],
) for src in glob(["test/*_test.cc"])]

[cc_test(
	name = "example_" + src[len("example/"):-len(".cc")],
	srcs = [ src ],
	deps = [
		":sog",
	],
) for src in glob(["example/*.cc"])]
