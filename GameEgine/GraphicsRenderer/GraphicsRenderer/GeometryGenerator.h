#pragma once
#include <vector>

#include "MeshData.h"

class GeometryGenerator {
public:
	static MeshData MakeCube(const float scale = 1.0f);
	static MeshData MakeTriangle(const float scale = 1.0f);
};