#pragma once

// 다양한 model을 load하기 위한 library
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <string>
#include <vector>
#include <filesystem>

#include "MeshData.h"

class ModelLoader {
public:
	bool LoadModel(const std::string& filePath, const std::string& fileName);
	void ProcessNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix );
	MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);
public:
	std::string basePath;
	std::vector<MeshData> meshes;
};