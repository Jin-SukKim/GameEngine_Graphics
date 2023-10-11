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

struct MeshPSConstData {
	Vector3 camWorld; // ������ ���� ��ǥ��
	bool useTexture;
	Material material;
	Light lights[MAX_LIGHTS];
};

struct MeshNormalConstData {
	float scale = 0.05f;
	float dummy[3];
};