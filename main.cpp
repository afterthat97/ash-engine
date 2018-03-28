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

// Create one 960 * 640 window
Window window(960, 640, "masterEngine");

// Deal with cursor movement
Cursor cursor;

// Perspective camera
Camera camera;

// Axis for translation and rotation
Axis *localAxis;

// Gridlines in the scene
Gridlines *gridlines;

// Lights in the scene
#define MAX_LIGHTS 8
vector<Light*> lights;

// All loaded scenes
vector<Scene*> scenes;

// Bullet dynamics
btDiscreteDynamicsWorld* dynamicsWorld = NULL;

// OpenGL Shader
Shader meshShader, skyboxShader, depthShader;

// For copy-and-paste function
Mesh *selectedMesh, *copyedMesh;

// Deal with mouse click and hold
double lastTime;

// Other vars
float fps = 60, singleFrameRenderTime;
bool canMove = 1;
float moveSpeed = 1.0f;
vec3 backgroundColor(0.7f);
uint32_t windowWidth, windowHeight, frameWidth, frameHeight;
bool retina = 0;

// Callback when user stretch the window
void windowSizeCallback(GLFWwindow*, int32_t width, int32_t height) {
    window.resize(width, height);
	windowWidth = window.getWindowSizei().first;
	windowHeight = window.getWindowSizei().second;
	frameWidth = window.getFrameSizei().first;
	frameHeight = window.getFrameSizei().second;
	retina = (frameWidth > windowWidth);
}

// Callback when user click the mouse
void mouseButtonCallback(GLFWwindow*, int button, int action, int mods) {
	// Only handle left button actions
	if (button != GLFW_MOUSE_BUTTON_LEFT) return;

	// Update currnet cursor position
    cursor.update(window.getGLFWwindow());

	// If the click is handled by AntTweakBar, just ignore it
    TwMouseAction twAction = (action == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	if (TwMouseButton(twAction, TW_MOUSE_LEFT)) return;

    if (action == GLFW_PRESS) {
        lastTime = glfwGetTime();
        vec4 raySt, rayEd;
        screenPosToWorldRay(cursor.getCntPosfv(), window.getWindowSizefv(), window.getProjMatrix(), camera.getViewMatrix(), raySt, rayEd);
        btCollisionWorld::AllHitsRayResultCallback allHits(btVector3(raySt.x, raySt.y, raySt.z), btVector3(rayEd.x, rayEd.y, rayEd.z));
        dynamicsWorld->rayTest(btVector3(raySt.x, raySt.y, raySt.z), btVector3(rayEd.x, rayEd.y, rayEd.z), allHits);
            for (int32_t i = 0; i < allHits.m_collisionObjects.size(); i++)
				if (localAxis->startDrag((Mesh*) allHits.m_collisionObjects[i]->getUserPointer(), raySt, rayEd)) {
                    lastTime = 0;
                    return;
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
		localAxis->stopDrag();
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
		case GLFW_KEY_F1:
			localAxis->setTransformMode(TRANSLATION);
			break;
		case GLFW_KEY_F2:
			localAxis->setTransformMode(ROTATION);
			break;
		case GLFW_KEY_F3:
			localAxis->setTransformMode(SCALING);
			break;
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
        float shift = 60.0f * singleFrameRenderTime;
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) shift *= 5.0f;
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) shift *= 5.0f;
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS) camera.moveForward(shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS) camera.moveForward(-shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS) camera.moveRight(-shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS) camera.moveRight(shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_Q) == GLFW_PRESS) camera.moveUp(-shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_E) == GLFW_PRESS) camera.moveUp(shift * moveSpeed);
    }

    // Change camera viewport by mouse motion
    cursor.update(window.getGLFWwindow());
    vec4 raySt, rayEd;
    screenPosToWorldRay(cursor.getCntPosfv(), window.getWindowSizefv(), window.getProjMatrix(), camera.getViewMatrix(), raySt, rayEd);
	if (localAxis->continueDrag(selectedMesh, raySt, rayEd) == false && glfwGetMouseButton(window.getGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        camera.turnLeft(-cursor.getDeltafv().x / 500.0f);
        camera.lookUp(-cursor.getDeltafv().y / 500.0f);
    }
}

