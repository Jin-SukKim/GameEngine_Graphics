#pragma once
#include <vector>
#include <algorithm>

#include "MeshData.h"

class GeometryGenerator {
public:
	// 정육면체 생성
	static MeshData MakeCube();
	// 삼각형 생성
	static MeshData MakeTriangle();
	// 사각형 생성
	static MeshData MakeSqaure();
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

	static MeshData SubdivideToSphere(const float& radius, MeshData sphere);
};