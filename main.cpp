#include "AntTweakBar.h"
#include "utilities.h"
#include "scene.h"
#include "camera.h"
#include "light.h"
#include "glconfig.h"
#include "io.h"
#include "shader.h"

typedef void(*func) ();
pair<double, double> lastPointerPos;
pair<int32_t, int32_t> windowSize = { 1024, 576 };
pair<int32_t, int32_t> windowFrameBufferSize;
int32_t scaleRatio;
Camera camera;
GLFWwindow* window;
vector<Light> lights;
vector<Scene> scenes;
vector<func> menuOptions;
Shader shader;
int32_t fps = 60, canMove = 1, canChangeViewport = 0;

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
	camera.moveForward((GLfloat) yoffse * 10.0f);
}

void keyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	if (action != GLFW_PRESS) return;
	int k = 0;
	switch (key) {
		case GLFW_KEY_F10: {
			uint8_t* pixels = new uint8_t[3 * windowFrameBufferSize.first * windowFrameBufferSize.second];
			glReadPixels(0, 0, windowFrameBufferSize.first, windowFrameBufferSize.second, GL_BGR, GL_UNSIGNED_BYTE, pixels);
			saveImage(windowFrameBufferSize.first, windowFrameBufferSize.second, pixels);
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

void drawGridlines(GLfloat length, GLfloat width, GLfloat inc) {
	return;
	length /= 2; width /= 2;
	// Draw grid lines
	GLfloat posx = floor(camera.pos.x / inc) * inc;
	GLfloat posz = floor(camera.pos.z / inc) * inc;
	glColor3f(0.2f, 0.2f, 0.2f);
	for (GLfloat x = -length; x <= length; x += inc) {
		glBegin(GL_LINES);
		glVertex3f(x + posx, 0.0f, posz - width);
		glVertex3f(x + posx, 0.0f, posz + width);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(posx - length, 0.0f, x + posz);
		glVertex3f(posx + length, 0.0f, x + posz);
		glEnd();
	}
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Move camera by keyboard
	if (canMove) {
		GLfloat speed = 1.0f;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) speed = 10.0f;
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) speed = 10.0f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.moveForward(speed);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.moveForward(-speed);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.moveRight(-speed);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.moveRight(speed);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.moveUp(-speed);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.moveUp(speed);
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
	mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowSize.first / (float)windowSize.second, 0.1f, 10000.0f);
	mat4 view = glm::lookAt(camera.pos, camera.pos + camera.dir, camera.up);
	shader.setMat4("projection", projection);
    shader.setMat4("view", view);
	
	// Draw all scenes
	glShadeModel(shadeModelStr == "FLAT" ? GL_FLAT : GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, (polygonModeStr == "LINE" ? GL_LINE : GL_FILL));
	if (enableGridlines) drawGridlines(10000.0f, 10000.0f, 100.0f);
	if (enableTexture) glEnable(GL_TEXTURE_2D);
	if (enableLight) glEnable(GL_LIGHTING);
	if (enableCullFace) glEnable(GL_CULL_FACE);
	if (enableDepthTest) glEnable(GL_DEPTH_TEST);
	for (uint32_t i = 0; i < scenes.size(); i++)
		scenes[i].render();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	// Draw tweak bars
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	TwDraw();
	glFlush();
}

int main(int argc, char **argv) {
#ifdef __APPLE__
	CGEventSourceRef evsrc = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
	CGEventSourceSetLocalEventsSuppressionInterval(evsrc, 0.0);
#endif
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(windowSize.first, windowSize.second, "atView", NULL, NULL);
	if (!window) { glfwTerminate(); return -1; }
	glfwGetFramebufferSize(window, &windowFrameBufferSize.first, &windowFrameBufferSize.second);
	scaleRatio = windowFrameBufferSize.first / windowSize.first;
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	glewInit();

	try {
		shader.loadFromFile("shader/vertexShader.glsl", "shader/fragmentShader.glsl");
	} catch (const string msg) {
		cerr << msg << endl;
		exit(-1);
	}
	
	shader.use();
	shader.setInt("texture0", 0);

	// Load scene from file
	for (int i = 1; i < argc; i++) try {
		scenes.push_back(Scene(argv[i]));
		scenes[scenes.size() - 1].init();
	} catch (const string msg) {
		cerr << msg << endl;
	}

	if (scenes.size() > 0) {
		//Set camera position
		camera.pos = vec3((scenes[0].xMax + scenes[0].xMin) / 2,
			(scenes[0].yMax + scenes[0].yMin) / 2,
			(scenes[0].zMax - scenes[0].zMin) / 2 + scenes[0].zMax);
	}

	// Dump info to console
	putchar('\n');
	for (uint32_t i = 0; i < scenes.size(); i++) scenes[i].dumpinfo("");
	putchar('\n');
	for (uint32_t i = 0; i < lights.size(); i++) lights[i].dumpinfo("");
	putchar('\n');

	// Add tweak bars
	TwInit(TW_OPENGL_CORE, NULL);
	TwDefine("GLOBAL fontsize=3");
	TwWindowSize(windowFrameBufferSize.first, windowFrameBufferSize.second);
	TwBar * InfoBar = TwNewBar("Info");
	TwSetParam(InfoBar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwSetParam(InfoBar, NULL, "position", TW_PARAM_CSTRING, 1, "5 5");
	TwSetParam(InfoBar, NULL, "size", TW_PARAM_CSTRING, 1, "200 115");
	TwAddVarRO(InfoBar, "FPS", TW_TYPE_INT32, &fps, "");
	TwAddVarRW(InfoBar, "Camera X", TW_TYPE_FLOAT, &camera.pos.x, "step=0.1");
	TwAddVarRW(InfoBar, "Camera Y", TW_TYPE_FLOAT, &camera.pos.y, "step=0.1");
	TwAddVarRW(InfoBar, "Camera Z", TW_TYPE_FLOAT, &camera.pos.z, "step=0.1");
	TwBar * Config = TwNewBar("Config");
	TwSetParam(Config, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwSetParam(Config, NULL, "position", TW_PARAM_CSTRING, 1, "5 130");
	TwSetParam(Config, NULL, "size", TW_PARAM_CSTRING, 1, "200 165");
	TwAddVarRW(Config, "Lighting", TW_TYPE_BOOLCPP, &enableLight, "");
	TwAddVarRW(Config, "Texture", TW_TYPE_BOOLCPP, &enableTexture, "");
	TwAddVarRW(Config, "DepthTest", TW_TYPE_BOOLCPP, &enableDepthTest, "");
	TwAddVarRW(Config, "Gridlines", TW_TYPE_BOOLCPP, &enableGridlines, "");
	TwAddVarRW(Config, "CullFace", TW_TYPE_BOOLCPP, &enableCullFace, "");
	TwAddVarCB(Config, "ShadeModel", TW_TYPE_STDSTRING, setShadeModel, getShadeModel, &shadeModelStr, NULL);
	TwAddVarCB(Config, "PolygonMode", TW_TYPE_STDSTRING, setPolygonMode, getPolygonMode, &polygonModeStr, NULL);

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
	for (int32_t totframes = 0;; totframes++) {
		if (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
		double cntTime = glfwGetTime();
		if (cntTime - lastTime >= 1.0f) {
			fps = totframes; totframes = 0; lastTime += 1.0f;
		}
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	TwTerminate();
	glfwTerminate();
	return 0;
}
