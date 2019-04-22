# Basic User Manual

## Open and save a scene project

Select `File` --> `Open Scene` from the menu to open a project (`*.aeproj`).

Select `File` --> `Save Scene` or `Save Scene As` from the menu to save the current project.

## Objects in the scene

### Types

In Ash Engine, a scene may include the following objects:

* Camera
* Light
* Gridline
* Model
* Mesh
* Material
* Texture

### Relationship

In Ash Engine, the objects in the scene and their relationships are represented by a **tree structure**:

1. `Scene` is the root node, and `Camera`, `Light`, `Gridline`, and `Model` can be its children.
2. The child of `Model` may be `Mesh` or other `Model`.
3. The child of `Mesh` is `Material`, and each mesh does not have more than one material.
4. The child of `Material` is `Texture`, and each `Material` may contain many different types of textures, but there will not be more than one texture of the same type.

### Transformation rules

Ash Engine supports basic transformations (translation, rotation, scaling) on ​​models and meshes in the following ways:

For nodes in the tree typed `Model` or `Mesh`, the child uses a local coordinate system relative to its father (the child's position, rotation angle, and scaling ratio are relative to his father), and the position, rotation angles, and scaling ratio you seen in its property widget are relative (or local).

When acquiring the absolute position of a model or mesh (relative to world coordinate system), its own transformation matrix is ​​first applied, followed by the transformation matrix of its father and ancestors. The transformation matrix (`M`) is defined as follows:

```
M = T * R * S
```

Among them, `T` represents the translation matrix, `R` represents the rotation matrix, and `S` represents the scaling matrix. The scaling transform is applied first, followed by rotation, and finally translation.

## Import and export models

Select `File` --> `Import Model` from the menu, or drag and drop the model file directly into the window to import the model.

Select a mesh or model and select `File` --> `Export Model` from the menu to export the model.

## Light

### Types

Ash Engine supports four types of light sources, and up to eight for each type of light source:

| Type | Attribute |
|------|-----------|
| Ambient Light     | Color, Intensity |
| Directional Light | Color, Intensity, Direction |
| Point Light | Color, Intensity, Position, Attenuation|
| Spot Light  | Color, Intensity, Position, Direction, Cut-offs, Attenuation|

### Attenuation

Point lights and spotlights have attenuation properties that allow for a more realistic simulation of real-world light sources. Ash Engine uses quadratic attenuation with the following formula:

![](images/attenuation.svg)

`d` represents the distance of the object from the light source. `Kc`, `Kl`, `Kq` are constant terms, linear terms, and quadratic terms, respectively:

* The constant term usually stays at 1.0, ensuring that the denominator will never be smaller than 1
* The linear term is multiplied by the distance and reduces the light intensity linearly
* The quadratic term is multiplied by the square of the distance and reduce the light intensity quadratically

Choosing the right values:

|distance|constant|linear|quadratic|
|----|----|-----|-----|
|7 |1.0 |0.7 |1.8 |
|13 |1.0 |0.35 |0.44 |
|20 |1.0 |0.22 |0.20 |
|32 |1.0 |0.14 |0.07 |
|50 |1.0 |0.09 |0.032|
|65 |1.0 |0.07 |0.017|
|100 |1.0 |0.045|0.0075|
|160 |1.0 |0.027|0.0028|
|200 |1.0 |0.022|0.0019|
|325 |1.0 |0.014|0.0007|
|600 |1.0 |0.007|0.0002|
|3250|1.0 |0.0014|0.000007|

In Ash Engine, the default parameter for attenuation is the corresponding parameter in the table with a distance of 325.

## Moving

* Use `W` and `S` to move forward or backward
* Use `A` and `D` to move left or right
* Use `E` and `Q` to move up or down
* Use `Shift` to get a 5x speed up
* Press and hold the left mouse button and move it to change viewport

## 3D Gizmos

3D gizmos help users translate, rotate, or scale objects along a specific axis or plane. Three types of 3D gizmos are supported:

* 3D Translate Gizmo: Translate a selected object along an axis
* 3D Rotate Gizmo: Rotate a selected object around an axis
* 3D Scale Gizmo: Scale a selected object along an axis

