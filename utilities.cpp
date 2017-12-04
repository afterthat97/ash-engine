#include "utilities.h"

void throwError(string type, string name, string msg) {
	throw "ERROR: Failed to " + type + " " + name + ":" + msg;
}

void reportInfo(string msg) {
	cout << "INFO: " << msg << endl;
}
