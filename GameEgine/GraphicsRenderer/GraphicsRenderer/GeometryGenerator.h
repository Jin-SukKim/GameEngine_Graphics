#pragma once
#include <vector>
#include <algorithm>

#include "MeshData.h"

class GeometryGenerator {
public:
	// ������ü ����
	static MeshData MakeCube(const float scale = 1.f);
	// �ﰢ�� ����
	static MeshData MakeTriangle(const float scale = 1.f);
	// �簢�� ����
	static MeshData MakeSquare(const float scale = 1.f);
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
	// �ϳ��� �ﰢ�� �޽��� ���� �ﰢ������ ������ �Լ�
	static MeshData SubdivideToSphere(const float& radius, MeshData sphere);
	static std::vector<MeshData> ReadFromFile(const std::string& basePath, const std::string& fileName);
};