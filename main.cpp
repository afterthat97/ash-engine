#include "utilities.h"
#include "scene.h"
#include "camera.h"
#include "light.h"
#include "glconfig.h"
#include "shader.h"
#include "axis.h"
#include "gridlines.h"

const string meshVertexShaderCode=R"(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 objPos;
out vec3 objNormalRaw;
out vec2 objTexCoord;
out mat3 TBN;

void main() {
	objPos = pos;
	objNormalRaw = normal;
	objTexCoord = texCoord;

	vec3 T = normalize(tangent);
	vec3 N = normalize(normal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(T, N));
	TBN = mat3(T, B, N);

	gl_Position = projection * view * model * vec4(pos, 1.0f);
}
)";

const string meshFragmentShaderCode=R"(
#version 330 core

struct Material {
	int hasAmbientMap;
	int hasDiffuseMap;
	int hasSpecularMap;
	int hasNormalMap;
	sampler2D ambientMap;
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
    vec3 ambientRGB;
    vec3 diffuseRGB;
    vec3 specularRGB;
    float shininess;
};

struct Light {
	int enable;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 color;

in vec3 objPos;
in vec3 objNormalRaw;
in vec2 objTexCoord;
in mat3 TBN;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
	vec3 ambient, diffuse, specular, objNormal = objNormalRaw;

	if (material.hasNormalMap != 0) {
		objNormal = texture(material.normalMap, objTexCoord).rgb;
		objNormal = normalize(objNormal * 2.0 - 1.0);
		objNormal = normalize(TBN * objNormal);
	}

	// ambient
	if (material.hasDiffuseMap == 0)
		ambient = light.ambient * material.ambientRGB;
	else
		ambient = light.ambient * vec3(texture(material.diffuseMap, objTexCoord));

    // diffuse
    vec3 lightDir = normalize(light.position - objPos);
    float diff = max(dot(objNormal, lightDir), 0.0);
	if (material.hasDiffuseMap == 0)
		diffuse = diff * light.diffuse * material.diffuseRGB;
	else
	    diffuse = diff * light.diffuse * vec3(texture(material.diffuseMap, objTexCoord));

    // specular
    vec3 viewDir = normalize(viewPos - objPos);
    vec3 reflectDir = reflect(-lightDir, objNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	if (material.hasSpecularMap == 0)
	    specular = spec * light.specular * material.specularRGB;
	else
		specular = spec * light.specular * vec3(texture(material.specularMap, objTexCoord));

    vec3 result = ambient + diffuse + specular;

	if (light.enable == 1)
		color = vec4(result, 1.0);
	else if (material.hasDiffuseMap == 1)
		color = texture(material.diffuseMap, objTexCoord);
	else
		color = vec4(material.diffuseRGB, 1.0);
}
)";

const string pureColorVertexShaderCode = R"(
#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0f);
}
)";

const string pureColorFragmentShaderCode = R"(
#version 330 core

out vec4 color;

uniform vec4 objColor;

void main() {
    color = objColor;
}
)";

pair<double, double> lastPointerPos;
pair<int32_t, int32_t> windowSize = { 1024, 576 }, windowFrameBufferSize;
int32_t scaleRatio, fps = 60, canMove = 1, canChangeViewport = 0;
Camera camera;
Light light0;
Axis globalAxis;
Gridlines gridlines;
vector<Scene> scenes;
Shader meshShader, pureColorShader;
uint32_t image_id = 0;
float moveSpeed = 1.0f;

void windowSizeCallback(GLFWwindow* window, int32_t width, int32_t height) {
	windowSize = { width, height };
	glfwGetFramebufferSize(window, &windowFrameBufferSize.first, &windowFrameBufferSize.second);
	scaleRatio = windowFrameBufferSize.first / windowSize.first;
	TwWindowSize(windowFrameBufferSize.first, windowFrameBufferSize.second);
	glViewport(0, 0, windowFrameBufferSize.first, windowFrameBufferSize.second);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	int handled = 0;
	TwMouseAction twAction = (action == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		handled = TwMouseButton(twAction, TW_MOUSE_LEFT);
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		handled = TwMouseButton(twAction, TW_MOUSE_RIGHT);
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		handled = TwMouseButton(twAction, TW_MOUSE_MIDDLE);
	if (handled) return;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &lastPointerPos.first, &lastPointerPos.second);
#ifdef __APPLE__
		lastPointerPos.first = floor(lastPointerPos.first);
		lastPointerPos.second = floor(lastPointerPos.second);
		CGDisplayHideCursor(kCGDirectMainDisplay);
#else
		ShowCursor(false);
#endif
		canChangeViewport = 1;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		glfwSetCursorPos(window, lastPointerPos.first, lastPointerPos.second);
#ifdef __APPLE__
		CGDisplayShowCursor(kCGDirectMainDisplay);
#else
		ShowCursor(true);
#endif
		canChangeViewport = 0;
	}
}

