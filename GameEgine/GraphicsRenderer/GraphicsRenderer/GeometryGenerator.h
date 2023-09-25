#pragma once
#include <vector>
#include <algorithm>

#include "MeshData.h"

class GeometryGenerator {
public:
	// ������ü ����
	static MeshData MakeCube();
	// �ﰢ�� ����
	static MeshData MakeTriangle();
	// �簢�� ����
	static MeshData MakeSqaure();
	// ���� �ﰢ������ �� ����
	static MeshData MakeCircle(const float& radius = 1.f, const int& numTriangles = 50, const Vector3& color = Vector3(1.f, 1.f, 1.f));
	// Grid ����
	static MeshData MakeGrid(const float& width = 1.f, const float& height = 1.f, const int& numSlices = 1, const int& numStacks = 1);
	// Cylinder ����
	static MeshData MakeCylinder(const float& topRadius = 1.f, const float& bottomRadius = 1.f, const float& height = 1.f, const int& numSlices = 3);
	// Sphere ����
	static MeshData MakeSphere(const float& radius = 1.f, const int& numSlices = 4, const int& numStacks = 4);
	// �� �ٸ� Sphere ���� ���
	static MeshData MakeIcosahedron();
	static MeshData MakeTetrahedron();

	static MeshData SubdivideToSphere(const float& radius, MeshData sphere);
};