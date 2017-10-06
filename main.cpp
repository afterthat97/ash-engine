#include "utilities.h"
#include "scene.h"
#include "camera.h"
#include "light.h"
#include "glconfig.h"

typedef void (*func) ();
pair<int32_t, int32_t> lastPointerPos;
Camera camera;
Window window;
vector<Light> lights;
vector<Scene> scenes;
vector<func> menuOptions;
vector3f dCursor;
bool pressKey[256];

void reshape(int32_t x, int32_t y) {
	window.width = x; window.height = y;
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	gluPerspective(45.0f, 1.0f * x / y, 1.0f, 100000.0f);
}

void keyDown(uint8_t key, int32_t x, int32_t y) { pressKey[key] = true; }
void keyUp(uint8_t key, int32_t x, int32_t y) { pressKey[key] = false; }

void mouse(int32_t button, int32_t state, int32_t x, int32_t y) {
	if (state == GLUT_UP)
		glutSetCursor(GLUT_CURSOR_INHERIT);
}

void passiveMotion(int32_t x, int32_t y) {
	glutSetCursor(GLUT_CURSOR_INHERIT);
}

void motion(int32_t x, int32_t y) {
	glutSetCursor(GLUT_CURSOR_NONE);
	int32_t dx = x - lastPointerPos.first;
	int32_t dy = y - lastPointerPos.second;
	lastPointerPos.first = x; lastPointerPos.second = y;
	if (abs(dx) > (window.width >> 3)) return;
	if (abs(dy) > (window.height >> 3)) return;
    if ( x < 9 || y < 9 || x > window.width - 9 || y > window.height - 9) {
		lastPointerPos.first = window.width >> 1;
		lastPointerPos.second =  window.height >> 1;
		glutWarpPointer(lastPointerPos.first, lastPointerPos.second);
	} else {
		dCursor.x += dx >> 1;
		dCursor.y += dy >> 1;
	}
}

void drawGridlines() {
	// Draw grid lines
	int32_t inc = 240;
	if (fabs(camera.pos.y) < 80) inc = 20;
	else if (fabs(camera.pos.y) < 500) inc = 80;
	int32_t posx = (int32_t)camera.pos.x / inc * inc;
	int32_t posz = (int32_t)camera.pos.z / inc * inc;
	glColor3f(0.2f, 0.2f, 0.2f);
	// 2000 * 2000 grid lines
	for (int32_t x = -2000; x <= 2000; x += inc) {
		glBegin(GL_LINES);
			glVertex3f(x + posx, 0, posz - 2000);
			glVertex3f(x + posx, 0, posz + 2000);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(posx - 2000, 0, x + posz);
			glVertex3f(posx + 2000, 0, x + posz);
		glEnd();
	}
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Move camera by keyboard
	if (pressKey[(uint8_t)'w']) camera.moveForward(2.0f);
	if (pressKey[(uint8_t)'s']) camera.moveForward(-2.0f);
	if (pressKey[(uint8_t)'a']) camera.moveRight(-2.0f);
	if (pressKey[(uint8_t)'d']) camera.moveRight(2.0f);
	if (pressKey[(uint8_t)'q']) camera.moveUp(-2.0f);
	if (pressKey[(uint8_t)'e']) camera.moveUp(2.0f);
	// Move camera by mouse
	camera.turnLeft(-dCursor.x / 200);
	camera.lookUp(-dCursor.y / 200);
	dCursor = vector3f(0.0f, 0.0f, 0.0f);
	// Apply camera viewport
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z,
		camera.pos.x + camera.dir.x,
		camera.pos.y + camera.dir.y,
		camera.pos.z + camera.dir.z,
		camera.up.x, camera.up.y, camera.up.z);
	// Draw all scenes
	glShadeModel(shadeModel);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	if (enableGridlines) drawGridlines();
	if (enableTexture) glEnable(GL_TEXTURE_2D);
	if (enableLight) glEnable(GL_LIGHTING);
	if (enableDepthTest) glEnable(GL_DEPTH_TEST);
	for (uint32_t i = 0; i < scenes.size(); i++)
		scenes[i].draw();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
    glFlush();
    glutSwapBuffers(); 
}

void menu(int32_t value) { menuOptions[value](); }

int main(int argc, char **argv) {
	if (argc == 1) return 0;
#ifdef __APPLE__
	CGSetLocalEventsSuppressionInterval(0.0);
#endif
	// Init OpenGL window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window.width, window.height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(window.title.c_str());
	// Set callback functions
    glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
    glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutFullScreen();
	// Set viewport
	reshape(window.width, window.height);
	// Add menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Toggle Lighting", 0);
	glutAddMenuEntry("Toggle Grid Lines", 1);
	glutAddMenuEntry("Toggle Texture", 2);
	glutAddMenuEntry("Toggle Polygon Mode", 3);
	glutAddMenuEntry("Toggle Depth Test", 4);
	glutAddMenuEntry("Toggle Shade Model", 5);
	menuOptions.push_back(toggleLighting);
	menuOptions.push_back(toggleGridlines);
	menuOptions.push_back(toggleTexture);
	menuOptions.push_back(togglePolygonMode);
	menuOptions.push_back(toggleDepthTest);
	menuOptions.push_back(toggleShadeModel);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	// Load scene from file
	for (int i = 1; i < argc; i++)
		scenes.push_back(Scene(argv[i]));
	//Set camera
	camera.pos = vector3f( (scenes[0].xMax + scenes[0].xMin) / 2,
				   (scenes[0].yMax + scenes[0].yMin) / 2,
				   (scenes[0].zMax - scenes[0].zMin) / 2 + scenes[0].zMax );
	camera.dir = vector3f(0, 0, -1);
	camera.up  = vector3f(0, 1, 0);
	//Add lights
	Light light1(fabs(scenes[0].xMax) * 4, fabs(scenes[0].yMax) * 4, fabs(scenes[0].zMax) * 2, 0.0f, 0);
	lights.push_back(light1);
	for (uint32_t i = 0; i < lights.size(); i++)
		lights[i].enable();
	// Start loop
    glutMainLoop();
    return(0);
}
