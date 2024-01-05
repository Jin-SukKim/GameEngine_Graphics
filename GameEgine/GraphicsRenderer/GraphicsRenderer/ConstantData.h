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
	Matrix model;	 // Model을 world space 좌표로 변환
	/*
		normal vector는 벡터이기에 이동은 되지 않지만 scale과 roation엔 영향을 받는다.
		normal vector도 회전해야 하기에 문제가 되지 않지만 scale은 문제가 된다.
		그래서 model 행렬의 Inverse Transpose를 적용하면 normal vector를 수직으로 변환할 수 있다.
	*/ 
	// normal vector에 non-uniform scale이 적용되면 모델의 포면과 수직이 아니게 된다.
	Matrix invTranspose; // 조명 효과를 위해 제대로 변환된 normal을 계산하기 위해 사용 (월드 좌표계에서 모델 좌표계로 변환하기 위한 행렬)
	/*
	Matrix view;	 // View 좌표계로 변환
	Matrix proj;	 // Projection 좌표계로 변환
	*/
	Matrix viewProj; // view와 proj 좌표계 변환을 한번에 수행 (CPU -> GPU 복사를 최소화하는 것이 좋다)
};
static_assert((sizeof(MeshVSConstData) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct MeshPSConstData {
	Vector3 camWorld; // 시점의 월드 좌표계
	bool useTexture; // C++에선 bool이 1 byte지만 HLSL에선 4 byte로 padding이 합쳐 표현된다.
	Material material;
	Light lights[MAX_LIGHTS];
	RimLight rimLight;
};
static_assert((sizeof(MeshPSConstData) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct MeshNormalConstData {
	float scale = 0.05f;
	float dummy[3];
};
static_assert((sizeof(MeshNormalConstData) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");
