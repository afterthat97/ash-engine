# atview

An cross-platform 3D model viewer for learning purpose, based on OpenGL, Assimp, and FreeImage.

## Features

* Supports reading 40+ 3D file formats, including FBX, DXF, Collada, Obj, X, PLY, 3DS.
* Supports popular texture formats like PNG, BMP, JPEG, TIFF.
* Provides multiple viewing options. You can toggle lighting, texture, the polygon mode and so on.
* Uses "w,a,s,d" to move in the scene, just like playing CS.

## Screenshots

Rock, lighting on, texture on, fill mode.

![](https://img.masterliu.net/atview/rock.jpg)

Rock, lighting off, texture on, fill mode.

![](https://img.masterliu.net/atview/rock-nolight.jpg)

Rock, lighting on, texture off, fill mode.

![](https://img.masterliu.net/atview/rock-notexture.jpg)

Rock, lighting on, texture off, line mode.

![](https://img.masterliu.net/atview/rock-line.jpg)

## How to use

Only command-line interface is supported currently. Pass the path of model as arguments, for example:

```
atview rock/rock.obj
```

## Build

For macOS, type `make` to build this project.

For Windows, just ignore the makefile and use Visual Studio to build it. Remember to put all the `*.dll` in the same directory with the `.exe`.
