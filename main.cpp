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

// Calculate FPS
float fps = 60, singleFrameRenderTime;

// Whether user can move around or not
bool canMove = 1;

// user's moving speed
float moveSpeed = 1.0f;

// Background color, can be adjusted during runtime
vec3 backgroundColor(0.7f);

// Window resolution
uint32_t windowWidth, windowHeight, frameWidth, frameHeight;
bool retina = 0;

// Callback when user modifies shadow resolution setting
void TW_CALL setShadowResolution(const void *value, void *client) {
	uint16_t *src = (uint16_t*) value, *des = (uint16_t*) client;

	// Max shadow resolution: 6144 * 6144
	*des = min(*src, (uint16_t) 6144);

	// Apply to all lights
	for (uint32_t i = 0; i < lights.size(); i++)
		lights[i]->setShadowResolution(*des);
}

// Callback when user modifies shadow resolution setting
void TW_CALL getShadowResolution(void *value, void *client) {
	uint16_t *des = (uint16_t*) value, *src = (uint16_t*) client;
	*des = *src;
}

// Callback when user stretches the window
void windowSizeCallback(GLFWwindow*, int32_t width, int32_t height) {
    window.resize(width, height);
	windowWidth = window.getWindowSizei().first;
	windowHeight = window.getWindowSizei().second;
	frameWidth = window.getFrameSizei().first;
	frameHeight = window.getFrameSizei().second;
	retina = (frameWidth > windowWidth);
}

