#include "utilities.h"
#include "extmath.h"
#include "skybox.h"
#include "scene.h"
#include "camera.h"
#include "light.h"
#include "glconfig.h"
#include "shader.h"
#include "axis.h"
#include "gridlines.h"
#include "cursor.h"
#include "window.h"

#define MAX_LIGHTS 8

Window window(960, 640, "masterEngine");
Cursor cursor;
Camera camera;
int32_t fps = 0, canMove = 1;
vector<string> faces { "RT.tga", "LF.tga", "UP.tga", "DN.tga", "BK.tga", "FR.tga" };

Skybox *skybox;
Axis *localAxis;
Gridlines *gridlines;
vector<Light*> lights;
vector<Scene*> scenes;

float moveSpeed = 1.0f;
btDiscreteDynamicsWorld* dynamicsWorld = NULL;
Shader meshShader, skyboxShader, depthShader;
Mesh *selectedMesh, *copyedMesh;
vec3 lastIntersection;
enum MouseDrag { NONE, TRANSLATE_X, TRANSLATE_Y, TRANSLATE_Z, ROTATION_X, ROTATION_Y, ROTATION_Z };
MouseDrag dragStatus;
double lastTime;

void windowSizeCallback(GLFWwindow*, int32_t width, int32_t height) {
    window.resize(width, height);
}

