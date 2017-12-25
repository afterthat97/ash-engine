#include "model.h"

void Model::initBO() {
	for (uint32_t i = 0; i < meshes.size(); i++)
		meshes[i].initBO();
	for (uint32_t i = 0; i < children.size(); i++)
		children[i].initBO();
}

void Model::render(Shader& shader) {
	for (uint32_t i = 0; i < meshes.size(); i++)
		meshes[i].render(shader);
	for (uint32_t i = 0; i < children.size(); i++)
		children[i].render(shader);
}

void Model::dumpinfo(string tab) {
	printf("%sModel %s, %lu meshes and %lu children in total.\n",
		tab.c_str(), name.c_str(), meshes.size(), children.size());
	for (uint32_t i = 0; i < meshes.size(); i++)
		meshes[i].dumpinfo(tab + "  ");
	for (uint32_t i = 0; i < children.size(); i++)
		children[i].dumpinfo(tab + "  ");
}