void cursorPosCallback(GLFWwindow * window, double xpos, double ypos) {
	TwMouseMotion((int32_t) xpos * scaleRatio, (int32_t) ypos * scaleRatio);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffse) {
	camera.moveForward((float) yoffse * 10.0f);
}

void keyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	if (action != GLFW_PRESS) return;
	int k = 0;
	switch (key) {
		case GLFW_KEY_F10: {
			// Capture screen pixels and save screenshot
			uint8_t* pixels = new uint8_t[3 * windowFrameBufferSize.first * windowFrameBufferSize.second];
			glReadPixels(
				0,
				0,
				windowFrameBufferSize.first,
				windowFrameBufferSize.second,
				GL_BGR,
				GL_UNSIGNED_BYTE,
				pixels);
			FreeImage_Initialise(0);
			string filename = "Screenshot" + to_string(image_id++) + ".bmp";
			FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(
				pixels,
				windowFrameBufferSize.first,
				windowFrameBufferSize.second,
				3 * windowFrameBufferSize.first,
				24,
				0x0000FF,
				0xFF0000,
				0x00FF00,
				0);
			FreeImage_Save(FIF_BMP, bitmap, filename.c_str(), 0);
			FreeImage_Unload(bitmap);
			FreeImage_DeInitialise();
			delete[] pixels; break; }
		case GLFW_KEY_ESCAPE:
			k = TW_KEY_ESCAPE; break;
		case GLFW_KEY_UP:
			k = TW_KEY_UP; break;
		case GLFW_KEY_DOWN:
			k = TW_KEY_DOWN; break;
		case GLFW_KEY_LEFT:
			k = TW_KEY_LEFT; break;
		case GLFW_KEY_RIGHT:
			k = TW_KEY_RIGHT; break;
		case GLFW_KEY_TAB:
			k = TW_KEY_TAB; break;
		case GLFW_KEY_ENTER:
			k = TW_KEY_RETURN; break;
		case GLFW_KEY_BACKSPACE:
			k = TW_KEY_BACKSPACE; break;
		case GLFW_KEY_INSERT:
			k = TW_KEY_INSERT; break;
		case GLFW_KEY_DELETE:
			k = TW_KEY_DELETE; break;
		case GLFW_KEY_PAGE_UP:
			k = TW_KEY_PAGE_UP; break;
		case GLFW_KEY_PAGE_DOWN:
			k = TW_KEY_PAGE_DOWN; break;
		case GLFW_KEY_HOME:
			k = TW_KEY_HOME; break;
		case GLFW_KEY_END:
			k = TW_KEY_END; break;
		case GLFW_KEY_KP_ENTER:
			k = TW_KEY_RETURN; break;
	}
	if (k > 0) TwKeyPressed(k, TW_KMOD_NONE);
}

void charCallback(GLFWwindow* window, uint32_t key) {
	canMove = !TwKeyPressed(key, TW_KMOD_NONE);
}

void render(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	
	// Move camera by keyboard
	if (canMove) {
		float shift = 1.0f;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) shift = 5.0f;
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) shift = 5.0f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.moveForward(shift * moveSpeed);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.moveForward(-shift * moveSpeed);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.moveRight(-shift * moveSpeed);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.moveRight(shift * moveSpeed);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.moveUp(-shift * moveSpeed);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.moveUp(shift * moveSpeed);
	}

	// Change camera viewport by mouse motion
	if (canChangeViewport) {
		double dx, dy;
		glfwGetCursorPos(window, &dx, &dy);
		glfwSetCursorPos(window, lastPointerPos.first, lastPointerPos.second);
		dx -= lastPointerPos.first; dy -= lastPointerPos.second;
		camera.turnLeft((GLfloat) -dx / 500);
		camera.lookUp((GLfloat) -dy / 500);
	}

	// Apply camera viewport
	mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowSize.first / (float)windowSize.second, 0.001f, 100000.0f);
	mat4 view = glm::lookAt(camera.pos, camera.pos + camera.dir, camera.up);

	// Set shader for meshes (models)
	meshShader.use();
	meshShader.setMat4("projection", projection);
	meshShader.setMat4("view", view);
	meshShader.setMat4("model", mat4(1.0f));
	meshShader.setVec3("viewPos", camera.pos);
	meshShader.setVec3("light.position", light0.pos);
	meshShader.setVec3("light.ambient", light0.ambient);
	meshShader.setVec3("light.diffuse", light0.diffuse);
	meshShader.setVec3("light.specular", light0.specular);
	meshShader.setInt("light.enable", enableLight);

	// Set shader for light
	pureColorShader.use();
	pureColorShader.setMat4("projection", projection);
	pureColorShader.setMat4("view", view);

	// OpenGL configurations
	glShadeModel(shadeModelStr == "FLAT" ? GL_FLAT : GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, (polygonModeStr == "LINE" ? GL_LINE : GL_FILL));
	if (enableCullFace) glEnable(GL_CULL_FACE);
	if (enableDepthTest) glEnable(GL_DEPTH_TEST);
	if (enableMultiSample) glEnable(GL_MULTISAMPLE);
	if (enableGridlines) gridlines.render(pureColorShader);
	if (enableGlobalAxis) globalAxis.render(pureColorShader);

	// Render scenes
	for (uint32_t i = 0; i < scenes.size(); i++)
		scenes[i].render(meshShader);

	// Render light
	if (enableLight) light0.render(pureColorShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_MULTISAMPLE);

	// Draw tweak bars
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	TwDraw();
}

