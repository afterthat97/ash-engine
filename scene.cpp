#include "scene.h"

void Scene::init() {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].init();
}

void Scene::render(Shader& shader) {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].render(shader);
}

void Scene::dumpinfo(string tab) {
	printf("%sScene %s, %lu models and %lu materials in total.\n",
		tab.c_str(), name.c_str(), models.size(), materials.size());
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].dumpinfo(tab + "  ");
}
