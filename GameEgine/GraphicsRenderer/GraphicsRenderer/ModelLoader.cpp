#include "ModelLoader.h"
#include <iostream>
using DirectX::SimpleMath::Matrix;

bool ModelLoader::Load(const std::string& filePath, const std::string& fileName)
{
    this->basePath = filePath;

    // Assimp Library ����ϱ� ���� intreface
    Assimp::Importer importer;

    // ���Ͽ��� �� ������ ��������
    // aiScene�� �������� root�� ��� nodes, meshes, materials, animaions, textures�� ���� ����
    const aiScene* pScene = importer.ReadFile(
        this->basePath + fileName,
        // Assimp�� default�� right-handed ��ǥ�踦 ����ϹǷ�(OpenGL) �޼� ��ǥ��� ��ȯ(DirectX)
        // aiProcess_ConvertToLeftHanded = ������ format�� directX�� �°� ��ȯ�����ش�(index ����, uv ��ǥ ��)
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded// ��� �޽��� �ﰢ������ �����.
    );

    // ������ �������µ� �����ߴٸ�
    if (pScene == nullptr) {
        std::cout << importer.GetErrorString() << '\n';
        return false;
    }

    // ������ �����Ϳ� ����
    // �� ���� ���� ���� ���� mesh��� �̷�����ִ�. (�ΰ� : �Ӹ�, ����, �ȴٸ�, �� ��)
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
        // mMeshes[]�� ���Ҵ� ������� ���, uv, ���� ����(indices ����), material index�� ����
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        auto newMesh = this->ProcessMesh(mesh, scene);

        for (auto& v : newMesh.vertices)
            v.pos = Vector3::Transform(v.pos, m);

        meshes.push_back(newMesh);
    }

    // Tree ����
    for (UINT i = 0; i < node->mNumChildren; i++)
        this->ProcessNode(node->mChildren[i], scene, m);
}

MeshData ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // ������ ����
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // �� Mesh�� vertex, normal, uv�� ����
    for (UINT i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // vertex�� position ����
        vertex.pos.x = mesh->mVertices[i].x;
        vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;

        // normal vector ����
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;
        vertex.normal.Normalize();

        // texture��ǥ
        if (mesh->mTextureCoords[0]) {
            vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    // �ϳ��� ��(�ﰢ��)�� 3���� index�� ������ �ִ�. (�ﰢ�� �޽�)
    for (UINT i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i]; // �ﰢ�� �޽�
        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    MeshData newMesh;
    newMesh.vertices = vertices;
    newMesh.indices = indices;

    // � mesh�� � texture�� ����� ���� ���� �̸����� �޾ƿ´�.
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
