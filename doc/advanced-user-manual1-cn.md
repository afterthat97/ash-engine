# 高级用户文档一：场景结构

## 类型

在 Ash Engine 中，一个场景（Scene）中可能包括以下对象：

* Camera（相机）
* Light（光源）
* Gridline（网格线）
* Model（模型）
* Mesh（网格）
* Material（材质）
* Texture（纹理）

## 关系

在 Ash Engine 中，场景中的各个对象及它们的联系是以树状结构表示的。它们的层次关系是：

1. 场景是根节点，它的孩子可能有相机、光源、网格线、模型。
2. 模型的孩子可能是网格，也可能是其他模型。
3. 网格的孩子是材质，且每个网格不会拥有多个一个材质。
4. 材质的孩子是纹理，每个材质可能包含多种不同类型的纹理，但同种类型的纹理不会有多个。

## 示例

下面的代码展示了如何创建一个场景，并向场景中添加模型和网格：

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

将 `src/AshEngine.cpp` 中的 `main` 函数的内容替换为以上代码，重新编译执行，可以在控制台中得到以下输出：

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

以上输出（不关键的输出已被 `...` 省略）描述了整个场景的树状结构，缩进的长度表示该结点在树中的深度。

### 日志级别

为了方便调试，Ash Engine 会在运行时在控制台输出日志信息。此外，Ash Engine 还支持三种日志级别，分别是：

* `LOG_LEVEL_ERROR`：仅输出错误日志
* `LOG_LEVEL_WARNING`：输出警告和错误日志
* `LOG_LEVEL_INFO`：输出所有日志（包括信息、警告和错误）

默认的日志级别是 `LOG_LEVEL_WARNING`，你可以随意修改，例如设置为 `LOG_LEVEL_INFO`：

```
log_level = LOG_LEVEL_INFO;
```

此时，对于示例中的代码，你还可以得到以下输出：

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
