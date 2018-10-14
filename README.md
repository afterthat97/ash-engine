# atview

An cross-platform 3D model viewer for learning purpose, based on OpenGL, Assimp, FreeImage, and AntTweakBar.

## Features

* Supports reading 40+ 3D file formats, including FBX, DXF, Collada, Obj, X, PLY, 3DS.
* Supports popular texture formats like PNG, BMP, JPEG, TIFF.
* Supports diffuse map, specular map and normal map.
* Provides some viewing options. You can toggle lighting, texture, MSAA and so on.
* Both light color and light position can be adjusted on bars.
* Press `<F10>` to take screenshots. (BMP only)

## Screenshots

![](screenshots/screenshot1.jpg)

![](screenshots/screenshot2.jpg)

## How to use

Only command-line interface is supported currently. Pass the filepath of model as arguments, for example:

```
atview /path-to-models/rock.obj
```

## Build

You don't need to download any third-party libraries. I have put them together with the source code.

For macOS, clone this repository and type `make` to build.

For Windows, just download the VS2017 project from the "release" page and build it. All `*.dll` and `*.lib` are included.
