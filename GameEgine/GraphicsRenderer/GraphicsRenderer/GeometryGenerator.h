#pragma once
#include <vector>
#include <algorithm>

#include "MeshData.h"

class GeometryGenerator {
public:
	// 정육면체 생성
	static MeshData MakeCube(const float scale = 1.f);
	// 삼각형 생성
	static MeshData MakeTriangle(const float scale = 1.f);
	// 사각형 생성
	static MeshData MakeSquare(const float scale = 1.f);
	// 여러 삼각형으로 원 생성
	static MeshData MakeCircle(const float& radius = 1.f, const int& numTriangles = 50, const Vector3& color = Vector3(1.f, 1.f, 1.f));
	// Grid 생성
	static MeshData MakeGrid(const float& width = 1.f, const float& height = 1.f, const int& numSlices = 1, const int& numStacks = 1);
	// Cylinder 생성
	static MeshData MakeCylinder(const float& topRadius = 1.f, const float& bottomRadius = 1.f, const float& height = 1.f, const int& numSlices = 3);
	// Sphere 생성
	static MeshData MakeSphere(const float& radius = 1.f, const int& numSlices = 4, const int& numStacks = 4);
	// 또 다른 Sphere 생성 방법
	static MeshData MakeIcosahedron();
	static MeshData MakeTetrahedron();
	// 하나의 삼각형 메쉬를 여러 삼각형으로 나누는 함수
	static MeshData SubdivideToSphere(const float& radius, MeshData sphere);
	static std::vector<MeshData> ReadFromFile(const std::string& basePath, const std::string& fileName);
};