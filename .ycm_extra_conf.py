import os.path

WORKSPACE = os.path.dirname(os.path.abspath(__file__))
DEFAULT_FLAGS = [
	"-U_FORTIFY_SOURCE",
	"-Wall",
	"--std=c++1z",
	"-iquote", WORKSPACE + "/bazel-out/..",
	"-iquote", WORKSPACE + "/bazel-out/../external/gtest",
	"-iquote", WORKSPACE + "/bazel-out/../external/bazel_tools",
	"-isystem", WORKSPACE + "/bazel-out/../external/bazel_tools/tools/cpp/gcc3",
	"-internal-isystem", "/usr/include/c++/6.3.1",
	"-internal-isystem", "/usr/include/c++/6.3.1/x86_64-pc-linux-gnu",
	"-internal-isystem", "/usr/include/c++/6.3.1/backward",
	"-internal-isystem", "/usr/local/include",
	"-internal-isystem", "/usr/lib/clang/3.9.1/include",
	"-internal-externc-isystem", "/include",
	"-internal-externc-isystem", "/usr/include",
]

def FlagsForFile(filename, **kwargs):
	return { "flags": DEFAULT_FLAGS, "do_cache": True }
