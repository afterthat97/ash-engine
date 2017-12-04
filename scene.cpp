#include "scene.h"
#include "io.h"

void Scene::loadFromFile(string filename) {
	loadScene(filename, *this);
}

void Scene::draw() {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].draw();
}

void Scene::dumpinfo(string tab) {
	printf("%sScene %s, %lu models and %lu materials in total.\n",
		tab.c_str(), name.c_str(), models.size(), materials.size());
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].dumpinfo(tab + "  ");
}
