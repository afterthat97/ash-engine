# Building Instructions

## Build on Windows

There are two options for building on Windows. The easiest way is to open the project directly in Qt Creator. Advanced users can also choose to compile with Visual Studio, which I recommend. More than 90% of the code in this project is written in Visual Studio 2017.

The following methods were tested in Windows 10 x64, Qt 5.9.7, Visual Studio 2017.

### Using Qt Creator

Open this project in Qt Creator, build and run it directly.

### Using Visual Studio

Install and configure the plugin `Qt VS Tools` in Visual Studio. You can refer to another tutorial: [How to Setting Qt GUI using C++ with Visual studio 2017](https://medium.com/@paryleevatou/how-to-setting-qt-gui-using-c-with-visual-studio-2017-dbb50406f99)

After the configuration is done, select `Qt VS Tools` --> `Open Qt Project File (*.pro)` in the Visual Studio menu, navigate to `AshEngine.pro` in the pop-up file browser and open it. A corresponding Visual Studio project will be generated and opened.

On the `Project` menu, click `Properties` and select the `Build Events` tab. In the **Post-build event command line** box, specify the syntax of the build event (otherwise you may encounter "xxx.dll not found"):

```
xcopy /Y "$(ProjectDir)lib\win\assimp-vc140-mt.dll" "$(OutDir)"
```

## Build on macOS

There are also many options for building on macOS. The easiest way is to open the project directly in Qt Creator. Advanced users can also use `qmake` to generate a Makefile or Xcode project, and then use GNU `make` or Xcode to build the project.

The following methods were tested in macOS 10.14.3, Qt 5.9.7, and Xcode 10.1.

### Using Qt Creator

Open this project in Qt Creator, build and run it directly.

### Using GNU make

Find where Qt is installed and add the command line tool to `PATH` (otherwise you may not be able to find `qmake` in the next step):

```
export PATH="/XXX/Qt5.9.7/5.9.7/clang_64/bin:$PATH"
```

Run `qmake` to generate Makefile:

```
qmake -spec macx-g++ AshEngine.pro
```

Run `make` to build:

```
make
```

After the compilation is done, the executable is located under `build/bin`:

```
open build/bin/*.app
```

### Using Xcode

Find where Qt is installed and add the command line tool to `PATH` (otherwise you may not be able to find `qmake` in the next step):

```
export PATH="/XXX/Qt5.9.7/5.9.7/clang_64/bin:$PATH"
```

Run `qmake` to generate the Xcode project (`*.xcodeproj`):

```
qmake -spec macx-xcode AshEngine.pro
```

Open it with Xcode:

```
open *.xcodeproj
```

Next, select `File` --> `Project Settings` from the menu and set the `Build System` option to `Legacy Build System` (this is because `qmake` does not yet support the new `Build System` in Xcode 10+).

Build and run it in Xcode.

## Build on Linux

This project does not provide a release that is deployed on Linux. You need to build from source.

The steps of building on Linux is similar to that on macOS, but since Linux does not have Xcode, only building with Qt Creator or GNU `make` is supported. Building steps are the same as those of macOS and will not be described again. It should be noted that the path of dynamic libraries needs to be added to `LD_LIBRARY_PATH` before execution:

```
Export LD_LIBRARY_PATH=$PWD/lib/linux:$LD_LIBRARY_PATH
```

