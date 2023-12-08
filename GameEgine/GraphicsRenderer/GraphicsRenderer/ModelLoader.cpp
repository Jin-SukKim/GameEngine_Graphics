#include "ModelLoader.h"
#include <iostream>
using DirectX::SimpleMath::Matrix;

// https://heinleinsgame.tistory.com/21
bool ModelLoader::LoadModel(const std::string& filePath, const std::string& fileName)
{
    this->basePath = filePath;

    // Assimp Library 사용하기 위한 intreface
    Assimp::Importer importer;

    // 파일에서 모델 데이터 가져오기
    // Assimp의 데이터 구조인 scene 객체
    // aiScene은 데이터의 root로 모든 nodes, meshes, materials, animaions, textures에 접근 가능
    const aiScene* scene = importer.ReadFile(
        this->basePath + fileName,
        // Assimp는 default로 right-handed 좌표계를 사용하므로(OpenGL) 왼손 좌표계로 변환(DirectX)
        // aiProcess_ConvertToLeftHanded = 데이터 format을 directX에 맞게 변환시켜준다(index 순서, uv 좌표 등)
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded// 모든 메쉬를 삼각형으로 만든다.
    );

    // 데이터 가져오는데 실패했다면 (scene과 root node가 null인지 확인, 데이터가 불안전한지 확인)
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR : ASSIMP - " << importer.GetErrorString() << '\n';
        return false;
    }

    // 파일의 데이터에 접근
    // 한 모델은 여러 개의 서브 mesh들로 이루어져있다. (인간 : 머리, 몸통, 팔다리, 옷 등)
    Matrix tr;
    ProcessNode(scene->mRootNode, scene, tr);

    return true;
}

// scene의 노드들을 처리하기 위한 recursive 함수 (각 노드는 자식들을 가지고 있어 현재 노드를 처리하고 자식 노드를 처리)
void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr)
{
    Matrix m;
    // 노드의 Transformation 정보 가져오기
    ai_real* temp = &node->mTransformation.a1;
    float* mTemp = &m._11;
    for (int t = 0; t < 16; t++)
        mTemp[t] = (float)temp[t];
    
    // 각 노드의 Transformation 처리
    m = m.Transpose() * tr;

    // 노드의 모든 mesh들을 처리 (각 노드는 mesh index들의 모음을 가지고 있다.)
    // 각 index는 scene 객체 내부의 특정한 mesh를 가리킨다.
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        // scene의 mMeshes 배열에서 mesh들을 얻는다.
        // mMeshes[]의 원소는 정점들과 노멀, uv, 면의 개수(indices 포함), material index를 포함
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        // assimp의 mesh 데이터를 MeshData로 변환
        auto newMesh = ProcessMesh(mesh, scene);

        // 노드의 mesh를 transformation에 맞춰 적용
        for (auto& v : newMesh.vertices)
            v.pos = Vector3::Transform(v.pos, m);

        meshes.push_back(newMesh);
    }

    // Tree 구조 - 자식 노드들도 처리
    for (UINT i = 0; i < node->mNumChildren; i++)
        ProcessNode(node->mChildren[i], scene, m);
}

// scene의 mMeshes 배열에 있는 mesh 데이터를 반환 
MeshData ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // 데이터 저장
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // Mesh 데이터는 크게 vertex, indices, material 데이터 세 부분으로 이루어진다.
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
        if (mesh->mTextureCoords[0]) { // texture 좌표가 있는지 확인
            vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    // indices 처리
    // 각 mesh들이 face의 배열을 가지고 있도록 정의되어 있고 각 face들은 하나의 primitive(현재 삼각형)을
    // 나타내며 각 face는 vertex의 indices 정보를 가지고 있다.
    // 하나의 면(삼각형)은 3개의 index를 가지고 있다. (삼각형 메쉬)
    for (UINT i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i]; // 삼각형 메쉬
        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    MeshData newMesh;
    newMesh.vertices = vertices;
    newMesh.indices = indices;

    // material 처리 (mesh는 오직 material 객체의 index만 가지고 있어 scene의 mMaterial 배열에서
    // mesh의 material index 정보를 사용해 데이터를 가져와야 한다.)
    // 어떤 mesh에 어떤 texture를 사용할 지를 파일 이름으로 받아온다.
    // http://assimp.sourceforge.net/lib_html/materials.html

    if (mesh->mMaterialIndex >= 0) {
        // scene에서 material 데이터 가져오기
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // texture의 type들은 aiTextureType_ 접두사로 분류된다.
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
