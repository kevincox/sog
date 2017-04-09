new_local_repository(
	name = "system",
	path = "/",
	build_file_content = """
		package(default_visibility = [ "//visibility:public" ])
		
		cc_library(
			name = "boost_config",
			hdrs = glob([
				"usr/include/boost/config.hpp",
				"usr/include/boost/config/...",
			]),
		)
		
		cc_library(
			name = "boost_container",
			hdrs = glob(["usr/include/boost/container/..."]),
		)
		
		cc_library(
			name = "boost_preprocessor",
			hdrs = glob(["usr/include/boost/preprocessor/..."]),
		)
		
		cc_library(
			name = "gtest",
			srcs = [ "usr/lib/libgtest.so" ],
			hdrs = glob(["usr/include/gtest/..."]),
		)
		
		cc_library(
			name = "gtest_main",
			srcs = [ "usr/lib/libgtest_main.so" ],
			deps = [ "gtest" ]
		)
	""".replace("\n\t\t", "\n"),
)

bind(name="boost_config", actual="@system//:boost_config")
bind(name="boost_container", actual="@system//:boost_container")
bind(name="boost_preprocessor", actual="@system//:boost_preprocessor")
bind(name="gtest", actual="@system//:gtest")
bind(name="gtest_main", actual="@system//:gtest_main")
