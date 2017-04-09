import qbs 1.0

Project {
	minimumQbsVersion: "1.6"
	
	StaticLibrary {
		name: "sog"
		files: "sog/*.cc"
		
		Depends { name: "cpp" }
		cpp.cxxLanguageVersion: "c++1z"
		
		Export {
			Depends { name: "cpp" }
			cpp.includePaths: product.sourceDirectory
			cpp.cxxLanguageVersion: "c++1z"
		}
	}
	
	StaticLibrary {
		name: "testlib"
		builtByDefault: false
		
		files: "test/*.cc";
		excludeFiles: "test/*_test.cc"
		
		Depends { name: "cpp" }
		Depends { name: "sog" }
		
		Export {
			Depends { name: "cpp" }
			Depends { name: "sog" }
			cpp.includePaths: product.sourceDirectory
		}
	}
	
	CppApplication {
		name: "test"
		type: ["application", "autotest"]
		files: "test/*_test.cc"
		excludeFiles: "test/macro_func_test.cc"
		cpp.defines: "__LINE__=1337"
		Group {
			files: "test/macro_func_test.cc"
			cpp.defines: []
		}
		Depends { name: "testlib" }
		cpp.dynamicLibraries: ["gtest", "gtest_main"]
	}
	
	AutotestRunner { }
}

