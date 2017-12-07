#include "io.h"
#include <FreeImage.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

vec3 toVec3f(const aiVector3D vec) {
	return vec3(vec.x, vec.y, vec.z);
}

void loadMesh(const aiMesh* aiMeshPtr, Mesh& newMesh, Scene& newScene) {
	newMesh.name = aiMeshPtr->mName.C_Str();
	for (uint32_t i = 0; i < aiMeshPtr->mNumVertices; i++) {
		if (aiMeshPtr->HasPositions()) {
			newMesh.vertices.push_back(aiMeshPtr->mVertices[i].x);
			newMesh.vertices.push_back(aiMeshPtr->mVertices[i].y);
			newMesh.vertices.push_back(aiMeshPtr->mVertices[i].z);
			newMesh.xMax = fmax(newMesh.xMax, aiMeshPtr->mVertices[i].x);
			newMesh.xMin = fmin(newMesh.xMin, aiMeshPtr->mVertices[i].x);
			newMesh.yMax = fmax(newMesh.yMax, aiMeshPtr->mVertices[i].y);
			newMesh.yMin = fmin(newMesh.yMin, aiMeshPtr->mVertices[i].y);
			newMesh.zMax = fmax(newMesh.zMax, aiMeshPtr->mVertices[i].z);
			newMesh.zMin = fmin(newMesh.zMin, aiMeshPtr->mVertices[i].z);
		}
		if (aiMeshPtr->HasNormals()) {
			newMesh.normals.push_back(aiMeshPtr->mNormals[i].x);
			newMesh.normals.push_back(aiMeshPtr->mNormals[i].y);
			newMesh.normals.push_back(aiMeshPtr->mNormals[i].z);
		}
		if (aiMeshPtr->HasTextureCoords(0)) {
			newMesh.texCoords.push_back(aiMeshPtr->mTextureCoords[0][i].x);
			newMesh.texCoords.push_back(aiMeshPtr->mTextureCoords[0][i].y);
		}
	}
	for (uint32_t i = 0; i < aiMeshPtr->mNumFaces; i++)
		for (uint32_t j = 0; j < 3; j++)
			newMesh.indices.push_back(aiMeshPtr->mFaces[i].mIndices[j]);
	newMesh.materials.push_back(newScene.materials[aiMeshPtr->mMaterialIndex]);
}

void loadModel(const aiNode* aiNodePtr, const aiScene* aiScenePtr, Model& newModel, Scene& newScene) {
	newModel.name = aiNodePtr->mName.C_Str();
	for (uint32_t i = 0; i < aiNodePtr->mNumMeshes; i++) {
		Mesh newMesh;
		loadMesh(aiScenePtr->mMeshes[aiNodePtr->mMeshes[i]], newMesh, newScene);
		newModel.meshes.push_back(newMesh);
		newModel.xMax = fmax(newModel.xMax, newMesh.xMax);
		newModel.xMin = fmin(newModel.xMin, newMesh.xMin);
		newModel.yMax = fmax(newModel.yMax, newMesh.yMax);
		newModel.yMin = fmin(newModel.yMin, newMesh.yMin);
		newModel.zMax = fmax(newModel.zMax, newMesh.zMax);
		newModel.zMin = fmin(newModel.zMin, newMesh.zMin);
	}
	for (uint32_t i = 0; i < aiNodePtr->mNumChildren; i++) {
		Model newChildModel;
		loadModel(aiNodePtr->mChildren[i], aiScenePtr, newChildModel, newScene);
		newModel.children.push_back(newChildModel);
		newModel.xMax = fmax(newModel.xMax, newChildModel.xMax);
		newModel.xMin = fmin(newModel.xMin, newChildModel.xMin);
		newModel.yMax = fmax(newModel.yMax, newChildModel.yMax);
		newModel.yMin = fmin(newModel.yMin, newChildModel.yMin);
		newModel.zMax = fmax(newModel.zMax, newChildModel.zMax);
		newModel.zMin = fmin(newModel.zMin, newChildModel.zMin);
	}
}

void loadColor(const aiColor4D& col, vec3& arr) {
	arr = vec3(col.r, col.g, col.b);
}

