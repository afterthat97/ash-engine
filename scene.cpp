#include "scene.h"
#include "vertex.h"

Mesh* loadMesh(const aiMesh* aiMeshPtr,
			   vector<Material*>& materials,
			   btDiscreteDynamicsWorld* dynamicsWorld) {
	vector<Vertex> vertices;
	for (uint32_t i = 0; i < aiMeshPtr->mNumVertices; i++) {
		Vertex vertex;
		if (aiMeshPtr->HasPositions())
			vertex.position = vec3(aiMeshPtr->mVertices[i].x, aiMeshPtr->mVertices[i].y, aiMeshPtr->mVertices[i].z);
		if (aiMeshPtr->HasNormals())
			vertex.normal = vec3(aiMeshPtr->mNormals[i].x, aiMeshPtr->mNormals[i].y, aiMeshPtr->mNormals[i].z);
		if (aiMeshPtr->HasTangentsAndBitangents())
			vertex.tangent = vec3(aiMeshPtr->mTangents[i].x, aiMeshPtr->mTangents[i].y, aiMeshPtr->mTangents[i].z);
		if (aiMeshPtr->HasTextureCoords(0))
			vertex.texCoord = vec2(aiMeshPtr->mTextureCoords[0][i].x, aiMeshPtr->mTextureCoords[0][i].y);
		vertices.push_back(vertex);
	}

	vector<uint32_t> indices;
	for (uint32_t i = 0; i < aiMeshPtr->mNumFaces; i++)
		for (uint32_t j = 0; j < 3; j++)
			indices.push_back(aiMeshPtr->mFaces[i].mIndices[j]);

	return new Mesh(vertices, indices, materials[aiMeshPtr->mMaterialIndex], dynamicsWorld, aiMeshPtr->mName.C_Str());
}

Model* loadModel(const aiNode* aiNodePtr,
				 const aiScene* aiScenePtr,
				 vector<Material*>& materials,
				 btDiscreteDynamicsWorld* dynamicsWorld) {
	Model* newModel = new Model();
	newModel->name = string(aiNodePtr->mName.C_Str());

	// Load meshes
	for (uint32_t i = 0; i < aiNodePtr->mNumMeshes; i++) {
		Mesh* newMesh = loadMesh(aiScenePtr->mMeshes[aiNodePtr->mMeshes[i]], materials, dynamicsWorld);
		newModel->addMesh(newMesh);
	}

	// Load child models
	for (uint32_t i = 0; i < aiNodePtr->mNumChildren; i++) {
		Model* newChildModel = loadModel(aiNodePtr->mChildren[i], aiScenePtr, materials, dynamicsWorld);
		newModel->addChildren(newChildModel);
	}

	return newModel;
}

Material* loadMaterial(const aiMaterial* aiMaterialPtr, string dir) {
	Material* newMaterial = new Material();
	aiColor4D color; float value; aiString aiStr;

	// material name
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_NAME, aiStr))
		newMaterial->name = string(aiStr.C_Str());

	// ambient RGB
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_AMBIENT, color))
		newMaterial->ambient = toVec3(color);

	// diffuse RGB
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_DIFFUSE, color))
		newMaterial->diffuse = toVec3(color);

	// specular RGB
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_SPECULAR, color))
		newMaterial->specular = toVec3(color);

	// shininess
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_SHININESS, value) && value > 0.01)
		newMaterial->shininess = value;

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
				Texture *newTexture = new Texture(DIFFUSE);
				newTexture->loadFromFile(dir, aiStr.C_Str());
				newMaterial->addTexture(newTexture);
			} catch (const string msg) {
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
				Texture *newTexture = new Texture(SPECULAR);
				newTexture->loadFromFile(dir, aiStr.C_Str());
				newMaterial->addTexture(newTexture);
			} catch (const string msg) {
				cerr << msg << endl;
			}
		}

	// Normal map
	for (uint32_t i = 0; i < aiMaterialPtr->GetTextureCount(aiTextureType_HEIGHT); i++)
		if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_HEIGHT, i, &aiStr)) {
			try {
				Texture *newTexture = new Texture(NORMAL);
				newTexture->loadFromFile(dir, aiStr.C_Str());
				newMaterial->addTexture(newTexture);
			} catch (const string msg) {
				cerr << msg << endl;
			}
		}
	return newMaterial;
}

Scene::Scene(string filename, btDiscreteDynamicsWorld* dynamicsWorld) {
	pos = vec3(FLT_MAX);
	model = mat4(1.0);
	rot = quat(vec3(0.0));

	string dir = "";
	for (uint32_t i = 0; i < filename.length(); i++)
		if (filename[i] == '/' || filename[i] == '\\')
			dir = filename.substr(0, i + 1);

	// Use Assimp to load scene
	Assimp::Importer importer;
	const aiScene* aiScenePtr = importer.ReadFile(filename,
												  aiProcess_Triangulate |
												  aiProcess_CalcTangentSpace |
												  aiProcess_GenSmoothNormals);
	if (aiScenePtr == NULL ||
		aiScenePtr->mRootNode == NULL ||
		aiScenePtr->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
		throwError("load", filename, importer.GetErrorString());

	// Load material
	name = string(aiScenePtr->mRootNode->mName.C_Str());
	for (uint32_t i = 0; i < aiScenePtr->mNumMaterials; i++) {
		Material* newMaterial = loadMaterial(aiScenePtr->mMaterials[i], dir);
		addMaterial(newMaterial);
	}

	// Load model
	Model *newModel = loadModel(aiScenePtr->mRootNode, aiScenePtr, materials, dynamicsWorld);
	addModel(newModel);
}

void Scene::addModel(Model *newModel) {
	pos = minVec3(pos, newModel->getPosition());
	newModel->setParent(this);
	models.push_back(newModel);
}

void Scene::addMaterial(Material *newMaterial) {
	materials.push_back(newMaterial);
}

void Scene::show() {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i]->show();
}

void Scene::hide() {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i]->hide();
}

void Scene::select() {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i]->select();
}

void Scene::deselect() {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i]->deselect();
}

void Scene::render(Shader& shader) {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i]->render(shader);
}

void Scene::dumpinfo(string tab) {
	printf("\n%sScene %s, %lu models and %lu materials in total.\n",
		   tab.c_str(), name.c_str(), models.size(), materials.size());
	for (uint32_t i = 0; i < models.size(); i++)
		models[i]->dumpinfo(tab + "  ");
	putchar('\n');
}

void Scene::addTranslation(vec3 delta) {
	pos += delta;
	for (uint32_t i = 0; i < models.size(); i++)
		models[i]->addTranslation(delta);
}

void Scene::addRotation(vec3 eularAngle) {
	for (uint32_t i = 0; i < models.size(); i++)
		models[i]->addRotation(eularAngle);
}

void Scene::setPosition(vec3 newPos) {
	addTranslation(newPos - pos);
}

vec3 Scene::getPosition() {
	return pos;
}
