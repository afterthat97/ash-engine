#include "scene.h"

void loadMesh(const aiMesh* aiMeshPtr, Mesh& newMesh, Scene& newScene) {
	newMesh.name = aiMeshPtr->mName.C_Str();
	for (uint32_t i = 0; i < aiMeshPtr->mNumVertices; i++) {
		if (aiMeshPtr->HasPositions()) {
			vec3 vertex(aiMeshPtr->mVertices[i].x, aiMeshPtr->mVertices[i].y, aiMeshPtr->mVertices[i].z);
			newMesh.vertices.push_back(vertex.x);
			newMesh.vertices.push_back(vertex.y);
			newMesh.vertices.push_back(vertex.z);
			newMesh.minv = minVec3(newMesh.minv, vertex);
			newMesh.maxv = maxVec3(newMesh.maxv, vertex);
		}
		if (aiMeshPtr->HasNormals()) {
			newMesh.normals.push_back(aiMeshPtr->mNormals[i].x);
			newMesh.normals.push_back(aiMeshPtr->mNormals[i].y);
			newMesh.normals.push_back(aiMeshPtr->mNormals[i].z);
		}
		newMesh.tangents.push_back(aiMeshPtr->mTangents[i].x);
		newMesh.tangents.push_back(aiMeshPtr->mTangents[i].y);
		newMesh.tangents.push_back(aiMeshPtr->mTangents[i].z);
		if (aiMeshPtr->HasTextureCoords(0)) {
			newMesh.texCoords.push_back(aiMeshPtr->mTextureCoords[0][i].x);
			newMesh.texCoords.push_back(aiMeshPtr->mTextureCoords[0][i].y);
		}
	}
	for (uint32_t i = 0; i < aiMeshPtr->mNumFaces; i++)
		for (uint32_t j = 0; j < 3; j++)
			newMesh.indices.push_back(aiMeshPtr->mFaces[i].mIndices[j]);
	newMesh.lenv = newMesh.maxv - newMesh.minv;
	newMesh.materials.push_back(newScene.materials[aiMeshPtr->mMaterialIndex]);
}

void loadModel(const aiNode* aiNodePtr, const aiScene* aiScenePtr, Model& newModel, Scene& newScene) {
	newModel.name = aiNodePtr->mName.C_Str();
	for (uint32_t i = 0; i < aiNodePtr->mNumMeshes; i++) {
		Mesh newMesh;
		loadMesh(aiScenePtr->mMeshes[aiNodePtr->mMeshes[i]], newMesh, newScene);
		newModel.meshes.push_back(newMesh);
		newModel.minv = minVec3(newModel.minv, newMesh.minv);
		newModel.maxv = maxVec3(newModel.maxv, newMesh.maxv);
	}
	for (uint32_t i = 0; i < aiNodePtr->mNumChildren; i++) {
		Model newChildModel;
		loadModel(aiNodePtr->mChildren[i], aiScenePtr, newChildModel, newScene);
		newModel.children.push_back(newChildModel);
		newModel.minv = minVec3(newModel.minv, newChildModel.minv);
		newModel.maxv = maxVec3(newModel.maxv, newChildModel.maxv);
	}
	newModel.lenv = newModel.maxv - newModel.minv;
}

void loadColor(const aiColor4D& col, vec3& arr) {
	arr = vec3(col.r, col.g, col.b);
}

