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