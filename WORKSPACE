new_local_repository(
	name = "boost_headers",
	path = "/usr/include/boost",
	build_file_content = """
		package(default_visibility = [ "//visibility:public" ])
		
		cc_library(
			name = "boost_headers",
			hdrs = glob([".../*.h"]),
		)
	""".replace("\n\t\t", "\n"),
)

new_local_repository(
	name = "gtest_headers",
	path = "/usr/include/gtest",
	build_file_content = """
		package(default_visibility = [ "//visibility:public" ])
		
		filegroup(
			name = "headers",
			srcs = glob(["..."]),
		)
	""".replace("\n\t\t", "\n"),
)

new_local_repository(
	name = "system",
	path = "/usr/lib/",
	build_file_content = """
		package(default_visibility = [ "//visibility:public" ])
		
		cc_library(
			name = "gtest",
			srcs = [ "libgtest.so" ],
			hdrs = [ "@gtest_headers//:headers" ],
		)
		
		cc_library(
			name = "gtest_main",
			srcs = [ "libgtest_main.so" ],
			hdrs = [ "@gtest_headers//:headers" ]
		)
	""".replace("\n\t\t", "\n"),
)

bind(name="boost", actual="@boost_headers")
bind(name="gtest", actual="@system//:gtest")
bind(name="gtest_main", actual="@system//:gtest_main")