void mouseButtonCallback(GLFWwindow*, int button, int action, int mods) {
    cursor.update(window.getGLFWwindow());
    int handled = 0;
    TwMouseAction twAction = (action == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        handled = TwMouseButton(twAction, TW_MOUSE_LEFT);
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        handled = TwMouseButton(twAction, TW_MOUSE_RIGHT);
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        handled = TwMouseButton(twAction, TW_MOUSE_MIDDLE);

    // Not handled by AntTweakBar
    if (handled || button != GLFW_MOUSE_BUTTON_LEFT) return;

    if (action == GLFW_PRESS) {
        lastTime = glfwGetTime();
        vec4 raySt, rayEd;
        screenPosToWorldRay(cursor.getCntPosfv(), window.getWindowSizefv(), window.getProjMatrix(), camera.getViewMatrix(), raySt, rayEd);
        btCollisionWorld::AllHitsRayResultCallback allHits(btVector3(raySt.x, raySt.y, raySt.z), btVector3(rayEd.x, rayEd.y, rayEd.z));
        dynamicsWorld->rayTest(btVector3(raySt.x, raySt.y, raySt.z), btVector3(rayEd.x, rayEd.y, rayEd.z), allHits);
        if (selectedMesh != NULL) {
            for (int32_t i = 0; i < allHits.m_collisionObjects.size(); i++) {
                vec3 tmp;
                Mesh* selectedAxis = (Mesh*) allHits.m_collisionObjects[i]->getUserPointer();
                if (selectedAxis->name == "ATVIEW_AXIS_transX") {
                    getClosestPointOfLineLine(localAxis->getPosition(), vec3(1, 0, 0), raySt, rayEd - raySt, lastIntersection, tmp);
                    dragStatus = TRANSLATE_X;
                } else if (selectedAxis->name == "ATVIEW_AXIS_transY") {
                    getClosestPointOfLineLine(localAxis->getPosition(), vec3(0, 1, 0), raySt, rayEd - raySt, lastIntersection, tmp);
                    dragStatus = TRANSLATE_Y;
                } else if (selectedAxis->name == "ATVIEW_AXIS_transZ") {
                    getClosestPointOfLineLine(localAxis->getPosition(), vec3(0, 0, 1), raySt, rayEd - raySt, lastIntersection, tmp);
                    dragStatus = TRANSLATE_Z;
                } else if (selectedAxis->name == "ATVIEW_AXIS_rotX") {
                    getIntersectionOfLinePlane(raySt, rayEd - raySt, localAxis->getPosition(), vec3(1, 0, 0), lastIntersection);
                    dragStatus = ROTATION_X;
                } else if (selectedAxis->name == "ATVIEW_AXIS_rotY") {
                    getIntersectionOfLinePlane(raySt, rayEd - raySt, localAxis->getPosition(), vec3(0, 1, 0), lastIntersection);
                    dragStatus = ROTATION_Y;
                } else if (selectedAxis->name == "ATVIEW_AXIS_rotZ") {
                    getIntersectionOfLinePlane(raySt, rayEd - raySt, localAxis->getPosition(), vec3(0, 0, 1), lastIntersection);
                    dragStatus = ROTATION_Z;
                } else
                    dragStatus = NONE;
                if (selectedAxis->name.substr(0, 11) == "ATVIEW_AXIS") {
                    lastTime = 0;
                    return;
                }
            }
        }
        float mindis = FLT_MAX;
        for (int32_t i = 0; i < allHits.m_collisionObjects.size(); i++)
            if (((Mesh*) allHits.m_collisionObjects[i]->getUserPointer())->name.substr(0, 11) != "ATVIEW_AXIS") {
                if (distance(camera.pos, toVec3(allHits.m_hitPointWorld[i])) > mindis) continue;
                if (selectedMesh != NULL) selectedMesh->deselect();
                mindis = distance(camera.pos, toVec3(allHits.m_hitPointWorld[i]));
                selectedMesh = (Mesh*) allHits.m_collisionObjects[i]->getUserPointer();
                selectedMesh->select();
                localAxis->setPosition(selectedMesh->getPosition());
                localAxis->show();
                lastTime = 0;
            }
    } else { // Release
        dragStatus = NONE;
        lastIntersection = vec3(0.0);
        if (glfwGetTime() - lastTime < 0.2f) {
            if (selectedMesh != NULL) selectedMesh->deselect();
            selectedMesh = NULL;
            localAxis->hide();
        }
    }
}

void cursorPosCallback(GLFWwindow*, double xpos, double ypos) {
    TwMouseMotion((int32_t) xpos * window.getScaleRatio(), (int32_t) ypos * window.getScaleRatio());
}

void keyCallback(GLFWwindow*, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
    if (action != GLFW_PRESS) return;

    int k = 0;
    switch (key) {
        case GLFW_KEY_ESCAPE:    k = TW_KEY_ESCAPE;    break;
        case GLFW_KEY_UP:        k = TW_KEY_UP;        break;
        case GLFW_KEY_DOWN:      k = TW_KEY_DOWN;      break;
        case GLFW_KEY_LEFT:      k = TW_KEY_LEFT;      break;
        case GLFW_KEY_RIGHT:     k = TW_KEY_RIGHT;     break;
        case GLFW_KEY_TAB:       k = TW_KEY_TAB;       break;
        case GLFW_KEY_ENTER:     k = TW_KEY_RETURN;    break;
        case GLFW_KEY_BACKSPACE: k = TW_KEY_BACKSPACE; break;
        case GLFW_KEY_INSERT:    k = TW_KEY_INSERT;    break;
        case GLFW_KEY_DELETE:    k = TW_KEY_DELETE;    break;
        case GLFW_KEY_PAGE_UP:   k = TW_KEY_PAGE_UP;   break;
        case GLFW_KEY_PAGE_DOWN: k = TW_KEY_PAGE_DOWN; break;
        case GLFW_KEY_HOME:      k = TW_KEY_HOME;      break;
        case GLFW_KEY_END:       k = TW_KEY_END;       break;
        case GLFW_KEY_KP_ENTER:  k = TW_KEY_RETURN;    break;
    }

    if (k > 0 && TwKeyPressed(k, TW_KMOD_NONE)) return;

    switch (key) {
        case GLFW_KEY_F10:
            window.screenshot();
            break;
        case GLFW_KEY_C:
            if (mods == GLFW_MOD_CONTROL && selectedMesh != NULL)
                copyedMesh = selectedMesh;
            break;
        case GLFW_KEY_V:
            if (mods == GLFW_MOD_CONTROL && copyedMesh != NULL) {
                if (copyedMesh->name == "ATVIEW_LIGHT") {
                    if (lights.size() >= MAX_LIGHTS) {
                        reportWarning("You have reached the maximum number of lights, new lights will not be created.");
                        return;
                    }
                    Light* newLight = new Light(*(Light*) copyedMesh);
                    newLight->addTranslation(vec3(newLight->getSize().x * 1.2, 0, 0));
                    lights.push_back(newLight);
                    if (selectedMesh != NULL)
                        selectedMesh->deselect();
                    copyedMesh = selectedMesh = newLight;
                    selectedMesh->select();
                    localAxis->setPosition(newLight->getPosition());
                    reportInfo("Light " + to_string(lights.size() - 1) + " pasted");
                } else {
                    Model* parentModel = ((Model*) copyedMesh->getParent());
                    Mesh *newMesh = new Mesh(*copyedMesh);
                    newMesh->addTranslation(vec3(newMesh->getSize().x * 1.2, 0, 0));
                    parentModel->addMesh(newMesh);
                    if (selectedMesh != NULL)
                        selectedMesh->deselect();
                    copyedMesh = selectedMesh = newMesh;
                    selectedMesh->select();
                    localAxis->setPosition(newMesh->getPosition());
                    reportInfo("Mesh " + selectedMesh->name + " pasted");
                }
            }
            break;
        case GLFW_KEY_BACKSPACE:
            if (selectedMesh != NULL && selectedMesh->name == "ATVIEW_LIGHT") {
                for (uint32_t i = 0; i < lights.size(); i++)
                    if (lights[i] == selectedMesh) {
                        lights.erase(lights.begin() + i);
                    }
                delete (Light*) selectedMesh;
            } else if (selectedMesh != NULL) {
                Model* parentModel = ((Model*) selectedMesh->getParent());
                parentModel->removeMesh(selectedMesh);
                delete selectedMesh;
            }
            if (selectedMesh == copyedMesh) copyedMesh = NULL;
            selectedMesh = NULL;
            localAxis->hide();
            break;
    }
    for (uint32_t i = 0; i < scenes.size(); i++)
        scenes[i]->recycle();
}

void charCallback(GLFWwindow*, uint32_t key) {
    canMove = !TwKeyPressed(key, TW_KMOD_NONE);
}

void dropCallBack(GLFWwindow*, int count, const char** paths) {
    for (int i = 0; i < count; i++) try {
        Scene *newScene = new Scene(paths[i], dynamicsWorld);
        newScene->dumpinfo("");
        scenes.push_back(newScene);
    } catch (const string msg) {
        cerr << msg << endl;
    }
}

void processInput() {
    // Move camera by keyboard
    if (canMove) {
        float shift = 1.0f;
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) shift = 5.0f;
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) shift = 5.0f;
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS) camera.moveForward(shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS) camera.moveForward(-shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS) camera.moveRight(-shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS) camera.moveRight(shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_Q) == GLFW_PRESS) camera.moveUp(-shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_E) == GLFW_PRESS) camera.moveUp(shift * moveSpeed);
    }

    // Change camera viewport by mouse motion
    cursor.update(window.getGLFWwindow());
    if (dragStatus == TRANSLATE_X || dragStatus == TRANSLATE_Y || dragStatus == TRANSLATE_Z) {
        vec4 raySt, rayEd;
        vec3 p, tmp, translateAxis(dragStatus == TRANSLATE_X, dragStatus == TRANSLATE_Y, dragStatus == TRANSLATE_Z);
        screenPosToWorldRay(cursor.getCntPosfv(), window.getWindowSizefv(), window.getProjMatrix(), camera.getViewMatrix(), raySt, rayEd);
        getClosestPointOfLineLine(localAxis->getPosition(), translateAxis, raySt, rayEd - raySt, p, tmp);
        selectedMesh->addTranslation(p - lastIntersection);
        localAxis->addTranslation(p - lastIntersection);
        lastIntersection = p;
    } else if (dragStatus == ROTATION_X || dragStatus == ROTATION_Y || dragStatus == ROTATION_Z) {
        vec4 raySt, rayEd;
        vec3 p, rotationAxis(dragStatus == ROTATION_X, dragStatus == ROTATION_Y, dragStatus == ROTATION_Z);
        screenPosToWorldRay(cursor.getCntPosfv(), window.getWindowSizefv(), window.getProjMatrix(), camera.getViewMatrix(), raySt, rayEd);
        getIntersectionOfLinePlane(raySt, rayEd - raySt, localAxis->getPosition(), rotationAxis, p);
        vec3 v1 = lastIntersection - localAxis->getPosition(), v2 = p - localAxis->getPosition();
        float theta = acos(fmin(fmax(dot(v1, v2) / length(v1) / length(v2), -1.0f), 1.0f));
        if (dot(rotationAxis, cross(v1, v2)) < 0) theta = -theta;
        selectedMesh->addRotation(theta * rotationAxis);
        lastIntersection = p;
    } else if (glfwGetMouseButton(window.getGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        camera.turnLeft(-cursor.getDeltafv().x / 500.0f);
        camera.lookUp(-cursor.getDeltafv().y / 500.0f);
    }
}

