# Ash Engine

Ash Engine is a cross-platform 3D engine (only for learning purpose) based on Qt, OpenGL and Assimp.

## Screenshot

![](screenshots/screenshot0.jpg)

## Requirements

* Windows 7 x64 or newer, macOS 10.12 or newer, or Ubuntu 16.04
* A graphics card that supports OpenGL 3.3+

## Features

* Friendly, simple and convenient GUI.
* Readable, clear code structure which is easy to expand.
* Supports reading and saving 3D model files in 40+ formats, including FBX, DXF, Collada, Obj, X, PLY, 3DS, etc.
* Supports reading and saving the entire project (using the file type `*.aeproj` defined by this engine)
* Uses tree structure to describe the scene, supports basic transformation (translation, rotation, scaling) on model and mesh.
* Supports diffuse maps, specular maps, and normal maps.
* Supports ambient light, directional light, point light, and spotlight. You can create not more than 8 lights of each type. For each light, you can adjust its color, position, and many other properties.

## User Manual

[Basic User Manual (English)](doc/basic-manual-en.md)
[Basic User Manual (Chinese)](doc/basic-manual-cn.md)

## Build

[Build Instructions (English)](doc/build-en.md)
[Build Instructions (Chinese)](doc/build-cn.md)

## Future Work

### Rendering

* DirectX 12
* Real-time shadow based on depth map
* Real-time ray tracing using DirectX 12 DXR
* Deferred rendering
* SSAO
* Displacement mapping
* PBR

### Others

* More detailed documents for both English and Chinese
* Implement unit tests using Google Test

Welcome to contact or join me!

Email: afterthat97@gmail.com
