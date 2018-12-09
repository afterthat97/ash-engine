# masterEngine

A cross-platform 3D Engine for learning purpose, based on OpenGL, Bullet, Assimp, FreeImage, and AntTweakBar.

## 公告

由于当前版本在最初设计时就存在缺陷，导致代码臃肿，含义混乱。新版本（`v1.0.0`）开发进行中，所有类、UI 将重新设计，届时老版代码将不再维护。

最后补充一句，新版本的 README、Release 更新日志等将全部使用**中文**表达。原因有二：

1. 关心这个项目的大部分（全部）都是中国人
2. 用英文表达太累了。我写的累，你读也累。何必相互折磨？

### 版本迭代一览

* `v0.1.x`：传统 OpenGL（固定渲染管线）
* `v0.2.x`：在 `v0.1.x` 基础上，将渲染函数替换为现代 OpenGL（OpenGL 3.3）
* `v0.3.x`：在 `v0.2.x` 基础上，加入了鼠标拾取功能（基于 bullet 物理引擎），支持对模型平移、旋转、缩放。
* `v0.4.x`：在 `v0.3.x` 基础上，加入了实时阴影功能（基于深度贴图）。
* `v0.5.0`：在 `v0.4.x` 基础上，调整了部分 UI 以方便使用。

### 已知 BUG

1. 在 macOS Mojave (10.14) 上编译运行后黑屏，手动调整窗口大小后可以正常使用。
 * 影响范围：`v0.2.0` - `v0.5.0`
 * BUG 定位：`GLFW` 库

2. 在对模型进行缩放变换时，可能出现缩放前的模型和缩放后的模型同时存在的情况。（随机发生）
 * 影响范围：未知
 * BUG 定位：未知 

## Requirements

* macOS 10.12+ or Windows x86 / x64
* A graphics card which supports OpenGL 3.3+

> If you are using Windows, extra Visual C++ redistributable packages (VC2017 and VC2013) are required. You can download them from [this page](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).

## Features

* Supports reading 40+ 3D file formats, including FBX, DXF, Collada, Obj, X, PLY, 3DS.
* Supports diffuse map, specular map, normal map and parallax map, along with popular texture formats like PNG, BMP, JPEG, TGA.
* Supports up to 8 point lights with real-time shadow (depth map)
* Supports translation, rotation and scaling.
* Import your model easily: just drag and drop them onto the window.

## Screenshots

![](screenshots/screenshot0.jpg)

![](screenshots/screenshot1.jpg)

## Usage

### Import

How to import your models? Drag and Drop them onto the window!

### Move around

* Press `W` and `S` to move forward or backward
* Press `A` and `D` to move left ot right
* Press `E` and `Q` to move up or down
* Press `SHIFT` to shift moving speed (5X faster)
* Click and hold your left mouse button and drag to change viewport

### Transform

1. Select a mesh or light;
2. Use function keys to switch mode:
 * `<F1>` = translation (default)
 * `<F2>` = rotation
 * `<F3>` = scaling.
3. Click on the axis and drag your mouse to perform transformation.

### Add a light

**There is no light in the scene at the begining**, you need to add lights by the `Add new light` button on the left bar. You can also press `L` to add one quickly.

### Copy & Paste

1. Select a mesh or light;
2. Press `Ctrl + C` to copy the selected item;
3. Press `Ctrl + V` to paste it.

### Delete

1. Select a mesh or light;
2. Press `BACKSPACE` to delete the selected item.

### Screenshots

Press `<F10>` to take screenshots. (automatically saved as BMP on your desktop)

## Build

All the pre-compiled dependencies are placed in the `lib` directory.

For macOS, clone this repository and type `make` to build.

For Windows, just download the VS2017 project from the "release" page and build it. All the dynamic libraries and the static libraries are included.
