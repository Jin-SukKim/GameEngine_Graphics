#pragma once
#include "Light.h"

// DirectX의 Math Library를 좀 더 간편하게 사용할 수 있는 Library
#include <directxtk/SimpleMath.h>

#define MAX_LIGHTS 3

using DirectX::SimpleMath::Matrix;

// Constant Data Structure

// 16 byte 단위 (float 4개) 단위로 전송
// Matrix = XMFLOAT4 = float 4개
struct MeshVSConstData {
	Matrix world;	 // Model을 world space 좌표로 변환
	Matrix invWorld; // 조명 효과를 위해 제대로 변환된 normal을 계산하기 위해 사용
	Matrix view;	 // View 좌표계로 변환
	Matrix proj;	 // Projection 좌표계로 변환
};

struct MeshPSConstData {
	Vector3 camWorld; // 시점의 월드 좌표계
	bool useTexture;
	Material material;
	Light lights[MAX_LIGHTS];
};

struct MeshNormalConstData {
	float scale = 0.05f;
	float dummy[3];
};