void render() {
    glViewport(0, 0, window.getFrameSizei().first, window.getFrameSizei().second);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    // OpenGL configurations
    glPolygonMode(GL_FRONT_AND_BACK, (polygonModeStr == "LINE" ? GL_LINE : GL_FILL));
    if (enableFaceCulling) glEnable(GL_CULL_FACE);

    // Get projection and view matrix
    mat4 projection = window.getProjMatrix();
    mat4 view = camera.getViewMatrix();

    // Set shader for meshes (models)
    meshShader.use();
    meshShader.setMat4("projection", projection);
    meshShader.setMat4("view", view);
    meshShader.setVec3("viewPos", camera.pos);

    if (enableLight) {
        meshShader.setInt("lightNum", (int32_t) lights.size());
        meshShader.setInt("enableLight", 0);
		meshShader.setInt("enableAttenuation", enableAttenuation);
        for (uint32_t i = 0; i < lights.size(); i++) {
            string idx = "lights[" + to_string(i) + "]";
            glActiveTexture(GL_TEXTURE8 + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i]->getDepthMap());
            meshShader.setBool(idx + ".hasDepthMap", enableShadow);
            meshShader.setInt("depthMap[" + to_string(i) + "]", 8 + i);
            meshShader.setFloat(idx + ".farPlane", lights[i]->getFarPlane());
            meshShader.setVec3(idx + ".pos", lights[i]->getPosition());
            meshShader.setVec3(idx + ".color", lights[i]->getColor());
            lights[i]->render(meshShader);
        }
    }

    // Render skybox
    skyboxShader.use();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
    skybox->render(skyboxShader);

    // Render scenes
    meshShader.use();
    meshShader.setInt("enableLight", enableLight);
    for (uint32_t i = 0; i < scenes.size(); i++)
        scenes[i]->render(meshShader);

    // Render axis and gridlines
    glDisable(GL_CULL_FACE);
    meshShader.use();
    meshShader.setInt("enableLight", 0);
    if (enableGridlines) gridlines->render(meshShader);
    localAxis->render(meshShader, camera.pos);

    // Draw tweak bars
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    TwDraw();
}

