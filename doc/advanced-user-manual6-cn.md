# 高级用户文档六：自定义渲染循环

## 渲染循环

渲染器一次只能渲染一张画面，要形成连贯的动画就需要渲染循环。默认情况下，渲染循环仅仅处理用户输入（键鼠操作），使用户可以在场景中自由移动。

如果想要实现动态的场景（场景中的对象每一帧都在变化），就需要在默认情况的基础上添加自定义渲染循环。

## 示例

下面的代码展示了如何编写自定义渲染循环函数，实现三个动态光源。

`customRenderingLoop(Scene* scene)` 即为自定义的渲染循环函数，它在**每帧**画面的渲染前被调用。输入参数 `Scene* scene` 即为我们在 `main` 中创建的场景 `scene`。

> 想了解实现原理的用户，请参考 C/C++ 中的函数指针：[Function Pointers in C and C++](https://www.cprogramming.com/tutorial/function-pointers.html)

```cpp
void customRenderingLoop(Scene* scene) {
    PointLight* light0 = scene->pointLights()[0];
    PointLight* light1 = scene->pointLights()[1];
    PointLight* light2 = scene->pointLights()[2];

    static int angle0 = 0, angle1 = 90, angle2 = 180;
    angle0++; angle1++; angle2++;

    light0->setPosition(QVector3D(10 * sin(rad(angle0)), 0, 10 * cos(rad(angle0))));
    light1->setPosition(QVector3D(10 * sin(rad(angle1)), 10 * cos(rad(angle1)), 0));
    light2->setPosition(QVector3D(0, 10 * cos(rad(angle2)), 10 * sin(rad(angle2))));
}

int main(int argc, char *argv[]) {
    initApplication();

    QApplication a(argc, argv);

    // Step 1: Create an OpenGLWindow
    OpenGLWindow* window = new OpenGLWindow;
    window->resize(800, 600);
    window->setEnableMousePicking(false); // Disable mouse picking to improve performance

    // Step 2: Create an OpenGLRenderer for the window
    OpenGLRenderer* renderer = new OpenGLRenderer;
    window->setRenderer(renderer);

    // Step 3: Create a scene
    Scene* scene = new Scene;
    window->setScene(new OpenGLScene(scene));

    // Step 4: Customize your scene
    scene->camera()->moveForward(400.0f);

    Model* model = ModelLoader::loadCubeModel();
    model->setScaling(QVector3D(10, 10, 10));
    scene->addModel(model);

    scene->addLight(new PointLight);
    scene->addLight(new PointLight);
    scene->addLight(new PointLight);

    // Pass function pointer to OpenGLWindow
    window->setCustomRenderingLoop(customRenderingLoop);
    window->show();

    return a.exec();
}
```

将 `src/AshEngine.cpp` 中的 `main` 函数的内容替换为以上代码，重新编译执行，你可以得到以下结果：

![](images/advanced-user-manual6.gif)