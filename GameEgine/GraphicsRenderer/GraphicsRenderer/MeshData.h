#pragma once
#include <vector>

#include "Vertex.h"

// Mesh Data
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
};