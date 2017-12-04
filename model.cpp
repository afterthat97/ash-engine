#include "model.h"

void Model::init() {
	for (uint32_t i = 0; i < meshes.size(); i++)
		meshes[i].init();
	for (uint32_t i = 0; i < children.size(); i++)
		children[i].init();
}

void Model::render() {
	for (uint32_t i = 0; i < meshes.size(); i++)
		meshes[i].render();
	for (uint32_t i = 0; i < children.size(); i++)
		children[i].render();
}

void Model::dumpinfo(string tab) {
	printf("%sModel %s, %lu meshes and %lu children in total.\n",
		tab.c_str(), name.c_str(), meshes.size(), children.size());
	for (uint32_t i = 0; i < meshes.size(); i++)
		meshes[i].dumpinfo(tab + "  ");
	for (uint32_t i = 0; i < children.size(); i++)
		children[i].dumpinfo(tab + "  ");
}
