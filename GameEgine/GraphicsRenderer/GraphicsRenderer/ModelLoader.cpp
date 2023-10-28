#include "ModelLoader.h"
#include <iostream>
using DirectX::SimpleMath::Matrix;

bool ModelLoader::Load(const std::string& filePath, const std::string& fileName)
{
    this->basePath = filePath;

    // Assimp Library 사용하기 위한 intreface
    Assimp::Importer importer;

    // 파일에서 모델 데이터 가져오기
    // aiScene은 데이터의 root로 모든 nodes, meshes, materials, animaions, textures에 접근 가능
    const aiScene* pScene = importer.ReadFile(
        this->basePath + fileName,
        // Assimp는 default로 right-handed 좌표계를 사용하므로(OpenGL) 왼손 좌표계로 변환(DirectX)
        // aiProcess_ConvertToLeftHanded = 데이터 format을 directX에 맞게 변환시켜준다(index 순서, uv 좌표 등)
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded// 모든 메쉬를 삼각형으로 만든다.
    );

    // 데이터 가져오는데 실패했다면
    if (pScene == nullptr) {
        std::cout << importer.GetErrorString() << '\n';
        return false;
    }

    // 파일의 데이터에 접근
    // 한 모델은 여러 개의 서브 mesh들로 이루어져있다. (인간 : 머리, 몸통, 팔다리, 옷 등)
    Matrix tr;
    ProcessNode(pScene->mRootNode, pScene, tr);

    if (pScene->mRootNode == nullptr)
        return false;

    return true;
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr)
{
    Matrix m;
    ai_real* temp = &node->mTransformation.a1;
    float* mTemp = &m._11;
    for (int t = 0; t < 16; t++)
        mTemp[t] = (float)temp[t];
    
    m = m.Transpose() * tr;

    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        // mMeshes[]의 원소는 정점들과 노멀, uv, 면의 개수(indices 포함), material index를 포함
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        auto newMesh = this->ProcessMesh(mesh, scene);

        for (auto& v : newMesh.vertices)
            v.pos = Vector3::Transform(v.pos, m);

        meshes.push_back(newMesh);
    }

    // Tree 구조
    for (UINT i = 0; i < node->mNumChildren; i++)
        this->ProcessNode(node->mChildren[i], scene, m);
}

MeshData ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // 데이터 저장
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // 각 Mesh의 vertex, normal, uv를 저장
    for (UINT i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // vertex의 position 저장
        vertex.pos.x = mesh->mVertices[i].x;
        vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;

        // normal vector 저장
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;
        vertex.normal.Normalize();

        // texture좌표
        if (mesh->mTextureCoords[0]) {
            vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    // 하나의 면(삼각형)은 3개의 index를 가지고 있다. (삼각형 메쉬)
    for (UINT i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i]; // 삼각형 메쉬
        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    MeshData newMesh;
    newMesh.vertices = vertices;
    newMesh.indices = indices;

    // 어떤 mesh에 어떤 texture를 사용할 지를 파일 이름으로 받아온다.
    // http://assimp.sourceforge.net/lib_html/materials.html
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString filepath;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);

            std::string fullPath =
                this->basePath +
                std::string(std::filesystem::path(filepath.C_Str())
                    .filename()
                    .string());

            newMesh.texturePath = fullPath;
        }
    }

    return newMesh;
}
