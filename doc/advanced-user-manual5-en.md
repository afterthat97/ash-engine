# Advanced User Manual 5: Basic Transformation

## Basic transformation

Basic transformation consists of three operations:

* Translate
* Rotate
* Scale

In Ash Engine, you can translate, rotate, or scale a model or mesh using its `translate()`, `rotate()`, `scale()` functions, or set its basic transformation parameters directly using its `setPosition()`, `setRotation()`, `setScaling()` functions.

## Example

The following code shows how to add 100 cubes to the scene and set random basic transformation parameters for them:

```cpp
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
    for (int i = 0; i < 100; i++)
        scene->addModel(ModelLoader::loadCubeModel());

    for (int i = 0; i < scene->models().size(); i++) {
        scene->models()[i]->setPosition(QVector3D(rand() % 30, rand() % 30, rand() % 30));
        scene->models()[i]->setRotation(QVector3D(rand() % 360, rand() % 360, rand() % 360));
        scene->models()[i]->setScaling(QVector3D(rand() % 2 + 1, rand() % 2 + 1, rand() % 2 + 1));
    }

    DirectionalLight* directionalLight = new DirectionalLight(QVector3D(1, 1, 1), QVector3D(-2, -4, -3));
    scene->addLight(directionalLight);

    window->show();

    return a.exec();
}
```

Replace the `main` function in `src/AshEngine.cpp` with the above code, recompile and execute, you can get the following results:

![](images/advanced-user-manual5.jpg)