#include "load.h"
#include "vector3f.h"
#include <FreeImage.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

vector3f toVec3f(const aiVector3D vec) {
	return vector3f(vec.x, vec.y, vec.z);
}

void loadFace(const aiFace aiFacePtr, Face& newFace) {
	for (uint32_t i = 0; i < aiFacePtr.mNumIndices; i++)
		newFace.indices.push_back(aiFacePtr.mIndices[i]);
}

void loadMesh(const aiMesh* aiMeshPtr, Mesh& newMesh, Scene& newScene) {
	newMesh.name = aiMeshPtr -> mName.C_Str();
	for (uint32_t i = 0; i < aiMeshPtr -> mNumVertices; i++) {
		if (aiMeshPtr -> HasPositions()) {
			newMesh.vertices.push_back(toVec3f(aiMeshPtr -> mVertices[i]));
			newMesh.xMax = fmax(newMesh.xMax, aiMeshPtr -> mVertices[i].x);
			newMesh.xMin = fmin(newMesh.xMin, aiMeshPtr -> mVertices[i].x);
			newMesh.yMax = fmax(newMesh.yMax, aiMeshPtr -> mVertices[i].y);
			newMesh.yMin = fmin(newMesh.yMin, aiMeshPtr -> mVertices[i].y);
			newMesh.zMax = fmax(newMesh.zMax, aiMeshPtr -> mVertices[i].z);
			newMesh.zMin = fmin(newMesh.zMin, aiMeshPtr -> mVertices[i].z);
		}
		if (aiMeshPtr -> HasNormals())
			newMesh.normals.push_back(toVec3f(aiMeshPtr -> mNormals[i]));
		if (aiMeshPtr -> HasTextureCoords(0))
			newMesh.texCoords.push_back(toVec3f(aiMeshPtr -> mTextureCoords[0][i]));
	}
	for (uint32_t i = 0; i < aiMeshPtr -> mNumFaces; i++) {
		Face newFace;
		loadFace(aiMeshPtr -> mFaces[i], newFace);
		newMesh.faces.push_back(newFace);
	}
	newMesh.materials.push_back(newScene.materials[aiMeshPtr -> mMaterialIndex]);
}

void loadModel(const aiNode* aiNodePtr, const aiScene* aiScenePtr, Model& newModel, Scene& newScene) {
	newModel.name = aiNodePtr -> mName.C_Str();
	for (uint32_t i = 0; i < aiNodePtr -> mNumMeshes; i++) {
		Mesh newMesh;
		loadMesh(aiScenePtr -> mMeshes[aiNodePtr -> mMeshes[i]], newMesh, newScene);
		newModel.meshes.push_back(newMesh);
		newModel.xMax = fmax(newModel.xMax, newMesh.xMax);
		newModel.xMin = fmin(newModel.xMin, newMesh.xMin);
		newModel.yMax = fmax(newModel.yMax, newMesh.yMax);
		newModel.yMin = fmin(newModel.yMin, newMesh.yMin);
		newModel.zMax = fmax(newModel.zMax, newMesh.zMax);
		newModel.zMin = fmin(newModel.zMin, newMesh.zMin);
	}
	for (uint32_t i = 0; i < aiNodePtr -> mNumChildren; i++) {
		Model newChildModel;
		loadModel(aiNodePtr -> mChildren[i], aiScenePtr, newChildModel, newScene);
		newModel.children.push_back(newChildModel);
		newModel.xMax = fmax(newModel.xMax, newChildModel.xMax);
		newModel.xMin = fmin(newModel.xMin, newChildModel.xMin);
		newModel.yMax = fmax(newModel.yMax, newChildModel.yMax);
		newModel.yMin = fmin(newModel.yMin, newChildModel.yMin);
		newModel.zMax = fmax(newModel.zMax, newChildModel.zMax);
		newModel.zMin = fmin(newModel.zMin, newChildModel.zMin);
	}
}

void loadColor(const aiColor4D& col, float* arr) {
	arr[0] = col.r; arr[1] = col.g; arr[2] = col.b; arr[3] = col.a;
}

uint32_t texture_id = 1;
uint32_t loadTexture(string dir, const char* filename) {
	FreeImage_Initialise(0);
	if (filename[0] == '/') filename = filename + 1;
	dir = dir + filename;
	printf("Loading texture file %s...\n", dir.c_str());
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(dir.c_str(), 0);
	FIBITMAP *bitmap = FreeImage_Load(fifmt, dir.c_str(), 0);
	if (bitmap == NULL) {
		printf("ERROR: Could not load texture from file %s!\n", dir.c_str());
		return 0;
	}
	bitmap = FreeImage_ConvertTo32Bits(bitmap);
	uint8_t *textureArr = (uint8_t*) FreeImage_GetBits(bitmap);	
	int32_t width = FreeImage_GetWidth(bitmap);
	int32_t height = FreeImage_GetHeight(bitmap);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureArr);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_BGRA, GL_UNSIGNED_BYTE, textureArr);
	printf("Texture file %s loaded. (Width = %d, Height = %d)\n", dir.c_str(), width, height);
	textureArr = NULL;
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
	return texture_id++;
}