void render() {
    glViewport(0, 0, window.getFrameSizei().first, window.getFrameSizei().second);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);

    // OpenGL configurations
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

    // Render scenes
    glPolygonMode(GL_FRONT_AND_BACK, (polygonModeStr == "LINE" ? GL_LINE : GL_FILL));
    meshShader.setInt("enableLight", enableLight);
    for (uint32_t i = 0; i < scenes.size(); i++)
        scenes[i]->render(meshShader);

    // Render axis and gridlines
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    meshShader.setInt("enableLight", 0);
    if (enableGridlines) gridlines->render(meshShader);
    localAxis->render(meshShader, camera.pos);

    // Draw tweak bars
    TwDraw();
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
    if (meshShader.loadFromString(meshVertexShaderCode, meshFragmentShaderCode, "") == false) exit(-1);
	if (depthShader.loadFromString(depthVertexShaderCode, depthFragmentShaderCode, depthGeometryShaderCode) == false) exit(-1);

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

    localAxis = new Axis(dynamicsWorld);
    gridlines = new Gridlines();
    lights.push_back(new Light(vec3(1.0), dynamicsWorld));
    lights[0]->setPosition(vec3(100.0));
    localAxis->hide();
	
	windowWidth = window.getWindowSizei().first;
	windowHeight = window.getWindowSizei().second;
	frameWidth = window.getFrameSizei().first;
	frameHeight = window.getFrameSizei().second;
	retina = (frameWidth > windowWidth);

    meshShader.use();
    for (int i = 0; i < MAX_LIGHTS; i++) {
        glActiveTexture(GL_TEXTURE8 + i);
        meshShader.setInt("depthMap[" + to_string(i) + "]", 8 + i);
    }

    // Initialize camera
    camera.pos = vec3(50.0f, 50.0f, 300.0f);

    // Add tweak bars
    TwInit(TW_OPENGL_CORE, NULL);
    TwDefine("GLOBAL fontsize=3");
    TwWindowSize(frameWidth, frameHeight);

    TwBar * windowInfoBar = TwNewBar("Window Info");
    TwSetParam(windowInfoBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.5");
    TwSetParam(windowInfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 10");
    TwSetParam(windowInfoBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 150");
    TwAddVarRO(windowInfoBar, "FPS", TW_TYPE_FLOAT, &fps, "step=0.1");
    TwAddVarRO(windowInfoBar, "Window Width", TW_TYPE_UINT32, &windowWidth, "");
    TwAddVarRO(windowInfoBar, "Window Height", TW_TYPE_UINT32, &windowHeight, "");
    TwAddVarRO(windowInfoBar, "Frame Width", TW_TYPE_UINT32, &frameWidth, "");
    TwAddVarRO(windowInfoBar, "Frame Height", TW_TYPE_UINT32, &frameHeight, "");
    TwAddVarRO(windowInfoBar, "Retina", TW_TYPE_BOOLCPP, &retina, "");

    // Show FPS and camara info
    TwBar * viewerInfoBar = TwNewBar("Viewer Info");
    TwSetParam(viewerInfoBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(viewerInfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 170");
    TwSetParam(viewerInfoBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 120");
    TwAddVarRW(viewerInfoBar, "Moving Speed", TW_TYPE_FLOAT, &moveSpeed, "step=0.01");
    TwAddVarRW(viewerInfoBar, "Camera Position X", TW_TYPE_FLOAT, &camera.pos.x, "step=0.1");
    TwAddVarRW(viewerInfoBar, "Camera Position Y", TW_TYPE_FLOAT, &camera.pos.y, "step=0.1");
    TwAddVarRW(viewerInfoBar, "Camera Position Z", TW_TYPE_FLOAT, &camera.pos.z, "step=0.1");

    // Show OpenGL config
    TwBar * configBar = TwNewBar("Configuration");
    TwSetParam(configBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(configBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 300");
    TwSetParam(configBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 230");
    TwAddVarRW(configBar, "Background Color", TW_TYPE_COLOR3F, &backgroundColor.x, "");
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
    TwSetParam(appInfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 540");
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
	double totTime = 0;
    for (uint32_t totframes = 0;; totframes++) {
        if (glfwWindowShouldClose(window.getGLFWwindow())) break;
		double startTime = glfwGetTime();
        processInput();
        if (enableShadow)
            for (uint32_t i = 0; i < lights.size(); i++)
                lights[i]->renderDepthMap(scenes, depthShader);
        render();
        glfwSwapBuffers(window.getGLFWwindow());
        glfwPollEvents();
        glCheckError();
        double endTime = glfwGetTime();
		singleFrameRenderTime = endTime - startTime;
		totTime += singleFrameRenderTime;
		if (totTime > 1.0) {
			fps = totframes / totTime;
			totTime = 0.0;
			totframes = 0;
		}
    }

    // Clean
    TwTerminate();
    glfwTerminate();
    return 0;
}