void loadMaterial(const aiMaterial* aiMaterialPtr, Material& newMaterial, string dir) {
	aiColor4D color; float value; aiString aiStr;
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_NAME, aiStr)) {
		newMaterial.name = aiStr.C_Str();
		if (newMaterial.name == "")
			newMaterial.loadDefaultName();
	}
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_AMBIENT, color))
		loadColor(color, newMaterial.ambient);
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_DIFFUSE, color))
		loadColor(color, newMaterial.diffuse);
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_SPECULAR, color))
		loadColor(color, newMaterial.specular);
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_SHININESS, value)) {
		newMaterial.shininess = value;
		if (newMaterial.shininess < 1e-2)
			newMaterial.loadDefaultShininess();
	}

	// Ambient map (current not supported)
	/*
	for (uint32_t i = 0; i < aiMaterialPtr -> GetTextureCount(aiTextureType_AMBIENT); i++)
	if (AI_SUCCESS == aiMaterialPtr -> GetTexture(aiTextureType_AMBIENT, i, &aiStr))
	try {
	newMaterial.ambientMap = loadTexture(dir, aiStr.C_Str());
	} catch (const string msg) {
	cerr << msg << endl;
	}
	*/

	// Diffuse map
	for (uint32_t i = 0; i < aiMaterialPtr->GetTextureCount(aiTextureType_DIFFUSE); i++)
		if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_DIFFUSE, i, &aiStr)) {
			try {
				Texture newTexture(DIFFUSE);
				newTexture.loadFromFile(dir, aiStr.C_Str());
				newMaterial.textures.push_back(newTexture);
				newMaterial.loadDefaultDiffuseRGB();
			} catch (const char* msg) {
				cerr << msg << endl;
			}
		}


	// Emissive map (currently not supported)
	/*
	for (uint32_t i = 0; i < aiMaterialPtr -> GetTextureCount(aiTextureType_EMISSIVE); i++)
	if (AI_SUCCESS == aiMaterialPtr -> GetTexture(aiTextureType_EMISSIVE, i, &aiStr))
	try {
	newMaterial.emissiveMap = loadTexture(aiStr.C_Str());
	} catch (const string msg) {
	cerr << msg << endl;
	}
	*/

	// Specular map
	for (uint32_t i = 0; i < aiMaterialPtr->GetTextureCount(aiTextureType_SPECULAR); i++)
		if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_SPECULAR, i, &aiStr)) {
			try {
				Texture newTexture(SPECULAR);
				newTexture.loadFromFile(dir, aiStr.C_Str());
				newMaterial.textures.push_back(newTexture);
				newMaterial.loadDefaultSpecularRGB();
			} catch (const char* msg) {
				cerr << msg << endl;
			}
		}

	// Normal map
	for (uint32_t i = 0; i < aiMaterialPtr->GetTextureCount(aiTextureType_HEIGHT); i++)
		if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_HEIGHT, i, &aiStr)) {
			try {
				Texture newTexture(NORMAL);
				newTexture.loadFromFile(dir, aiStr.C_Str());
				newMaterial.textures.push_back(newTexture);
			} catch (const char* msg) {
				cerr << msg << endl;
			}
		}
}

void Scene::LoadFromFile(string filename) {
	string dir = "";
	for (uint32_t i = 0; i < filename.length(); i++)
		if (filename[i] == '/' || filename[i] == '\\')
			dir = filename.substr(0, i + 1);
	Assimp::Importer importer;
	const aiScene* aiScenePtr = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (aiScenePtr == NULL || aiScenePtr->mFlags == AI_SCENE_FLAGS_INCOMPLETE || aiScenePtr->mRootNode == NULL)
		throw string(importer.GetErrorString());
	this->name = aiScenePtr->mRootNode->mName.C_Str();
	for (uint32_t i = 0; i < aiScenePtr->mNumMaterials; i++) {
		Material newMaterial;
		loadMaterial(aiScenePtr->mMaterials[i], newMaterial, dir);
		this->materials.push_back(newMaterial);
	}
	for (uint32_t i = 0; i < aiScenePtr->mRootNode->mNumChildren; i++) {
		Model newModel;
		loadModel(aiScenePtr->mRootNode->mChildren[i], aiScenePtr, newModel, *this);
		this->models.push_back(newModel);
		this->minv = minVec3(this->minv, newModel.minv);
		this->maxv = maxVec3(this->maxv, newModel.maxv);
	}
	this->lenv = this->maxv - this->minv;
}

void Scene::initBO() {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].initBO();
}

void Scene::computeSDF(uint32_t sample) {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].computeSDF(sample);
}

void Scene::render(Shader& shader) {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].render(shader);
}

void Scene::renderSDF(Shader& shader) {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].renderSDF(shader);
}

void Scene::dumpinfo(string tab) {
	printf("\n%sScene %s, %lu models and %lu materials in total.\n",
		tab.c_str(), name.c_str(), models.size(), materials.size());
	for (uint32_t i = 0; i < models.size(); i++)
		models[i].dumpinfo(tab + "  ");
	putchar('\n');
}
