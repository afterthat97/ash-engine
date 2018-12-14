# masterEngine

masterEngine 是一个跨平台的 3D 引擎，基于 Qt、OpenGL 和 Assimp，仅用于学习用途。

## 截图

![](screenshots/screenshot0.jpg)

![](screenshots/screenshot1.jpg)

## 版本迭代一览

### `v0.x.x` 系列

* `v0.1.x`：GLUT + 传统 OpenGL（固定渲染管线）+ Assimp。
* `v0.2.x`：在 `v0.1.x` 基础上，将渲染函数替换为**现代 OpenGL**（OpenGL 3.3），并支持**法线贴图**和自定义**天空盒**。
* `v0.3.x`：在 `v0.2.x` 基础上，加入了**鼠标拾取**功能（基于 bullet 物理引擎），支持对模型**平移、旋转、缩放**，并支持最多 16 个光源。
* `v0.4.x`：在 `v0.3.x` 基础上，加入了**实时阴影**功能（基于深度贴图），并支持**视差贴图**（凹凸贴图）、**光强衰减**和**伽马矫正**。
* `v0.5.0`：在 `v0.4.x` 基础上，调整了部分 UI 以方便使用。

**注意**：`v0.x.x` 在最初设计时就存在缺陷，导致代码臃肿、含义混乱，因此不建议阅读源码。

### `v1.x.x` 系列

* `v1.0.0-rc1`：使用 **Qt** 重制了图形界面，内外架构全部重新设计。将模型的通用属性（顶点、法线等属性）、加载（Assimp）和渲染（OpenGL）分开处理。

## 先决条件

* 需要 macOS 10.12+ 或 Windows x64 操作系统。
* 需要一张支持 OpenGL 3.3 或以上版本的图形卡，并安装好最新的驱动程序。

## 特点

* 支持读取 40 多种格式的 3D 模型文件，包括 FBX, DXF, Collada, Obj, X, PLY, 3DS 等。
* 支持漫反射贴图、镜面反射贴图和法线贴图。
* 支持最多 8 个点光源，可对光源的颜色、位置、衰减进行调整。
* 友好、简洁、方便的用户界面。

## 用法

### 加载模型

在菜单中选择 `文件` --> `打开`，或者直接将模型文件拖放到本程序的窗口中。

### 添加光源

由于初始场景中没有光源，你加载的模型可能看起来一片漆黑。在菜单中选择 `创建` --> `光源` 来添加点光源。

### 移动

* 使用 `W` 和 `S` 键来前进和后退
* 使用 `A` 和 `D` 键来左移或右移
* 使用 `E` 和 `Q` 键来上升或下降
* 使用 `Shift` 键来加速移动（5 倍速度）
* 按下鼠标左键并移动鼠标来调整视角

### 编辑属性

在左侧的场景树状视图中选择某个对象，即可在右侧的属性栏中编辑其属性。目前支持对 `光源`，`网格`，`材质`，`贴图` 进行属性编辑。