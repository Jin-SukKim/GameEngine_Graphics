#pragma once
#include <vector>
#include <string>

#include "Vertex.h"

// Mesh Data
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::string texturePath;
};


// ���� - ������ ���Ǵ� ���� ���� ������ Texture���� �����´�.
struct Material {
	Vector3 ambient = Vector3(0.1f);	// ��ü�� ���� �� - default (���� ���)
	float shininess = 8.f;				// ��ü�� ��Ż ����(��¦���� ����)
	Vector3 diffuse = Vector3(0.f);		// �л걤 - ���� �޾� ���� ǥ��
	float dummy1;						// 16byte������ ���۵Ǳ⿡ ���� ������
	Vector3 specular = Vector3(1.f);	// �ݻ籤 - ��Ż���� ������ ��¦�̴� ȿ�� - default(���)
	float dummy2;						// 16byte������ ���۵Ǳ⿡ ���� ������
	Vector3 fresnelR0 = Vector3(1.0f, 0.71f, 0.29f); // Gold
	float dummy3;
};