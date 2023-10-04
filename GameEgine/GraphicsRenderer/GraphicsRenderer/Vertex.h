#pragma once

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;

// Vertex Data
struct Vertex {
	Vector3 pos;
	Vector3 color = { 0.f, 0.f, 0.f };
	Vector3 normal;
	Vector2 texcoord;
};