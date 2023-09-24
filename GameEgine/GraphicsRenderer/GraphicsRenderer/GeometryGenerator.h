#pragma once
#include <vector>
#include <algorithm>

#include "MeshData.h"

class GeometryGenerator {
public:
	static MeshData MakeCube();
	static MeshData MakeTriangle();
	static MeshData MakeSqaure();
	static MeshData MakeCircle(const float& radius = 1.f, const int& numTriangles = 50, const Vector3& color = Vector3(1.f, 1.f, 1.f));
	static MeshData MakeGrid(const float& width = 1.f, const float& height = 1.f, const int& numSlices = 1, const int& numStacks = 1);
};