package main

import (
	"fmt"
	"io"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
)

func main() {
	if err := start(); err != nil {
		fmt.Printf("Error: %v\n", err)
		os.Exit(2)
	}
}

var isWindows = runtime.GOOS == "windows"
var debugEnabled = false

func start() error {
	// Only supporting Windows 64 bit for now
	if !isWindows || runtime.GOARCH != "amd64" {
		return fmt.Errorf("Unsupported OS '%v' or arch '%v'", runtime.GOOS, runtime.GOARCH)
	}
	if len(os.Args) < 2 {
		return fmt.Errorf("No command provided. Use either 'clean', 'build', or 'run'")
	}

	switch os.Args[1] {
	case "rerun":
		err := clean()
		if err == nil {
			err = run()
		}
		return err
	case "run":
		return run()
	case "clean":
		return clean()
	case "rebuild":
		err := clean()
		if err == nil {
			err = build()
		}
		return err
	case "build":
		return build()
	default:
		return fmt.Errorf("Unrecognized command '%v'", os.Args[1])
	}
}

func run() error {
	if err := build(); err != nil {
		return err
	}
	target, err := target()
	if err != nil {
		return err
	}
	return execCmd(filepath.Join(target, "qt_cef_poc.exe"))
}

func clean() error {
	err := os.RemoveAll("debug")
	if err == nil {
		err = os.RemoveAll("release")
	}
	return err
}

func build() error {
	target, err := target()
	if err != nil {
		return err
	}
	// Get qmake path
	qmakeExeName := "qmake"
	if isWindows {
		qmakeExeName += ".exe"
	}
	qmakePath, err := exec.LookPath(qmakeExeName)
	if err != nil {
		return err
	}

	// Make the dir for the target
	if err := os.MkdirAll(target, os.ModeDir); err != nil {
		return err
	}

	// Run qmake TODO: put behind flag
	if err := execCmd(qmakePath, "qt_cef_poc.pro"); err != nil {
		return fmt.Errorf("QMake failed: %v", err)
	}

	// Run nmake
	nmakePath, err := exec.LookPath("nmake.exe")
	if err != nil {
		return err
	}
	if err := execCmd(nmakePath, target); err != nil {
		return fmt.Errorf("NMake failed: %v", err)
	}

	// Copy over resources
	if err := copyResources(qmakePath, target); err != nil {
		return err
	}

	return nil
}

func target() (string, error) {
	target := "debug"
	if len(os.Args) >= 3 {
		if os.Args[2] != "release" && os.Args[2] != "debug" {
			return "", fmt.Errorf("Unknown target '%v'", os.Args[2])
		}
		target = os.Args[2]
	}
	return target, nil
}

func execCmd(name string, args ...string) error {
	cmd := exec.Command(name, args...)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	return cmd.Run()
}

func copyResources(qmakePath string, target string) error {
	cefDir := os.Getenv("CEF_DIR")
	if cefDir == "" {
		return fmt.Errorf("Unable to find CEF_DIR env var")
	}
	// Copy over some Qt DLLs
	qtDlls := []string {
		"Qt5Core.dll",
		"Qt5Gui.dll",
		"Qt5Widgets.dll",
	};
	// Debug libs are d.dll
	if target == "debug" {
		for i := range qtDlls {
			qtDlls[i] = strings.Replace(qtDlls[i], ".dll", "d.dll", -1)
		}
	}
	err := copyEachToDirIfNotPresent(filepath.Dir(qmakePath), target, qtDlls...)
	if err != nil {
		return err
	}

	// Need special ucrtbased.dll for debug builds
	if target == "debug" {
		err = copyEachToDirIfNotPresent("C:\\Program Files (x86)\\Windows Kits\\10\\bin\\x64\\ucrt",
			target, "ucrtbased.dll");
		if err != nil {
			return err
		}
	}

	// Copy over CEF libs
	err = copyEachToDirIfNotPresent(filepath.Join(cefDir, strings.Title(target)), target,
		"libcef.dll",
		"chrome_elf.dll",
		"natives_blob.bin",
		"snapshot_blob.bin",
		"d3dcompiler_43.dll",
		"d3dcompiler_47.dll",
		"libEGL.dll",
		"libGLESv2.dll",
	)
	if err != nil {
		return err
	}

	// Copy over CEF resources
	cefResDir := filepath.Join(cefDir, "Resources")
	err = copyEachToDirIfNotPresent(cefResDir, target,
		"icudtl.dat",
		"cef.pak",
		"cef_100_percent.pak",
		"cef_200_percent.pak",
		"cef_extensions.pak",
		"devtools_resources.pak",
	)
	if err != nil {
		return err
	}

	// And CEF locales
	targetLocaleDir := filepath.Join(target, "locales")
	if err = os.MkdirAll(targetLocaleDir, os.ModeDir); err != nil {
		return err
	}
	err = copyEachToDirIfNotPresent(filepath.Join(cefResDir, "locales"), targetLocaleDir, "en-US.pak")
	return err
}

func copyEachToDirIfNotPresent(srcDir string, destDir string, srcFilenames ...string) error {
	for _, srcFilename := range srcFilenames {
		if err := copyToDirIfNotPresent(filepath.Join(srcDir, srcFilename), destDir); err != nil {
			return err
		}
	}
	return nil
}

func copyToDirIfNotPresent(src string, destDir string) error {
	return copyIfNotPresent(src, filepath.Join(destDir, filepath.Base(src)))
}

func copyIfNotPresent(src string, dest string) error {
	if _, err := os.Stat(dest); os.IsExist(err) {
		debugLogf("Skipping copying '%v' to '%v' because it already exists")
		return nil
	}
	debugLogf("Copying %v to %v\n", src, dest)
	in, err := os.Open(src)
	if err != nil {
		return err
	}
	defer in.Close()
	out, err := os.Create(dest)
	if err != nil {
		return err
	}
	defer out.Close()
	_, err = io.Copy(out, in)
	cerr := out.Close()
	if err != nil {
		return err
	}
	return cerr
}

func debugLogf(format string, v ...interface{}) {
	if debugEnabled {
		log.Printf(format, v...)
	}
}
