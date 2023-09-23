#pragma once

// DirectX�� Math Library�� �� �� �����ϰ� ����� �� �ִ� Library
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Matrix;

// Constant Data Structure

// 16 byte ���� (float 4��) ������ ����
// Matrix = XMFLOAT4 = float 4��
struct MeshConstData {
	Matrix world;
	Matrix view;
	Matrix proj;
};