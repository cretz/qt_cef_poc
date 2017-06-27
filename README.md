
## Building on Windows

### Prereqs

* Qt 5.8+
* [MSVC 2015 Build Tools](http://landinghub.visualstudio.com/visual-cpp-build-tools)
* Latest [Windows 64-bit standard dist of CEF](http://opensource.spotify.com/cefbuilds/index.html#windows64_builds)
* Latest cmake
* Python 2.x
* Go 1.8+

### Building CEF Wrapper

This builds the DLL C++ wrapper that is statically linked to the final app. TODO: automate this too

* Extract CEF standard dist, cd to dir
* Make sure 64-bit msvc tools are on path by running `vcvarsall.bat amd64` (e.g.
  `"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64`)
* cmake -G "Visual Studio 14 Win64" .
* `cd libcef_dll_wrapper`
* In libcef_dll_wrapper.vcxproj, under debug's `RuntimeLibrary`, change `MultiThreaded` to `MultiThreadedDebugDLL` and
  under release's `RuntimeLibrary`, change `MultiThreaded` to `MultiThreadedDLL`
* For the debug build, `msbuild libcef_dll_wrapper.vcxproj /p:Configuration=Debug`
* For the release build, `msbuild libcef_dll_wrapper.vcxproj /p:Configuration=Release`

### Building App

* Cd to cloned dir
* Make sure `qmake.exe` from Qt and `go` from Go are on the `PATH`
* Make sure 64-bit msvc tools are on path by running `vcvarsall.bat amd64` (e.g.
  `"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64`)
* Make sure env var `CEF_DIR` is set to the dir where CEF was extracted to
* run `go run build.go build release` (or `run release` to run the release, or `clean` to clean, or replace `release`
  with `debug` for debug version)
