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

// Create a 960 * 640 window titled 'masterEngine'
Window window(960, 640, "masterEngine");

// Deal with cursor movement
Cursor cursor;

// Perspective camera
Camera camera;

// Axis for translation, rotation and scaling
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

// The last time when user presses mouse button
double lastTime;

// Calculate FPS
float fps = 60, singleFrameRenderTime;

// If a key press or mouse press is handled by AntTweakBar,
// we should not handle it again.
bool ignoreKeyboard = false;
bool ignoreMouseMotion = false;

// Moving speed
float moveSpeed = 1.0f;

// Background color, can be adjusted during runtime
vec3 backgroundColor(0.7f);

// Window resolution
uint32_t windowWidth, windowHeight, frameWidth, frameHeight;
bool retina = 0;

// Add a light to the scene
void TW_CALL addLight(void *clientData) {
    if (lights.size() >= MAX_LIGHTS) {
        reportWarning("You have reached the maximum number of lights, new lights will not be created.");
        return;
    }
    Light *newLight = new Light(vec3(1.0), shadowResolution, dynamicsWorld);
    newLight->setPosition(camera.pos + camera.dir * 200);
    lights.push_back(newLight);
    newLight->select();
    selectedMesh = newLight;
    
    // Locate the local axis to new light
    localAxis->setPosition(newLight->getPosition());
    localAxis->show();
}

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

	// Retina support for MacBook, might not work on Windows.
    retina = (frameWidth > windowWidth);
}