// Callback when user clicks the mouse
void mouseButtonCallback(GLFWwindow*, int button, int action, int mods) {
	// Only respond handle left button actions
	if (button != GLFW_MOUSE_BUTTON_LEFT) return;

	// Update currnet cursor position
    cursor.update(window.getGLFWwindow());

	// If the click is handled by AntTweakBar, just ignore it
    TwMouseAction twAction = (action == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	if (TwMouseButton(twAction, TW_MOUSE_LEFT)) return;

    if (action == GLFW_PRESS) {
        lastTime = glfwGetTime();
        vec4 raySt, rayEd;
        // Screen space to world space
		screenPosToWorldRay(cursor.getCntPosfv(), window.getWindowSizefv(), window.getProjMatrix(), camera.getViewMatrix(), raySt, rayEd);
		
		// Perform ray test
        btCollisionWorld::AllHitsRayResultCallback allHits(btVector3(raySt.x, raySt.y, raySt.z), btVector3(rayEd.x, rayEd.y, rayEd.z));
        dynamicsWorld->rayTest(btVector3(raySt.x, raySt.y, raySt.z), btVector3(rayEd.x, rayEd.y, rayEd.z), allHits);

		// If user tries to drag one of the axes
		for (int32_t i = 0; i < allHits.m_collisionObjects.size(); i++)
			if (localAxis->startDrag((Mesh*) allHits.m_collisionObjects[i]->getUserPointer(), raySt, rayEd)) {
				lastTime = 0;
				return;
			}

		// Select the closest object
        float mindis = FLT_MAX;
        for (int32_t i = 0; i < allHits.m_collisionObjects.size(); i++)
            if (((Mesh*) allHits.m_collisionObjects[i]->getUserPointer())->name.substr(0, 11) != "MASTER_AXIS") {
                if (distance(camera.pos, toVec3(allHits.m_hitPointWorld[i])) > mindis) continue;
                if (selectedMesh != NULL) selectedMesh->deselect();
                mindis = distance(camera.pos, toVec3(allHits.m_hitPointWorld[i]));
                selectedMesh = (Mesh*) allHits.m_collisionObjects[i]->getUserPointer();
                selectedMesh->select();

				// Show local axis
                localAxis->setPosition(selectedMesh->getPosition());
                localAxis->show();
                lastTime = 0;
            }
    } else { // Release
		localAxis->stopDrag();

		// The time when the mouse is pressed down is stored in `lastTime`.
        // If the interval is less than 200ms, it is treated as a mouse
		// click. Otherwise it is treated as a mouse hold.
		if (glfwGetTime() - lastTime < 0.2f) {
            if (selectedMesh != NULL) selectedMesh->deselect();
            selectedMesh = NULL;
            localAxis->hide();
        }
    }
}

// Callback when user moves the cursor
void cursorPosCallback(GLFWwindow*, double xpos, double ypos) {
    TwMouseMotion((int32_t) xpos * window.getScaleRatio(), (int32_t) ypos * window.getScaleRatio());
}

// Callback when user presses a key
void keyCallback(GLFWwindow*, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
    if (action != GLFW_PRESS) return;

	// If the key is handled by AntTweakBar
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
			// F1: set transform mode to translation
			localAxis->setTransformMode(TRANSLATION);
			break;
		case GLFW_KEY_F2:
			// F2: set transform mode to rotation
			localAxis->setTransformMode(ROTATION);
			break;
		case GLFW_KEY_F3:
			// F3: set transform mode to scaling
			localAxis->setTransformMode(SCALING);
			break;
        case GLFW_KEY_F10:
			// F10: take a screenshot
            window.screenshot();
            break;
        case GLFW_KEY_C:
			// Ctrl + C: copy selected mesh
            if (mods == GLFW_MOD_CONTROL && selectedMesh != NULL)
                copyedMesh = selectedMesh;
            break;
        case GLFW_KEY_V:
			// Ctrl + V: paste copyed mesh
            if (mods == GLFW_MOD_CONTROL && copyedMesh != NULL) {
                if (copyedMesh->name == "MASTER_LIGHT") { // If the selected mesh is a light
                    if (lights.size() >= MAX_LIGHTS) {
                        reportWarning("You have reached the maximum number of lights, new lights will not be created.");
                        return;
                    }

					// Construct a new light
                    Light* newLight = new Light(*(Light*) copyedMesh);
                    lights.push_back(newLight);

					// Move to a different position
                    newLight->addTranslation(vec3(newLight->getSize().x * 1.2, 0, 0));
					// Select the new light
                    if (selectedMesh != NULL)
                        selectedMesh->deselect();
                    copyedMesh = selectedMesh = newLight;
                    selectedMesh->select();
                    localAxis->setPosition(newLight->getPosition());
                    reportInfo("Light " + to_string(lights.size() - 1) + " pasted");
                } else { // If the selected mesh is a general model
                    // Get the copyed mesh's parent
					Model* parentModel = ((Model*) copyedMesh->getParent());
                   
					// Contruct a new mesh
					Mesh *newMesh = new Mesh(*copyedMesh);

					// Move it to a different position
                    newMesh->addTranslation(vec3(newMesh->getSize().x * 1.2, 0, 0));
					// Add it to the same parent
                    parentModel->addMesh(newMesh);

					// Select the new mesh
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
			// Delete selected mesh
            if (selectedMesh != NULL && selectedMesh->name == "MASTER_LIGHT") { // If the selected mesh is a light
                for (uint32_t i = 0; i < lights.size(); i++)
                    if (lights[i] == selectedMesh) {
                        lights.erase(lights.begin() + i);
                    }
                delete (Light*) selectedMesh;
            } else if (selectedMesh != NULL) { // Other general models
                Model* parentModel = ((Model*) selectedMesh->getParent());
                parentModel->removeMesh(selectedMesh);
                delete selectedMesh;
            }
            if (selectedMesh == copyedMesh) copyedMesh = NULL;
            selectedMesh = NULL;
            localAxis->hide();
            break;
    }

	// Recycle unreferenced resources
    for (uint32_t i = 0; i < scenes.size(); i++)
        scenes[i]->recycle();
}

// Callback when user inputs a char
void charCallback(GLFWwindow*, uint32_t key) {
    canMove = !TwKeyPressed(key, TW_KMOD_NONE);
}

// Callback when user drops some files onto the window
void dropCallBack(GLFWwindow*, int count, const char** paths) {
    // Treat those files as scenes and load them
	for (int i = 0; i < count; i++) try {
        Scene *newScene = new Scene(paths[i], dynamicsWorld);
        newScene->dumpinfo("");
        scenes.push_back(newScene);
    } catch (const string msg) {
        cerr << msg << endl;
    }
}

void processInput() {
    // Move using keyboard
    if (canMove) {
        float shift = 60.0f * singleFrameRenderTime;

		// Press SHIFT to get a 5x speed up
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) shift *= 5.0f;
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) shift *= 5.0f;
        
		// Press W, S, A, D, Q, E to move around
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
	
	// If the user is not dragging the axis
	if (localAxis->continueDrag(selectedMesh, raySt, rayEd) == false && glfwGetMouseButton(window.getGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        camera.turnLeft(-cursor.getDeltafv().x / 500.0f);
        camera.lookUp(-cursor.getDeltafv().y / 500.0f);
    }
}

// Render a frame
void render() {
    glViewport(0, 0, window.getFrameSizei().first, window.getFrameSizei().second);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);

    // OpenGL configurations
	glfwSwapInterval(enableVSync);
    if (enableFaceCulling) glEnable(GL_CULL_FACE);
	if (enableWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (enableMSAA4X)
		glEnable(GL_MULTISAMPLE);
	else
		glDisable(GL_MULTISAMPLE);

    // Get projection and view matrix
    mat4 projection = window.getProjMatrix();
    mat4 view = camera.getViewMatrix();

    // Set shader for meshes
    meshShader.use();
    meshShader.setMat4("projection", projection);
    meshShader.setMat4("view", view);
    meshShader.setVec3("viewPos", camera.pos);
	meshShader.setInt("enableDoubleSideLighting", enableDoubleSideLighting);
	meshShader.setFloat("bias", bias);
	meshShader.setFloat("radius", radius);

	// Render lights
    if (enableLighting && !enableWireFrame) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        meshShader.setInt("lightNum", (int32_t) lights.size());
        meshShader.setInt("enableLighting", 0);
		meshShader.setInt("enableAttenuation", enableAttenuation);
		meshShader.setFloat("attenuation_quadratic", attenuation_quadratic);
		meshShader.setFloat("attenuation_linear", attenuation_linear);
		meshShader.setFloat("attenuation_constant", attenuation_constant);
        for (uint32_t i = 0; i < lights.size(); i++) {
            string idx = "lights[" + to_string(i) + "]";
            
			// The depthmap of each point light is a cubemap
			glActiveTexture(GL_TEXTURE8 + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i]->getDepthMap());
            meshShader.setBool(idx + ".hasDepthMap", enableShadow);
            meshShader.setInt("depthMap[" + to_string(i) + "]", 8 + i);
            meshShader.setFloat(idx + ".farPlane", lights[i]->getFarPlane());
            meshShader.setVec3(idx + ".pos", lights[i]->getPosition());
            meshShader.setVec3(idx + ".color", lights[i]->getColor());
			lights[i]->show();
            lights[i]->render(meshShader);
		}
		meshShader.setInt("enableLighting", 1);
	} else {
		meshShader.setInt("enableLighting", 0);
		for (uint32_t i = 0; i < lights.size(); i++)
			lights[i]->hide();
	}

	// Enable gamma correction?
	if (enableGammaCorrection) glEnable(GL_FRAMEBUFFER_SRGB);
    
	// Render scenes
    for (uint32_t i = 0; i < scenes.size(); i++)
        scenes[i]->render(meshShader);

	// Disable some options when rendering axes, gridlines, and UI
	glDisable(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    meshShader.setInt("enableLighting", 0);

    // Render axis and gridlines
    if (enableGridlines) gridlines->render(meshShader);
    localAxis->render(meshShader, camera.pos);

    // Draw UI
    TwDraw();
}

int main(int argc, char **argv) {
	// Some issues on macOS
#ifdef APPLE_MACOS
    CGEventSourceRef evsrc = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    CGEventSourceSetLocalEventsSuppressionInterval(evsrc, 0.0);
#endif

    // Initialize GLEW library
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        reportError("Failed to initialize GLAD library");
        exit(-1);
    }

    // Get hardware and driver info
    string rendererInfo = "GPU: " + (string) (char*) glGetString(GL_RENDERER);
    reportInfo(rendererInfo);
    string glVersionInfo = "OpenGL Version: " + (string) (char*) glGetString(GL_VERSION);
    reportInfo(glVersionInfo);
    string glShadingLanguageVersionInfo = "Shading Language Version: " + (string) (char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
    reportInfo(glShadingLanguageVersionInfo);

    // Load shaders
    if (meshShader.loadFromString(meshVertexShaderCode, meshFragmentShaderCode, "") == false) exit(-1);
	if (depthShader.loadFromString(depthVertexShaderCode, depthFragmentShaderCode, depthGeometryShaderCode) == false) exit(-1);

    // Initialize bullet physics engine
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10.0f, 0));

    // Load scenes given by arguments
    for (int i = 1; i < argc; i++) try {
        Scene *newScene = new Scene(argv[i], dynamicsWorld);
        newScene->dumpinfo("");
        scenes.push_back(newScene);
    } catch (const string msg) {
        cerr << msg << endl;
    }

	// Initialize local axes
    localAxis = new Axis(dynamicsWorld);
    localAxis->hide();

	// Initialize gridlines
    gridlines = new Gridlines();

	// Initialize lights
    lights.push_back(new Light(vec3(1.0), shadowResolution, dynamicsWorld));
    lights[0]->setPosition(vec3(100.0));
	
	// Get window size
	windowWidth = window.getWindowSizei().first;
	windowHeight = window.getWindowSizei().second;
	frameWidth = window.getFrameSizei().first;
	frameHeight = window.getFrameSizei().second;
	retina = (frameWidth > windowWidth);

	// Initialize depthmap indices in shader
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

	// Show window info
    TwBar * windowInfoBar = TwNewBar("Window Info");
    TwSetParam(windowInfoBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.5");
    TwSetParam(windowInfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 10");
    TwSetParam(windowInfoBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 145");
    TwAddVarRO(windowInfoBar, "FPS", TW_TYPE_FLOAT, &fps, "step=0.1");
    TwAddVarRO(windowInfoBar, "Window Width", TW_TYPE_UINT32, &windowWidth, "");
    TwAddVarRO(windowInfoBar, "Window Height", TW_TYPE_UINT32, &windowHeight, "");
    TwAddVarRO(windowInfoBar, "Frame Width", TW_TYPE_UINT32, &frameWidth, "");
    TwAddVarRO(windowInfoBar, "Frame Height", TW_TYPE_UINT32, &frameHeight, "");
    TwAddVarRO(windowInfoBar, "Retina", TW_TYPE_BOOLCPP, &retina, "");

    // Show FPS and camara info
    TwBar * viewerInfoBar = TwNewBar("Viewer Info");
    TwSetParam(viewerInfoBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(viewerInfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 165");
    TwSetParam(viewerInfoBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 115");
    TwAddVarRW(viewerInfoBar, "Moving Speed", TW_TYPE_FLOAT, &moveSpeed, "step=0.01");
    TwAddVarRW(viewerInfoBar, "Camera Position X", TW_TYPE_FLOAT, &camera.pos.x, "step=0.1");
    TwAddVarRW(viewerInfoBar, "Camera Position Y", TW_TYPE_FLOAT, &camera.pos.y, "step=0.1");
    TwAddVarRW(viewerInfoBar, "Camera Position Z", TW_TYPE_FLOAT, &camera.pos.z, "step=0.1");

	// Show texture map config
    TwBar * textureBar = TwNewBar("Texture");
    TwSetParam(textureBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(textureBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 290");
    TwSetParam(textureBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 115");
	TwAddVarRW(textureBar, "Diffuse Map", TW_TYPE_BOOLCPP, &enableDiffuseMap, "");
    TwAddVarRW(textureBar, "Specular Map", TW_TYPE_BOOLCPP, &enableSpecularMap, "");
    TwAddVarRW(textureBar, "Normal Map", TW_TYPE_BOOLCPP, &enableNormalMap, "");
    TwAddVarRW(textureBar, "Parallax Map", TW_TYPE_BOOLCPP, &enableParallaxMap, "");

    // Show lighting config
	TwBar * lightingBar = TwNewBar("Lighting");
    TwSetParam(lightingBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(lightingBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 415");
    TwSetParam(lightingBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 155");
    TwAddVarRW(lightingBar, "Enable Lighting", TW_TYPE_BOOLCPP, &enableLighting, "");
    TwAddVarRW(lightingBar, "Double Side Lighting", TW_TYPE_BOOLCPP, &enableDoubleSideLighting, "");
    TwAddVarRW(lightingBar, "Enable Attenuation", TW_TYPE_BOOLCPP, &enableAttenuation, "");
	TwAddVarRW(lightingBar, "Attenuation Quadratic", TW_TYPE_FLOAT, &attenuation_quadratic, "step=0.000001");
	TwAddVarRW(lightingBar, "Attenuation Linear", TW_TYPE_FLOAT, &attenuation_linear, "step=0.000001");
    TwAddVarRW(lightingBar, "Attenuation Constant", TW_TYPE_FLOAT, &attenuation_constant, "step=0.000001");

    // Show shadow config
	TwBar * shadowBar = TwNewBar("Shadow");
    TwSetParam(shadowBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(shadowBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 580");
    TwSetParam(shadowBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 115");
    TwAddVarRW(shadowBar, "Enable Shadow", TW_TYPE_BOOLCPP, &enableShadow, "");
    TwAddVarCB(shadowBar, "Resolution", TW_TYPE_UINT16, setShadowResolution, getShadowResolution, &shadowResolution, NULL);
    TwAddVarRW(shadowBar, "Bias", TW_TYPE_FLOAT, &bias, "step=0.001");
    TwAddVarRW(shadowBar, "Radius", TW_TYPE_FLOAT, &radius, "step=0.001");

    // Show OpenGL config
    TwBar * configBar = TwNewBar("Configuration");
    TwSetParam(configBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(configBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 705");
    TwSetParam(configBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 165");
    TwAddVarRW(configBar, "Background Color", TW_TYPE_COLOR3F, &backgroundColor.x, "");
    TwAddVarRW(configBar, "Backface Culling", TW_TYPE_BOOLCPP, &enableFaceCulling, "");
    TwAddVarRW(configBar, "Gridlines", TW_TYPE_BOOLCPP, &enableGridlines, "");
    TwAddVarRW(configBar, "Vertical Sync", TW_TYPE_BOOLCPP, &enableVSync, "");
    TwAddVarRW(configBar, "MSAA 4X", TW_TYPE_BOOLCPP, &enableMSAA4X, "");
    TwAddVarRW(configBar, "Gamma Correction", TW_TYPE_BOOLCPP, &enableGammaCorrection, "");
    TwAddVarRW(configBar, "WireFrame Mode", TW_TYPE_BOOLCPP, &enableWireFrame, "");

    // Show application info
    TwBar * appInfoBar = TwNewBar("Application Info");
    TwSetParam(appInfoBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(appInfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 880");
    TwSetParam(appInfoBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 120");
    TwAddButton(appInfoBar, "1.0", NULL, NULL, "label='App Version: v0.4.2'");
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

    // Set camera viewport
    glViewport(0, 0, window.getFrameSizei().first, window.getFrameSizei().second);

	// Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Start loop
	double totTime = 0;
    for (uint32_t totframes = 0;; totframes++) {
        if (glfwWindowShouldClose(window.getGLFWwindow())) break;
		double startTime = glfwGetTime();
        processInput();
    
		// Render all depthmaps first in each frame
		if (enableShadow)
            for (uint32_t i = 0; i < lights.size(); i++)
                lights[i]->renderDepthMap(scenes, depthShader);
        
		// Render the scene
		render();

		// Swap buffers, poll events, and check errors
        glfwSwapBuffers(window.getGLFWwindow());
        glfwPollEvents();
        glCheckError();

		// Calculate FPS
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
