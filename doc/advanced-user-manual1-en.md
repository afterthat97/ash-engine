# Advanced User Manual 1: Scene Structure

## Types

In Ash Engine, a scene may include the following objects:

* Camera
* Light
* Gridline
* Model
* Mesh
* Material
* Texture

## Relationship

In Ash Engine, the objects in the scene and their relationships are represented by a **tree structure**:

1. `Scene` is the root node, and `Camera`, `Light`, `Gridline`, and `Model` can be its children.
2. The child of `Model` may be `Mesh` or other `Model`.
3. The child of `Mesh` is `Material`, and each mesh does not have more than one material.
4. The child of `Material` is `Texture`, and each `Material` may contain many different types of textures, but there will not be more than one texture of the same type.

## Example

The following code shows how to create a scene and add models and meshes to the scene:

```cpp
int main(int argc, char *argv[]) {
    Scene* scene = new Scene;

    Model* childModel1 = new Model;
    childModel1->setObjectName("Child Model 1");
    childModel1->addChildMesh(new Mesh);
    childModel1->addChildMesh(new Mesh);

    Model* childModel2 = new Model;
    childModel2->setObjectName("Child Model 2");
    childModel2->addChildMesh(new Mesh);

    Mesh* childMesh1 = new Mesh;
    childMesh1->setObjectName("Child Mesh 1");

    Model* topModel = new Model;
    topModel->setObjectName("Top Model");
    topModel->addChildModel(childModel1);
    topModel->addChildModel(childModel2);
    topModel->addChildMesh(childMesh1);

    scene->addModel(topModel);
    scene->dumpObjectTree();

    return 0;
}
```

Replace the `main` function in `src/AshEngine.cpp` with the above code, recompile and execute, and you can get the following output in the console:

```
Scene: "Untitled Scene"
0 gridline(s), 0 ambient light(s), 0 directional light(s), 0 point light(s), 0 spotlights(s), 1 model(s)
    Camera: "Camera"
        ...
    Model: "Top Model"
        ...
        1 child mesh(es), 2 child model(s)
        Mesh: "Child Mesh 1"
            ...
            0 vertices, 0 indices, 0 material
        Model: "Child Model 1"
            ...
            2 child mesh(es), 0 child model(s)
            Mesh: "Untitled Mesh"
                ...
                0 vertices, 0 indices, 0 material
            Mesh: "Untitled Mesh"
                ...
                0 vertices, 0 indices, 0 material
        Model: "Child Model 2"
            ...
            1 child mesh(es), 0 child model(s)
            Mesh: "Untitled Mesh"
                ...
                0 vertices, 0 indices, 0 material
```

The above output (the uncritical output has been omitted by `...`) describes the tree structure of the entire scene, and the length of the indentation indicates the depth of node in the tree.

### Log level

To facilitate debugging, Ash Engine prints logs to the console at runtime. In addition, Ash Engine supports three log levels, namely:

* `LOG_LEVEL_ERROR`: only print error logs
* `LOG_LEVEL_WARNING`: print warning and error logs
* `LOG_LEVEL_INFO`: print all logs (including info, warning and error logs)

The default log level is `LOG_LEVEL_WARNING`. You can modify it at will, for example, set to `LOG_LEVEL_INFO`:

```
log_level = LOG_LEVEL_INFO;
```

At this point, for the code in the example, you can also get the following output:

```
setCamera(Scene.cpp:59): Camera "Camera" is assigned to scene "Untitled Scene"
addChildMesh(Model.cpp:29): Mesh "Untitled Mesh" is added to model "Child Model 1" as child
addChildMesh(Model.cpp:29): Mesh "Untitled Mesh" is added to model "Child Model 1" as child
addChildMesh(Model.cpp:29): Mesh "Untitled Mesh" is added to model "Child Model 2" as child
addChildModel(Model.cpp:43): Model "Child Model 1" is added to model "Top Model" as child
addChildModel(Model.cpp:43): Model "Child Model 2" is added to model "Top Model" as child
addChildMesh(Model.cpp:29): Mesh "Child Mesh 1" is added to model "Top Model" as child
addModel(Scene.cpp:182): Model "Top Model" is added to scene "Untitled Scene"
```