// Callback when user presses the mouse button
void mouseButtonCallback(GLFWwindow*, int button, int action, int mods) {
	// Only handle left button
	if (button != GLFW_MOUSE_BUTTON_LEFT) return;

	// Update currnet cursor position
    cursor.update(window.getGLFWwindow());

	// If the click is handled by AntTweakBar, ignore it
    TwMouseAction twAction = (action == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
    if (TwMouseButton(twAction, TW_MOUSE_LEFT)) {
        ignoreMouseMotion = (action == GLFW_PRESS);
        return;
    }

    // Mouse picking
    if (action == GLFW_PRESS) {
        // Record the time when user press mouse button
        lastTime = glfwGetTime();
        
        // Convert mouse position in 2D screen space to a ray in 3D world space
        vec4 raySt, rayEd;
		screenPosToWorldRay(cursor.getCntPosfv(), window.getWindowSizefv(), window.getProjMatrix(), camera.getViewMatrix(), raySt, rayEd);
		
		// Perform ray test using bullet
        btCollisionWorld::AllHitsRayResultCallback allHits(btVector3(raySt.x, raySt.y, raySt.z), btVector3(rayEd.x, rayEd.y, rayEd.z));
        dynamicsWorld->rayTest(btVector3(raySt.x, raySt.y, raySt.z), btVector3(rayEd.x, rayEd.y, rayEd.z), allHits);

		// If user tries to drag any axis
		for (int32_t i = 0; i < allHits.m_collisionObjects.size(); i++)
			if (localAxis->startDrag((Mesh*) allHits.m_collisionObjects[i]->getUserPointer(), raySt, rayEd)) {
				lastTime = 0;
				return;
			}

		// Select the closest object
        float mindis = FLT_MAX;
        for (int32_t i = 0; i < allHits.m_collisionObjects.size(); i++)
            if (((Mesh*) allHits.m_collisionObjects[i]->getUserPointer())->getType() != AXIS) {
                // Calculate the distance between user and object and get the closest one
                if (distance(camera.pos, toVec3(allHits.m_hitPointWorld[i])) > mindis) continue;
                
                // If user selected a mesh before, deselect it
                if (selectedMesh != NULL) selectedMesh->deselect();
                
                // Update mindis
                mindis = distance(camera.pos, toVec3(allHits.m_hitPointWorld[i]));
                
                // Select it
                selectedMesh = (Mesh*) allHits.m_collisionObjects[i]->getUserPointer();
                selectedMesh->select();

				// Locate local axis to the selected mesh
                localAxis->setPosition(selectedMesh->getPosition());
                localAxis->show();

                lastTime = 0;
            }
    } else { // Mouse button released
		localAxis->stopDrag();

		// The time when the mouse button is pressed is stored in `lastTime`.
        // If the interval is less than 200ms, it is treated as a mouse click.
		// Otherwise it is treated as a mouse hold.
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
        case GLFW_KEY_L:
            // L: Add a new light
            addLight(NULL);
            break;
        case GLFW_KEY_C:
			// Ctrl + C: copy selected mesh
            if (mods == GLFW_MOD_CONTROL && selectedMesh && selectedMesh->getType() == DEFAULT) {
                copyedMesh = selectedMesh;
                reportInfo(selectedMesh->getName() + " copyed.");
            }
            break;
        case GLFW_KEY_V:
			// Ctrl + V: paste copyed mesh
            if (mods == GLFW_MOD_CONTROL && copyedMesh != NULL) {
                // Get the copyed mesh's parent
                Model* parentModel = ((Model*) copyedMesh->getParent());
               
                // Contruct a new mesh
                Mesh *newMesh = new Mesh(*copyedMesh);

                // Move it to a different position
                newMesh->addTranslation(vec3(newMesh->getSize().x * 1.2, 0, 0));
                
                // Add it to the same parent
                parentModel->addMesh(newMesh);

                // Select the new mesh
                if (selectedMesh != NULL) selectedMesh->deselect();
                copyedMesh = selectedMesh = newMesh;
                selectedMesh->select();
                
                // Locate the local axis to new mesh
                localAxis->setPosition(newMesh->getPosition());
                localAxis->show();

                reportInfo(selectedMesh->getName() + " pasted");
            }
            break;
        case GLFW_KEY_BACKSPACE: // Delete selected mesh
            if (selectedMesh == NULL) break;
            if (selectedMesh->getType() == LIGHT) {
                // If the selected mesh is a light
                for (uint32_t i = 0; i < lights.size(); i++)
                    if (lights[i] == selectedMesh)
                        lights.erase(lights.begin() + i);
                delete (Light*) selectedMesh;
            } else if (selectedMesh->getType() == DEFAULT) { // Others
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
    for (uint32_t i = 0; i < scenes.size(); i++) {
        scenes[i]->recycle();
        if (scenes[i]->getMeshNum() == 0) {
            delete scenes[i];
            scenes.erase(scenes.begin() + i);
            i--;
        }
    }
}

// Callback when user presses a key
void charCallback(GLFWwindow*, uint32_t key) {
    ignoreKeyboard = TwKeyPressed(key, TW_KMOD_NONE);
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
    // Update cursor position
    cursor.update(window.getGLFWwindow());
   
    if (!ignoreKeyboard) {
        // Press SHIFT to get a 5x speed up
        float shift = 60.0f * singleFrameRenderTime;
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
                || glfwGetKey(window.getGLFWwindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
            shift *= 5.0f;
        
        // Press W, S, A, D, Q, E to move around
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS)
            camera.moveForward(shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS)
            camera.moveForward(-shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS)
            camera.moveRight(-shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS)
            camera.moveRight(shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_Q) == GLFW_PRESS)
            camera.moveUp(-shift * moveSpeed);
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_E) == GLFW_PRESS)
            camera.moveUp(shift * moveSpeed);
    }

    if (glfwGetMouseButton(window.getGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS
            || ignoreMouseMotion) return;

    // Convert mouse position in 2D screen space to a ray in 3D world space
    vec4 raySt, rayEd;
    screenPosToWorldRay(cursor.getCntPosfv(), window.getWindowSizefv(), window.getProjMatrix(), camera.getViewMatrix(), raySt, rayEd);
	
    // If the user is not dragging any axis, change camera viewport by mouse motion
	if (localAxis->continueDrag(selectedMesh, raySt, rayEd) == false) { 
        camera.turnLeft(-cursor.getDeltafv().x / 500.0f);
        camera.lookUp(-cursor.getDeltafv().y / 500.0f);
    }
}

// Render a frame
void render() {
    glViewport(0, 0, window.getFrameSizei().first, window.getFrameSizei().second);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set background color
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

    // Configure shader for meshes
    meshShader.use();
    meshShader.setMat4("projection", projection);
    meshShader.setMat4("view", view);
    meshShader.setVec3("viewPos", camera.pos);
	meshShader.setInt("enableDoubleSideLighting", enableDoubleSideLighting);
	meshShader.setFloat("bias", bias);
	meshShader.setFloat("radius", radius);

    // Enable lighting?
    if (enableLighting && !enableWireFrame) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Configure shader for lights
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

            // Render lights
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

	// Disable some options when rendering axes, gridlines, and AntTweakBar
	glDisable(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    meshShader.setInt("enableLighting", 0);

    // Render axis and gridlines
    if (enableGridlines) gridlines->render(meshShader);
    localAxis->render(meshShader, camera.pos);

    // Render AntTweakBar
    TwDraw();
}

int main(int argc, char **argv) {
	// Some issues on macOS
#ifdef APPLE_MACOS
    CGEventSourceRef evsrc = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    CGEventSourceSetLocalEventsSuppressionInterval(evsrc, 0.0);
#endif

	// Get window size
	windowWidth = window.getWindowSizei().first;
	windowHeight = window.getWindowSizei().second;
	frameWidth = window.getFrameSizei().first;
	frameHeight = window.getFrameSizei().second;
	retina = (frameWidth > windowWidth);
    if (!retina) window.resize(1600, 900);

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
    TwWindowSize(frameWidth, frameHeight);

	// Show window info
    TwBar * configBar = TwNewBar("Configuration");
    TwSetParam(configBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwSetParam(configBar, NULL, "size", TW_PARAM_CSTRING, 1, "270 680");
    TwAddVarRO(configBar, "FPS", TW_TYPE_FLOAT, &fps, "step=0.1");
    TwAddVarRO(configBar, "Window Width", TW_TYPE_UINT32, &windowWidth, "");
    TwAddVarRO(configBar, "Window Height", TW_TYPE_UINT32, &windowHeight, "");
    TwAddVarRO(configBar, "Frame Width", TW_TYPE_UINT32, &frameWidth, "");
    TwAddVarRO(configBar, "Frame Height", TW_TYPE_UINT32, &frameHeight, "");
    TwAddVarRO(configBar, "Retina", TW_TYPE_BOOLCPP, &retina, "");

    // Show FPS and camara info
    TwAddSeparator(configBar, "sep1", NULL);
    TwAddVarRW(configBar, "Moving Speed", TW_TYPE_FLOAT, &moveSpeed, "step=0.01");
    TwAddVarRW(configBar, "Camera Position X", TW_TYPE_FLOAT, &camera.pos.x, "step=0.1");
    TwAddVarRW(configBar, "Camera Position Y", TW_TYPE_FLOAT, &camera.pos.y, "step=0.1");
    TwAddVarRW(configBar, "Camera Position Z", TW_TYPE_FLOAT, &camera.pos.z, "step=0.1");

	// Show texture map config
    TwAddSeparator(configBar, "sep2", NULL);
	TwAddVarRW(configBar, "Diffuse Map", TW_TYPE_BOOLCPP, &enableDiffuseMap, "");
    TwAddVarRW(configBar, "Specular Map", TW_TYPE_BOOLCPP, &enableSpecularMap, "");
    TwAddVarRW(configBar, "Normal Map", TW_TYPE_BOOLCPP, &enableNormalMap, "");
    TwAddVarRW(configBar, "Parallax Map", TW_TYPE_BOOLCPP, &enableParallaxMap, "");

    // Show lighting config
    TwAddSeparator(configBar, "sep3", NULL);
    TwAddButton(configBar, "Add New Light", addLight, NULL, "");
    TwAddVarRW(configBar, "Enable Lighting", TW_TYPE_BOOLCPP, &enableLighting, "");
    TwAddVarRW(configBar, "Double Side Lighting", TW_TYPE_BOOLCPP, &enableDoubleSideLighting, "");
    TwAddVarRW(configBar, "Enable Attenuation", TW_TYPE_BOOLCPP, &enableAttenuation, "");
	TwAddVarRW(configBar, "Attenuation Quadratic", TW_TYPE_FLOAT, &attenuation_quadratic, "step=0.000001");
	TwAddVarRW(configBar, "Attenuation Linear", TW_TYPE_FLOAT, &attenuation_linear, "step=0.000001");
    TwAddVarRW(configBar, "Attenuation Constant", TW_TYPE_FLOAT, &attenuation_constant, "step=0.000001");

    // Show shadow config
    TwAddSeparator(configBar, "sep4", NULL);
    TwAddVarRW(configBar, "Enable Shadow", TW_TYPE_BOOLCPP, &enableShadow, "");
    TwAddVarCB(configBar, "Resolution", TW_TYPE_UINT16, setShadowResolution, getShadowResolution, &shadowResolution, NULL);
    TwAddVarRW(configBar, "Bias", TW_TYPE_FLOAT, &bias, "step=0.001");
    TwAddVarRW(configBar, "Radius", TW_TYPE_FLOAT, &radius, "step=0.001");

    // Show OpenGL config
    TwAddSeparator(configBar, "sep5", NULL);
    TwAddVarRW(configBar, "Background Color", TW_TYPE_COLOR3F, &backgroundColor.x, "");
    TwAddVarRW(configBar, "Backface Culling", TW_TYPE_BOOLCPP, &enableFaceCulling, "");
    TwAddVarRW(configBar, "Gridlines", TW_TYPE_BOOLCPP, &enableGridlines, "");
    TwAddVarRW(configBar, "Vertical Sync", TW_TYPE_BOOLCPP, &enableVSync, "");
    TwAddVarRW(configBar, "MSAA 4X", TW_TYPE_BOOLCPP, &enableMSAA4X, "");
    TwAddVarRW(configBar, "Gamma Correction", TW_TYPE_BOOLCPP, &enableGammaCorrection, "");
    TwAddVarRW(configBar, "WireFrame Mode", TW_TYPE_BOOLCPP, &enableWireFrame, "");

    // Show application info
    TwAddSeparator(configBar, "sep6", NULL);
    TwAddButton(configBar, "1.0", NULL, NULL, "label='App Version: v0.5.0'");
    TwAddButton(configBar, "1.1", NULL, NULL, ("label='" + rendererInfo + "'").c_str());
    TwAddButton(configBar, "1.2", NULL, NULL, ("label='" + glVersionInfo + "'").c_str());
    TwAddButton(configBar, "1.3", NULL, NULL, ("label='" + glShadingLanguageVersionInfo + "'").c_str());
   
    TwDefine(retina ? "GLOBAL fontsize=3" : "GLOBAL fontsize=2");

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
		if (enableLighting && enableShadow && !enableWireFrame)
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
