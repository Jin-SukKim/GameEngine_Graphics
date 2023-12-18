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


// 재질 - 재질에 사용되는 색의 값은 보통은 Texture에서 가져온다.
struct Material {
	Vector3 ambient = Vector3(0.1f);	// 물체의 고유 색 - default (옅은 흰색)
	float shininess = 8.f;				// 물체의 메탈 강도(반짝임의 강도)
	Vector3 diffuse = Vector3(0.f);		// 분산광 - 빛을 받아 색을 표현
	float dummy1;						// 16byte단위로 전송되기에 더미 데이터
	Vector3 specular = Vector3(1.f);	// 반사광 - 메탈릭한 재질은 반짝이는 효과 - default(흰색)
	float dummy2;						// 16byte단위로 전송되기에 더미 데이터
	Vector3 fresnelR0 = Vector3(1.0f, 0.71f, 0.29f); // Gold
	float dummy3;
};