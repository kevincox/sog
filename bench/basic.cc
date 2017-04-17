#include <benchmark/benchmark.h>
#include <fstream>

#include "sog/init.h"
#include "sog/journald.h"
#include "sog/macro.h"
#include "sog/pretty.h"

namespace {

#define MSG "A message"

void NoData(benchmark::State& state) {
	while (state.KeepRunning())
		LOG(INFO, MSG);
}
BENCHMARK(NoData);

void StringOne(benchmark::State& state) {
	while (state.KeepRunning())
		LOG(INFO, MSG, str, "Value");
}
BENCHMARK(StringOne);

void StringTen(benchmark::State& state) {
	while (state.KeepRunning())
		LOG(INFO, MSG,
			str0, "Value0",
			str1, "Value1",
			str2, "Value2",
			str3, "Value3",
			str4, "Value4",
			str5, "Value5",
			str6, "Value6",
			str7, "Value7",
			str8, "Value8",
			str9, "Value9");
}
BENCHMARK(StringTen);

void StringTenLarge(benchmark::State& state) {
	while (state.KeepRunning())
		LOG(INFO, MSG,
			str0, "Value0Value0Value0Value0 Value0Value 0Value0Value0Value0",
			str1, "Value1Value1Value1Value1 Value1Value 1Value1Value1Value1",
			str2, "Value2Value2Value2Value2 Value2Value 2Value2Value2Value2",
			str3, "Value3Value3Value3Value3 Value3Value 3Value3Value3Value3",
			str4, "Value4Value4Value4Value4 Value4Value 4Value4Value4Value4",
			str5, "Value5Value5Value5Value5 Value5Value 5Value5Value5Value5",
			str6, "Value6Value6Value6Value6 Value6Value 6Value6Value6Value6",
			str7, "Value7Value7Value7Value7 Value7Value 7Value7Value7Value7",
			str8, "Value8Value8Value8Value8 Value8Value 8Value8Value8Value8",
			str9, "Value9Value9Value9Value9 Value9Value 9Value9Value9Value9");
}
BENCHMARK(StringTenLarge);

void IntOne(benchmark::State& state) {
	while (state.KeepRunning())
		LOG(INFO, MSG,
			int0, 0,
			int1, 1,
			int2, 2,
			int3, 3,
			int4, 4,
			int5, 5,
			int6, 6,
			int7, 7,
			int8, 8,
			int9, 9);
}
BENCHMARK(IntOne);

void IntTen(benchmark::State& state) {
	while (state.KeepRunning())
		LOG(INFO, MSG,
			int0, 0,
			int1, 1,
			int2, 2,
			int3, 3,
			int4, 4,
			int5, 5,
			int6, 6,
			int7, 7,
			int8, 8,
			int9, 9);
}
BENCHMARK(IntTen);

void IntTenLarge(benchmark::State& state) {
	while (state.KeepRunning())
		LOG(INFO, MSG,
			int0, 1234567890,
			int1, 1234567891,
			int2, 1234567892,
			int3, 1234567893,
			int4, 1234567894,
			int5, 1234567895,
			int6, 1234567896,
			int7, 1234567897,
			int8, 1234567898,
			int9, 1234567899);
}
BENCHMARK(IntTenLarge);

}

int main(int argc, char** argv) {
	std::ofstream null{"/dev/null"};
	std::ofstream ramfile{"/tmp/sog-bench-file"};
	std::remove("/tmp/sog-bench-file");
	
	// TODO: Find a better on-disk file.
	std::ofstream diskfile{"sog-bench-file"};
	std::remove("sog-bench-file");
	
	SINK_TYPE sink{SINK_ARGS};
	sog::init(&sink);
	
	benchmark::Initialize(&argc, argv);
	benchmark::RunSpecifiedBenchmarks();
}
