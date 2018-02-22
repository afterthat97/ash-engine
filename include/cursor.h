#ifndef CURSOR_H
#define CURSOR_H

#include "utilities.h"

class Cursor {
private:
	vec2 cntPos, lastPos;
public:
	Cursor() {}
	Cursor(GLFWwindow* window) {
		update(window);
	}
	void update(GLFWwindow* window) {
		lastPos = cntPos;
		double tmpx, tmpy;
		glfwGetCursorPos(window, &tmpx, &tmpy);
		cntPos = vec2((float) tmpx, (float) tmpy);
	}
	void update(vec2 newPos) {
		lastPos = cntPos;
		cntPos = newPos;
	}
	vec2 getDeltafv() {
		return cntPos - lastPos;
	}
	vec2 getLastPosfv() {
		return lastPos;
	}
	vec2 getCntPosfv() {
		return cntPos;
	}
};

#endif