void glCheckError() {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                               error = "Unknown error"; break;
        }
        reportError("OpenGL internal error: " + error);
    }
}

int main(int argc, char **argv) {
#ifdef APPLE_MACOS
    CGEventSourceRef evsrc = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    CGEventSourceSetLocalEventsSuppressionInterval(evsrc, 0.0);
#endif

    // Initialize GLEW library
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        reportError("Failed to initialize GLAD library");
        exit(-1);
    }

    // Dump hardware and software info
    string rendererInfo = "GPU: " + (string) (char*) glGetString(GL_RENDERER);
    reportInfo(rendererInfo);
    string glVersionInfo = "OpenGL Version: " + (string) (char*) glGetString(GL_VERSION);
    reportInfo(glVersionInfo);
    string glShadingLanguageVersionInfo = "Shading Language Version: " + (string) (char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
    reportInfo(glShadingLanguageVersionInfo);

    // Load shaders from file
    try {
        meshShader.loadFromString(meshVertexShaderCode, meshFragmentShaderCode, "");
        skyboxShader.loadFromString(skyboxVertexShaderCode, skyboxFragmentShaderCode, "");
        depthShader.loadFromString(depthVertexShaderCode, depthFragmentShaderCode, depthGeometryShaderCode);
    } catch (const string msg) {
        cerr << msg << endl;
        exit(4);
    }

    // Initialize bullet physics engine
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10.0f, 0));

    // Load scenes from file
    for (int i = 1; i < argc; i++) try {
        Scene *newScene = new Scene(argv[i], dynamicsWorld);
        newScene->dumpinfo("");
        scenes.push_back(newScene);
    } catch (const string msg) {
        cerr << msg << endl;
    }

    skybox = new Skybox();
    localAxis = new Axis(dynamicsWorld);
    gridlines = new Gridlines();
    lights.push_back(new Light(vec3(1.0), dynamicsWorld));
    lights[0]->setPosition(vec3(100.0));
    localAxis->hide();

    meshShader.use();
    for (int i = 0; i < MAX_LIGHTS; i++) {
        glActiveTexture(GL_TEXTURE8 + i);
        meshShader.setInt("depthMap[" + to_string(i) + "]", 8 + i);
    }

    // Initialize skybox
    try {
        skybox->loadCubeMap(faces);
    } catch (const string msg) {
        reportInfo("Skybox textures not found, using default color.");
    }

    // Initialize camera
    camera.pos = vec3(50.0f, 50.0f, 300.0f);

    // Add tweak bars
    TwInit(TW_OPENGL_CORE, NULL);
    TwDefine("GLOBAL fontsize=3");
    TwWindowSize(window.getFrameSizei().first, window.getFrameSizei().second);

    // Show FPS and camara info
    TwBar * viewerInfoBar = TwNewBar("Viewer Info");
    TwSetParam(viewerInfoBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(viewerInfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 10");
    TwSetParam(viewerInfoBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 130");
    TwAddVarRO(viewerInfoBar, "FPS", TW_TYPE_INT32, &fps, "");
    TwAddVarRW(viewerInfoBar, "Moving Speed", TW_TYPE_FLOAT, &moveSpeed, "step=0.01");
    TwAddVarRW(viewerInfoBar, "Camera Position X", TW_TYPE_FLOAT, &camera.pos.x, "step=0.1");
    TwAddVarRW(viewerInfoBar, "Camera Position Y", TW_TYPE_FLOAT, &camera.pos.y, "step=0.1");
    TwAddVarRW(viewerInfoBar, "Camera Position Z", TW_TYPE_FLOAT, &camera.pos.z, "step=0.1");

    // Show OpenGL config
    TwBar * configBar = TwNewBar("Configuration");
    TwSetParam(configBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(configBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 150");
    TwSetParam(configBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 210");
    TwAddVarRW(configBar, "Diffuse Map", TW_TYPE_BOOLCPP, &enableDiffuseMap, "");
    TwAddVarRW(configBar, "Specular Map", TW_TYPE_BOOLCPP, &enableSpecularMap, "");
    TwAddVarRW(configBar, "Normal Map", TW_TYPE_BOOLCPP, &enableNormalMap, "");
    TwAddVarRW(configBar, "Parallax Map", TW_TYPE_BOOLCPP, &enableParallaxMap, "");
    TwAddVarRW(configBar, "Lighting", TW_TYPE_BOOLCPP, &enableLight, "");
    TwAddVarRW(configBar, "Attenuation", TW_TYPE_BOOLCPP, &enableAttenuation, "");
    TwAddVarRW(configBar, "Shadow", TW_TYPE_BOOLCPP, &enableShadow, "");
    TwAddVarRW(configBar, "Backface Culling", TW_TYPE_BOOLCPP, &enableFaceCulling, "");
    TwAddVarRW(configBar, "Gridlines", TW_TYPE_BOOLCPP, &enableGridlines, "");
    TwAddVarCB(configBar, "Polygon Mode", TW_TYPE_STDSTRING, setPolygonMode, getPolygonMode, &polygonModeStr, NULL);

    // Show application info
    TwBar * appInfoBar = TwNewBar("Application Info");
    TwSetParam(appInfoBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(appInfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 370");
    TwSetParam(appInfoBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 120");
    TwAddButton(appInfoBar, "1.0", NULL, NULL, "label='App Version: v0.4.0'");
    TwAddButton(appInfoBar, "1.1", NULL, NULL, ("label='" + rendererInfo + "'").c_str());
    TwAddButton(appInfoBar, "1.2", NULL, NULL, ("label='" + glVersionInfo + "'").c_str());
    TwAddButton(appInfoBar, "1.3", NULL, NULL, ("label='" + glShadingLanguageVersionInfo + "'").c_str());

    // Set callback functions
    glfwSetWindowSizeCallback(window.getGLFWwindow(), windowSizeCallback);
    glfwSetMouseButtonCallback(window.getGLFWwindow(), mouseButtonCallback);
    glfwSetCursorPosCallback(window.getGLFWwindow(), cursorPosCallback);
    glfwSetKeyCallback(window.getGLFWwindow(), keyCallback);
    glfwSetCharCallback(window.getGLFWwindow(), charCallback);
    glfwSetDropCallback(window.getGLFWwindow(), dropCallBack);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window.getGLFWwindow(), GLFW_STICKY_KEYS, GL_TRUE);

    // Set Camera viewport
    glViewport(0, 0, window.getFrameSizei().first, window.getFrameSizei().second);

    // Start loop
    glEnable(GL_DEPTH_TEST);
    double lastTime = glfwGetTime();
    for (uint32_t totframes = 0;; totframes++) {
        if (glfwWindowShouldClose(window.getGLFWwindow())) break;
        double cntTime = glfwGetTime();
        if (cntTime - lastTime >= 1.0f) {
            fps = totframes;
            totframes = 0;
            lastTime += 1.0f;
        }
        processInput();
        if (enableShadow)
            for (uint32_t i = 0; i < lights.size(); i++)
                lights[i]->renderDepthMap(scenes, depthShader);
        render();
        glfwSwapBuffers(window.getGLFWwindow());
        glfwPollEvents();
        glCheckError();
    }

    // Clean
    TwTerminate();
    glfwTerminate();
    return 0;
}

