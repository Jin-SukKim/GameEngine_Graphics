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
	Matrix model;	 // Model�� world space ��ǥ�� ��ȯ
	/*
		normal vector�� �����̱⿡ �̵��� ���� ������ scale�� roation�� ������ �޴´�.
		normal vector�� ȸ���ؾ� �ϱ⿡ ������ ���� ������ scale�� ������ �ȴ�.
		�׷��� model ����� Inverse Transpose�� �����ϸ� normal vector�� �������� ��ȯ�� �� �ִ�.
	*/ 
	// normal vector�� non-uniform scale�� ����Ǹ� ���� ����� ������ �ƴϰ� �ȴ�.
	Matrix invTranspose; // ���� ȿ���� ���� ����� ��ȯ�� normal�� ����ϱ� ���� ��� (���� ��ǥ�迡�� �� ��ǥ��� ��ȯ�ϱ� ���� ���)
	/*
	Matrix view;	 // View ��ǥ��� ��ȯ
	Matrix proj;	 // Projection ��ǥ��� ��ȯ
	*/
	Matrix viewProj; // view�� proj ��ǥ�� ��ȯ�� �ѹ��� ���� (CPU -> GPU ���縦 �ּ�ȭ�ϴ� ���� ����)
};
static_assert((sizeof(MeshVSConstData) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct MeshPSConstData {
	Vector3 camWorld; // ������ ���� ��ǥ��
	bool useTexture; // C++���� bool�� 1 byte���� HLSL���� 4 byte�� padding�� ���� ǥ���ȴ�.
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
