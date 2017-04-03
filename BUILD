cc_library(
	name = "sog",
	srcs = glob(["sog/*.cc"]),
	hdrs = glob(["sog/*.h"]),
	deps = [
		"//external:boost",
	],
)

cc_library(
	name = "testlib",
	srcs = glob(["test/*.cc"], exclude=glob(["test/*_test.cc"])),
	hdrs = glob(["test/*.h"]),
	deps = [
		":sog",
	],
)

[cc_test(
	name = src[len("test/"):-len(".cc")],
	srcs = [ src ],
	deps = [
		"//external:gtest",
		"//external:gtest_main",
		":sog",
		":testlib",
	],
) for src in glob(["test/*_test.cc"])]
