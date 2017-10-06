#include "scene.h"
#include "load.h"

void Scene::loadFromFile(string filename) {
	loadScene(filename, *this);
}

void Scene::draw() {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].draw();
}

void Scene::dumpinfo() {
	printf("Scene %s, %lu models and %lu materials in total.\n",
			name.c_str(), models.size(), materials.size());
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].dumpinfo("  ");
}