int main(int argc, char **argv) {
#ifdef __APPLE__
	CGEventSourceRef evsrc = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
	CGEventSourceSetLocalEventsSuppressionInterval(evsrc, 0.0);
#endif
	// Initialize GLFW library
	if (!glfwInit()) {
		cerr << "FATAL: Failed to initialize GLFW.\n";
		exit(1);
	}
	
	// MSAA 4X
	glfwWindowHint(GLFW_SAMPLES, 4);

	// OpenGL 3.3 core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(windowSize.first, windowSize.second, "atView", NULL, NULL);
	if (window == NULL) {
		cerr << "FATAL: Failed to open GLFW window.\n";
		glfwTerminate();
		exit(2);
	}

	// Optimized for Retina display
	glfwGetFramebufferSize(window, &windowFrameBufferSize.first, &windowFrameBufferSize.second);
	scaleRatio = windowFrameBufferSize.first / windowSize.first;
	glfwMakeContextCurrent(window);

	// Initialize GLEW library
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		cerr << "FATAL: Failed to initialize GLEW.\n";	
		exit(3);
	}

	string rendererInfo = "GPU: " + (string) (char*) glGetString(GL_RENDERER);
	reportInfo(rendererInfo);
	string glVersionInfo = "OpenGL Version: " + (string) (char*) glGetString(GL_VERSION);
	reportInfo(glVersionInfo);
	string glShadingLanguageVersionInfo = "Shading Language Version: " + (string) (char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
	reportInfo(glShadingLanguageVersionInfo);

	// Load shaders from file
	try {
		meshShader.loadFromString(meshVertexShaderCode, meshFragmentShaderCode);
		pureColorShader.loadFromString(pureColorVertexShaderCode, pureColorFragmentShaderCode);
	} catch (const string msg) {
		cerr << msg << endl;
		exit(4);
	}

	// Load scenes from file
	for (int i = 1; i < argc; i++) 
	try {
		Scene newScene;
		newScene.LoadFromFile(argv[i]);
		scenes.push_back(newScene);
		scenes[scenes.size() - 1].initBO();
	} catch (const string msg) {
		cerr << msg << endl;
	}

	// Initialize camera, light and so on
	if (scenes.size() > 0) {
		float minLenv = min(scenes[0].lenv.x, min(scenes[0].lenv.y, scenes[0].lenv.z));
		float maxLenv = max(scenes[0].lenv.x, max(scenes[0].lenv.y, scenes[0].lenv.z));

		// Initialize camera
		camera.pos = scenes[0].minv + scenes[0].lenv * vec3(0.5f, 0.5f, 0.5f);
		camera.moveForward(-scenes[0].lenv.z * 2.0f);
		
		// Initialize lighting
		light0.pos = scenes[0].maxv + scenes[0].lenv * vec3(0.2f, 0.2f, 0.2f);
		light0.init(minLenv / 50.0f);
		
		// Set moving moveSpeed
		moveSpeed = minLenv / 50.0f;
		globalAxis.init(maxLenv);
		gridlines.init(maxLenv * 10, maxLenv * 10, maxLenv / 2);
	} else {
		// Initialize camera
		camera.pos = vec3(50.0f, 50.0f, 300.0f);

		// Set moving moveSpeed
		moveSpeed = 2.0f;
		globalAxis.init(100.0f);
		gridlines.init(1000.0f, 1000.0f, 50.0f);
	}

	// Dump info to console
	for (uint32_t i = 0; i < scenes.size(); i++)
		scenes[i].dumpinfo("");

	// Add tweak bars
	TwInit(TW_OPENGL_CORE, NULL);
	TwDefine("GLOBAL fontsize=3");
	TwWindowSize(windowFrameBufferSize.first, windowFrameBufferSize.second);

	// Show FPS and camara info
	TwBar * viewerInfoBar = TwNewBar("Viewer Info");
	TwSetParam(viewerInfoBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwSetParam(viewerInfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 10");
	TwSetParam(viewerInfoBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 170");
	TwAddVarRO(viewerInfoBar, "FPS", TW_TYPE_INT32, &fps, "");
	TwAddVarRO(viewerInfoBar, "Window Width", TW_TYPE_INT32, &windowFrameBufferSize.first, "");
	TwAddVarRO(viewerInfoBar, "Window Height", TW_TYPE_INT32, &windowFrameBufferSize.second, "");
	TwAddVarRW(viewerInfoBar, "Moving Speed", TW_TYPE_FLOAT, &moveSpeed, "step=0.1");
	TwAddVarRW(viewerInfoBar, "Camera Position X", TW_TYPE_FLOAT, &camera.pos.x, "step=0.1");
	TwAddVarRW(viewerInfoBar, "Camera Position Y", TW_TYPE_FLOAT, &camera.pos.y, "step=0.1");
	TwAddVarRW(viewerInfoBar, "Camera Position Z", TW_TYPE_FLOAT, &camera.pos.z, "step=0.1");

	// Show lighting info
	TwBar * lightBar = TwNewBar("Point Light");
	TwSetParam(lightBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwSetParam(lightBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 190");
	TwSetParam(lightBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 150");
	TwAddVarRW(lightBar, "Ambient Color", TW_TYPE_COLOR3F, &light0.ambient.x, "");
	TwAddVarRW(lightBar, "Diffuse Color", TW_TYPE_COLOR3F, &light0.diffuse.x, "");
	TwAddVarRW(lightBar, "Specular Color", TW_TYPE_COLOR3F, &light0.specular.x, "");
	TwAddVarRW(lightBar, "Light Postion X", TW_TYPE_FLOAT, &light0.pos.x, "step=0.1");
	TwAddVarRW(lightBar, "Light Postion Y", TW_TYPE_FLOAT, &light0.pos.y, "step=0.1");
	TwAddVarRW(lightBar, "Light Postion Z", TW_TYPE_FLOAT, &light0.pos.z, "step=0.1");

	// Show OpenGL config
	TwBar * configBar = TwNewBar("Configuration");
	TwSetParam(configBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwSetParam(configBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 350");
	TwSetParam(configBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 200");
	TwAddVarRW(configBar, "Lighting", TW_TYPE_BOOLCPP, &enableLight, "");
	TwAddVarRW(configBar, "Texture", TW_TYPE_BOOLCPP, &enableTexture, "");
	TwAddVarRW(configBar, "Depth Test", TW_TYPE_BOOLCPP, &enableDepthTest, "");
	TwAddVarRW(configBar, "Cull Face", TW_TYPE_BOOLCPP, &enableCullFace, "");
	TwAddVarRW(configBar, "MSAA 4X", TW_TYPE_BOOLCPP, &enableMultiSample, "");
	TwAddVarRW(configBar, "Gridlines", TW_TYPE_BOOLCPP, &enableGridlines, "");
	TwAddVarRW(configBar, "Global Axis", TW_TYPE_BOOLCPP, &enableGlobalAxis, "");
	TwAddVarCB(configBar, "Shade Model", TW_TYPE_STDSTRING, setShadeModel, getShadeModel, &shadeModelStr, NULL);
	TwAddVarCB(configBar, "Polygon Mode", TW_TYPE_STDSTRING, setPolygonMode, getPolygonMode, &polygonModeStr, NULL);

	// Show application info
	TwBar * appInfoBar = TwNewBar("Application Info");
	TwSetParam(appInfoBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwSetParam(appInfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 560");
	TwSetParam(appInfoBar, NULL, "size", TW_PARAM_CSTRING, 1, "280 120");
	TwAddButton(appInfoBar, "1.0", NULL, NULL, "label='App Version: v0.2.2'");
	TwAddButton(appInfoBar, "1.1", NULL, NULL, ("label='" + rendererInfo + "'").c_str());
	TwAddButton(appInfoBar, "1.2", NULL, NULL, ("label='" + glVersionInfo + "'").c_str());
	TwAddButton(appInfoBar, "1.3", NULL, NULL, ("label='" + glShadingLanguageVersionInfo + "'").c_str());


	// Set callback functions
	glfwSetWindowSizeCallback(window, windowSizeCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCharCallback(window, charCallback);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set Camera viewport
	glViewport(0, 0, windowFrameBufferSize.first, windowFrameBufferSize.second);
	
	// Start loop
	double lastTime = glfwGetTime();
	for (uint32_t totframes = 0;; totframes++) {
		if (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
		double cntTime = glfwGetTime();
		if (cntTime - lastTime >= 1.0f) {
			fps = totframes;
			totframes = 0;
			lastTime += 1.0f;
		}
		render(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	TwTerminate();
	glfwTerminate();
	return 0;
}
