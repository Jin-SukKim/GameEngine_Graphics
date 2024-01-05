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

// 물질의 고유 성질 (기본)
static Vector3 Fresnel[] = {
	{0.02f, 0.02f, 0.02f}, // Water
	{0.08f, 0.08f, 0.08f}, // Glass
	{0.05f, 0.05f, 0.05f}, // Plastic
	{1.0f, 0.71f, 0.29f}, // Gold
	{0.95f, 0.93f, 0.88f}, // Silver
	{0.95f, 0.64f, 0.54f}, // Copper
};

// 재질 - 재질에 사용되는 색의 값은 보통은 Texture에서 가져온다.
struct Material {
	Vector3 ambient = Vector3(0.1f);	// 물체의 고유 색 - default (옅은 흰색)
	float shininess = 8.f;				// 물체의 메탈 강도(반짝임의 강도)
	Vector3 diffuse = Vector3(0.f);		// 분산광 - 빛을 받아 색을 표현
	float dummy1;						// 16byte단위로 전송되기에 더미 데이터
	Vector3 specular = Vector3(1.f);	// 반사광 - 메탈릭한 재질은 반짝이는 효과 - default(흰색)
	float dummy2;						// 16byte단위로 전송되기에 더미 데이터
	Vector3 fresnelR0 = Fresnel[3];
	float dummy3;
};