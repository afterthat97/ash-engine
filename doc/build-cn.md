# 编译指导

## 在 Windows 上编译

在 Windows 上编译有两种选择，最简单的方法是直接在 Qt Creator 中打开编译。高级用户还可以选择使用 Visual Studio 编译，我更推荐这种方式，本项目超过 90% 的代码都是在 Visual Studio 2017 中编写的。

以下方法在 Windows 10 x64, Qt 5.9.7, Visual Studio 2017 环境下通过测试。

### 使用 Qt Creator

在 Qt Creator 中打开此项目，直接编译运行即可。

### 使用 Visual Studio

在 Visual Studio 中安装并配置插件 `Qt VS Tools`，具体配置方法可以参考另一个教程：[How to Setting Qt GUI using C++ with Visual studio 2017](https://medium.com/@paryleevatou/how-to-setting-qt-gui-using-c-with-visual-studio-2017-dbb50406f99)

配置完成后，在 Visual Studio 的菜单中选择 `Qt VS Tools` --> `Open Qt Project File (*.pro)`，在弹出的文件浏览器中定位到 `AshEngine.pro` 并打开，插件就会生成一个对应的 Visual Studio 项目并打开它。

在菜单中选择 `项目` -> `属性` -> `生成事件` -> `生成后事件`，在 `命令行` 一项中填入以下内容（作用是在编译完成后，将主程序依赖的动态库拷贝到统一路径下，否则运行时会报错 “找不到 xxx.dll”）：

```
xcopy /Y "$(ProjectDir)lib\win\assimp-vc140-mt.dll" "$(OutDir)"
```

## 在 macOS 上编译

在 macOS 上编译也有多种选择，最简单的方法还是直接在 Qt Creator 中打开编译。高级用户还可以在命令行中使用 `qmake` 生成 Makefile 或者 Xcode 项目，接着使用 GNU make 或者 Xcode 编译本项目。

以下方法在 macOS 10.14.3, Qt 5.9.7, Xcode 10.1 环境下通过测试。

### 使用 Qt Creator

在 Qt Creator 中打开此项目，直接编译运行即可。

### 使用 GNU make

确定 Qt 的安装位置，并将命令行工具加入 `PATH` 变量（错误的路径可能会导致下一步操作找不到 `qmake`）：

```
export PATH="/XXX/Qt5.9.7/5.9.7/clang_64/bin:$PATH"
```

运行 `qmake` 生成 Makefile：

```
qmake -spec macx-g++ AshEngine.pro
```

运行 `make` 执行编译：

```
make
```

编译完成后，可执行文件位于 `build/bin` 下：

```
open build/bin/*.app
```

### 使用 Xcode

确定 Qt 的安装位置，并将命令行工具加入 `PATH` 变量（错误的路径可能会导致下一步操作找不到 `qmake`）：

```
export PATH="/XXX/Qt5.9.7/5.9.7/clang_64/bin:$PATH"
```

运行 `qmake` 生成 Xcode 项目（`*.xcodeproj`）：

```
qmake -spec macx-xcode AshEngine.pro
```

使用 Xcode 打开项目：

```
open *.xcodeproj
```

接着，在菜单中选择 `File` --> `Project Settings`，将 `Build System` 选项设置为 `Legacy Build System`（这是因为 `qmake` 还不支持新版 Xcode 中的 `Build System`）。

在 Xcode 编译运行即可。

## 在 Linux 上编译

本项目没有提供在 Linux 平台上部署、打包好的程序，用户需要下载源码、自行编译。

在 Linux 上编译的方法和 macOS 类似，不过由于 Linux 没有 Xcode，因此只支持使用 Qt Creator 或 GNU make 编译项目。具体编译步骤和 macOS 相同，不再赘述。要注意的是，在执行前需要将动态链接库的路径加入 `LD_LIBRARY_PATH` 中：

```
export LD_LIBRARY_PATH=$PWD/lib/linux:$LD_LIBRARY_PATH
```

