#include "model.h"

Model::Model(void * _parent) {
	name = "Untitled Model";
	parent = _parent;
	pos = vec3(FLT_MAX);
}

Model::~Model() {
	for (uint32_t i = 0; i < meshes.size(); i++)
		if (meshes[i] != NULL)
			delete meshes[i];
	for (uint32_t i = 0; i < children.size(); i++)
		if (children[i] != NULL)
			delete children[i];
}

void Model::addMesh(Mesh *newMesh) {
	pos = minVec3(pos, newMesh->getPosition());
	newMesh->setParent(this);
	meshes.push_back(newMesh);
}

void Model::addChildren(Model *newModel) {
	pos = minVec3(pos, newModel->getPosition());
	newModel->setParent(this);
	children.push_back(newModel);
}

void Model::removeMesh(Mesh *target) {
	pos = vec3(FLT_MAX);
	for (uint32_t i = 0; i < meshes.size(); i++)
		if (meshes[i] == target)
			meshes[i] = NULL;
		else if (meshes[i] != NULL)
			pos = minVec3(pos, meshes[i]->getPosition());
}

void Model::removeChildren(Model *target) {
	pos = vec3(FLT_MAX);
	for (uint32_t i = 0; i < children.size(); i++)
		if (children[i] == target)
			children[i] = NULL;
		else if (children[i] != NULL)
			pos = minVec3(pos, children[i]->getPosition());
}

void Model::show() {
	for (uint32_t i = 0; i < meshes.size(); i++)
		if (meshes[i] != NULL)
			meshes[i]->show();
	for (uint32_t i = 0; i < children.size(); i++)
		if (children[i] != NULL)
			children[i]->show();
}

void Model::hide() {
	for (uint32_t i = 0; i < meshes.size(); i++)
		if (meshes[i] != NULL)
			meshes[i]->hide();
	for (uint32_t i = 0; i < children.size(); i++)
		if (children[i] != NULL)
			children[i]->hide();
}

void Model::select() {
	for (uint32_t i = 0; i < meshes.size(); i++)
		if (meshes[i] != NULL)
			meshes[i]->select();
	for (uint32_t i = 0; i < children.size(); i++)
		if (children[i] != NULL)
			children[i]->select();
}

void Model::deselect() {
	for (uint32_t i = 0; i < meshes.size(); i++)
		if (meshes[i] != NULL)
			meshes[i]->deselect();
	for (uint32_t i = 0; i < children.size(); i++)
		if (children[i] != NULL)
			children[i]->deselect();
}

void Model::render(Shader& shader) {
	for (uint32_t i = 0; i < meshes.size(); i++)
		if (meshes[i] != NULL)
			meshes[i]->render(shader);
	for (uint32_t i = 0; i < children.size(); i++)
		if (children[i] != NULL)
			children[i]->render(shader);
}

void Model::dumpinfo(string tab) {
	printf("%sModel %s, %lu meshes and %lu children in total.\n",
		   tab.c_str(), name.c_str(), meshes.size(), children.size());
	for (uint32_t i = 0; i < meshes.size(); i++)
		if (meshes[i] != NULL)
			meshes[i]->dumpinfo(tab + "  ");
	for (uint32_t i = 0; i < children.size(); i++)
		if (children[i] != NULL)
			children[i]->dumpinfo(tab + "  ");
}

void Model::addTranslation(vec3 delta) {
	pos += delta;
	for (uint32_t i = 0; i < meshes.size(); i++)
		if (meshes[i] != NULL)
			meshes[i]->addTranslation(delta);
	for (uint32_t i = 0; i < children.size(); i++)
		if (children[i] != NULL)
			children[i]->addTranslation(delta);
}

void Model::addRotation(vec3 eularAngle) {
	for (uint32_t i = 0; i < meshes.size(); i++)
		if (meshes[i] != NULL)
			meshes[i]->addRotation(eularAngle);
	for (uint32_t i = 0; i < children.size(); i++)
		if (children[i] != NULL)
			children[i]->addRotation(eularAngle);
}

void Model::setPosition(vec3 newPos) {
	addTranslation(newPos - pos);
}

vec3 Model::getPosition() {
	return pos;
}

void Model::setParent(void * _parent) {
	parent = _parent;
}

void * Model::getParent() {
	return parent;
}