void loadMaterial(const aiMaterial* aiMaterialPtr, Material& newMaterial, string dir) {
	aiColor4D color; float value; aiString aiStr;
	if (AI_SUCCESS == aiMaterialPtr -> Get(AI_MATKEY_NAME, aiStr))
		newMaterial.name = aiStr.C_Str();
	if (AI_SUCCESS == aiMaterialPtr -> Get(AI_MATKEY_COLOR_AMBIENT, color))
		loadColor(color, newMaterial.ambient);
	if (AI_SUCCESS == aiMaterialPtr -> Get(AI_MATKEY_COLOR_DIFFUSE, color))
		loadColor(color, newMaterial.diffuse);
	if (AI_SUCCESS == aiMaterialPtr -> Get(AI_MATKEY_COLOR_EMISSIVE, color))
		loadColor(color, newMaterial.emission);
	if (AI_SUCCESS == aiMaterialPtr -> Get(AI_MATKEY_COLOR_SPECULAR, color))
		loadColor(color, newMaterial.specular);
	if (AI_SUCCESS == aiMaterialPtr -> Get(AI_MATKEY_COLOR_TRANSPARENT, color))
		loadColor(color, newMaterial.transparent);
	if (AI_SUCCESS == aiMaterialPtr -> Get(AI_MATKEY_SHININESS, value))
		newMaterial.shininess = value;
/*	for (uint32_t i = 0; i < aiMaterialPtr -> GetTextureCount(aiTextureType_AMBIENT); i++)
		if (AI_SUCCESS == aiMaterialPtr -> GetTexture(aiTextureType_AMBIENT, i, &aiStr))
			newMaterial.textureIndices.push_back(loadTexture(aiStr.C_Str()));
*/	for (uint32_t i = 0; i < aiMaterialPtr -> GetTextureCount(aiTextureType_DIFFUSE); i++)
		if (AI_SUCCESS == aiMaterialPtr -> GetTexture(aiTextureType_DIFFUSE, i, &aiStr))
			newMaterial.textureIndices.push_back(loadTexture(dir, aiStr.C_Str()));
/*	for (uint32_t i = 0; i < aiMaterialPtr -> GetTextureCount(aiTextureType_EMISSIVE); i++)
		if (AI_SUCCESS == aiMaterialPtr -> GetTexture(aiTextureType_EMISSIVE, i, &aiStr))
			newMaterial.textureIndices.push_back(loadTexture(aiStr.C_Str()));
	for (uint32_t i = 0; i < aiMaterialPtr -> GetTextureCount(aiTextureType_SPECULAR); i++)
		if (AI_SUCCESS == aiMaterialPtr -> GetTexture(aiTextureType_SPECULAR, i, &aiStr))
			newMaterial.textureIndices.push_back(loadTexture(aiStr.C_Str()));
*/}

void loadScene(string filename, Scene& newScene) {
	string dir = "";
	for (uint32_t i = 0; i < filename.length(); i++)
		if (filename[i] == '/' || filename[i] == '\\')
			dir = filename.substr(0, i + 1);
	Assimp::Importer importer;
	const aiScene* aiScenePtr = importer.ReadFile(filename, aiProcess_Triangulate);
	if (aiScenePtr == NULL
		|| aiScenePtr -> mFlags == AI_SCENE_FLAGS_INCOMPLETE
		|| aiScenePtr -> mRootNode == NULL) {
			printf("ERROR: Could not load scene from %s! (%s)",
				filename.c_str(), importer.GetErrorString());
			return;
	}
	newScene.name = aiScenePtr -> mRootNode -> mName.C_Str();
	for (uint32_t i = 0; i < aiScenePtr -> mNumMaterials; i++) {
		Material newMaterial;
		loadMaterial(aiScenePtr -> mMaterials[i], newMaterial, dir);
		newScene.materials.push_back(newMaterial);
	}
	for (uint32_t i = 0; i < aiScenePtr -> mRootNode -> mNumChildren; i++) {
		Model newModel;
		loadModel(aiScenePtr -> mRootNode -> mChildren[i], aiScenePtr, newModel, newScene);
		newScene.models.push_back(newModel);
		newScene.xMax = fmax(newScene.xMax, newModel.xMax);
		newScene.xMin = fmin(newScene.xMin, newModel.xMin);
		newScene.yMax = fmax(newScene.yMax, newModel.yMax);
		newScene.yMin = fmin(newScene.yMin, newModel.yMin);
		newScene.zMax = fmax(newScene.zMax, newModel.zMax);
		newScene.zMin = fmin(newScene.zMin, newModel.zMin);
	}
	printf("File %s loaded.\n\n", filename.c_str());
	newScene.dumpinfo();
	printf("\n");
}
