#pragma once
#include "Light.h"

// DirectX�� Math Library�� �� �� �����ϰ� ����� �� �ִ� Library
#include <directxtk/SimpleMath.h>

#define MAX_LIGHTS 3

using DirectX::SimpleMath::Matrix;

// Constant Data Structure

// 16 byte ���� (float 4��) ������ ����
// Matrix = XMFLOAT4 = float 4��
struct MeshVSConstData {
	Matrix world;	 // Model�� world space ��ǥ�� ��ȯ
	Matrix invWorld; // ���� ȿ���� ���� ����� ��ȯ�� normal�� ����ϱ� ���� ���
	Matrix view;	 // View ��ǥ��� ��ȯ
	Matrix proj;	 // Projection ��ǥ��� ��ȯ
};
static_assert((sizeof(MeshVSConstData) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct MeshPSConstData {
	Vector3 camWorld; // ������ ���� ��ǥ��
	bool useTexture;
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

struct CubeVSConstData {
	Matrix viewProj; // ���� View, Projection ����� �̸� ���ؼ� ����Ѵ�.
};