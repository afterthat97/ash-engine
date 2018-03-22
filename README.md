# masterEngine

A cross-platform 3D Engine for learning purpose, based on OpenGL, Bullet, Assimp, FreeImage, and AntTweakBar.

## Features

* Requires macOS 10.12+ or Windows x86 or x64.
* Supports reading 40+ 3D file formats, including FBX, DXF, Collada, Obj, X, PLY, 3DS.
* Supports diffuse map, specular map, normal map and parallax map, along with popular texture formats like PNG, BMP, JPEG, TGA.
* Supports up to 8 point lights with real-time shadow (depth map, 1024 * 1024 with PCF)
* Import your model easily: just drag and drop them onto the window.

## Screenshots

![](screenshots/screenshot0.png)

![](screenshots/screenshot1.png)

## Usage

### Import

How to import your models? Drag and Drop them onto the window!

### Move

* Press `w` and `s` to move forward or backward
* Press `a` and `d` to move left ot right
* Press `e` and `q` to move up or down
* Press `SHIFT` to shift moving speed (5X faster)
* Click and hold your left mouse button and drag to change viewport

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