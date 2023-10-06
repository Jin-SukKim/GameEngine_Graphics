#pragma once

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;

struct Light {
	// 16 byte ���� ������� ����� GPU�� ���޵ȴ�.
	Vector3 pos;						// ������ ��ġ
	float fallOffStart = 0.f;			// ���� �߽ɰ��� �Ÿ� ������
	Vector3 dir;						// ���� ��� ����
	float fallOffEnd = 100.f;			// ���� �߽����κ��� ���� �־� ���Ŀ� ���� �����ʴ´�.
	Vector3 strength = Vector3(1.f);	// ���� ����
	float spotPower = 100.f;			// ���� ���߼���
	float lightType = 0;				// ���� Ÿ�� - Directional Light = 0, Point Light = 1, Spot Light = 2
	float dummy[3];
};