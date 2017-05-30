with import <nixpkgs> {};

stdenv.mkDerivation rec {
	name = "sog";
	meta = {
		description = "Structured logging library.";
		homepage = https://github.com/kevincox/sog;
	};
	
	outputs = [ "dev" "lib" "out" ];
	
	src = builtins.filterSource (name: type:
		(lib.hasPrefix (toString ./BUILD) name) ||
		(lib.hasPrefix (toString ./Makefile) name) ||
		(lib.hasPrefix (toString ./sog) name) ||
		(lib.hasPrefix (toString ./test) name) ||
		(lib.hasPrefix (toString ./tools) name) ||
		(lib.hasPrefix (toString ./WORKSPACE) name)) ./.;
	
	buildInputs = with pkgs; [ bazel ];
	
	CPLUS_INCLUDE_PATH = lib.concatStringsSep ":" [
		"${pkgs.boost.dev}/include/"
		"${pkgs.gtest}/include/"
	];
	
	doCheck = true;
	
	preConfigure = ''
		export HOME=$TMPDIR # Trick bazel to use a better cache.
		makeFlagsArray=(
			BAZEL_FLAGS="--output_base=$TMPDIR/bazel-out"
			BAZEL_BUILD_FLAGS="--copt=-Wno-error"
		)
	'';
}
