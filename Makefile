PREFIX := /usr/local/
PREFIX_LIB := $(PREFIX)/lib/
PREFIX_STATIC := $(PREFIX)/lib/
PREFIX_INCLUDE := $(PREFIX)/include/

OPT := opt

BAZEL := bazel
BAZEL_FLAGS :=
BAZEL_DEFAULT_FLAGS := \
	--bazelrc=tools/bazel.rc \
	--max_idle_secs=10

BAZEL_BUILD_FLAGS :=
BAZEL_BUILD_DEFAULT_FLAGS := -c $(OPT)

BAZEL_TEST_FLAGS := $(BAZEL_BUILD_FLAGS)
BAZEL_TEST_DEFAULT_FLAGS := $(BAZEL_BUILD_DEFAULT_FLAGS)

bazel := $(BAZEL) $(BAZEL_FLAGS) $(BAZEL_DEFAULT_FLAGS)
bazel-build := $(bazel) build $(BAZEL_BUILD_FLAGS) $(BAZEL_BUILD_DEFAULT_FLAGS)
bazel-test := $(bazel) build $(BAZEL_TEST_FLAGS) $(BAZEL_TEST_DEFAULT_FLAGS)

.DEFAULT: test
.PHONY: check test
check test:
	$(bazel-test) //...

.PHONY: all build
all build:
	$(bazel-build) //:sog

.PHONY: install
install:
	$(bazel-build) //:sog
	
	install -Dt $(PREFIX_LIB) bazel-bin/libsog.so
	install -Dt $(PREFIX_STATIC) bazel-bin/libsog.a
	install -Dt $(PREFIX_INCLUDE) sog/*.h