uint32_t loadTexture(string dir, const char* filename) {
	FreeImage_Initialise(0);
	if (filename[0] == '/') filename = filename + 1;
	dir = dir + filename;
	reportInfo("Loading texture file " + dir + "...");
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(dir.c_str(), 0);
	FIBITMAP *bitmap = FreeImage_Load(fifmt, dir.c_str(), 0);
	if (bitmap == NULL)
		throw ("Unable to load texture from " + dir).c_str();
	bitmap = FreeImage_ConvertTo32Bits(bitmap);
	uint8_t *textureArr = (uint8_t*) FreeImage_GetBits(bitmap);
	int32_t width = FreeImage_GetWidth(bitmap), height = FreeImage_GetHeight(bitmap);
	uint32_t texture_id = 0;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create texture and generate mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureArr);
	glGenerateMipmap(GL_TEXTURE_2D);
	reportInfo("Texture file " + dir + " loaded. (" + to_string(width) + " * " + to_string(height) + ")");
	textureArr = NULL;
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
	return texture_id;
}

void loadMaterial(const aiMaterial* aiMaterialPtr, Material& newMaterial, string dir) {
	aiColor4D color; float value; aiString aiStr;
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_NAME, aiStr))
		newMaterial.name = aiStr.C_Str();
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_AMBIENT, color))
		loadColor(color, newMaterial.ambient);
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_DIFFUSE, color))
		loadColor(color, newMaterial.diffuse);
//	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_EMISSIVE, color))
//		loadColor(color, newMaterial.emission);
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_SPECULAR, color))
		loadColor(color, newMaterial.specular);
//	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_TRANSPARENT, color))
//		loadColor(color, newMaterial.transparent);
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_SHININESS, value))
		newMaterial.shininess = value;
	if (newMaterial.shininess < 1e-2) newMaterial.shininess = 32.0f;

	// Ambient map
	for (uint32_t i = 0; i < aiMaterialPtr -> GetTextureCount(aiTextureType_AMBIENT); i++)
		if (AI_SUCCESS == aiMaterialPtr -> GetTexture(aiTextureType_AMBIENT, i, &aiStr))
			try {
				newMaterial.ambientMap = loadTexture(dir, aiStr.C_Str());
			} catch (const string msg) {
				cerr << msg << endl;
			}

	// Diffuse map
	for (uint32_t i = 0; i < aiMaterialPtr->GetTextureCount(aiTextureType_DIFFUSE); i++)
		if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_DIFFUSE, i, &aiStr))
			try {
				newMaterial.diffuseMap = loadTexture(dir, aiStr.C_Str());
			} catch (const char* msg) {
				cerr << msg << endl;
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
	for (uint32_t i = 0; i < aiMaterialPtr -> GetTextureCount(aiTextureType_SPECULAR); i++)
		if (AI_SUCCESS == aiMaterialPtr -> GetTexture(aiTextureType_SPECULAR, i, &aiStr))
			try {
				newMaterial.specularMap = loadTexture(dir, aiStr.C_Str());
			} catch (const char* msg) {
				cerr << msg << endl;
			}
}

int32_t loadScene(string filename, Scene& newScene) {
	string dir = "";
	for (uint32_t i = 0; i < filename.length(); i++)
		if (filename[i] == '/' || filename[i] == '\\')
			dir = filename.substr(0, i + 1);
	Assimp::Importer importer;
	const aiScene* aiScenePtr = importer.ReadFile(filename, aiProcess_Triangulate);
	if (aiScenePtr == NULL || aiScenePtr->mFlags == AI_SCENE_FLAGS_INCOMPLETE || aiScenePtr->mRootNode == NULL)
		throw importer.GetErrorString();
	newScene.name = aiScenePtr->mRootNode->mName.C_Str();
	for (uint32_t i = 0; i < aiScenePtr->mNumMaterials; i++) {
		Material newMaterial;
		loadMaterial(aiScenePtr->mMaterials[i], newMaterial, dir);
		newScene.materials.push_back(newMaterial);
	}
	for (uint32_t i = 0; i < aiScenePtr->mRootNode->mNumChildren; i++) {
		Model newModel;
		loadModel(aiScenePtr->mRootNode->mChildren[i], aiScenePtr, newModel, newScene);
		newScene.models.push_back(newModel);
		newScene.xMax = fmax(newScene.xMax, newModel.xMax);
		newScene.xMin = fmin(newScene.xMin, newModel.xMin);
		newScene.yMax = fmax(newScene.yMax, newModel.yMax);
		newScene.yMin = fmin(newScene.yMin, newModel.yMin);
		newScene.zMax = fmax(newScene.zMax, newModel.zMax);
		newScene.zMin = fmin(newScene.zMin, newModel.zMin);
	}
	return 1;
}

uint32_t image_id = 0;
void saveImage(int32_t width, int32_t height, uint8_t* pixels) {
	FreeImage_Initialise(0);
	string filename = "Screenshot" + to_string(image_id++) + ".bmp";
	FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(
		pixels, width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, 0);
	FreeImage_Save(FIF_BMP, bitmap, filename.c_str(), 0);
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}
