#pragma once

// DirectX의 Math Library를 좀 더 간편하게 사용할 수 있는 Library
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Matrix;

// Constant Data Structure

// 16 byte 단위 (float 4개) 단위로 전송
// Matrix = XMFLOAT4 = float 4개
struct MeshConstData {
	Matrix world;	// Model을 world space 좌표로 변환
	Matrix view;	// View 좌표계로 변환
	Matrix proj;	// Projection 좌표계로 변환
};