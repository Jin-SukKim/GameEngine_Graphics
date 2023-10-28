#pragma once

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;

struct Light {
	// 16 byte ���� ������� ����� GPU�� ���޵ȴ�.
	Vector3 pos = Vector3(0.f, 0.f, -2.f);	// ������ ��ġ
	float fallOffStart = 0.f;				// ���� �߽ɰ��� �Ÿ� ������
	Vector3 dir = Vector3(0.f, 0.f, 1.f);	// ���� ��� ����
	float fallOffEnd = 10.f;				// ���� �߽����κ��� ���� �־� ���Ŀ� ���� �����ʴ´�.
	Vector3 strength = Vector3(1.f);		// ���� ����
	float spotPower = 1.f;					// ���� ���߼���

// TODO : �� ������ ������ ����ε� �����Ͱ� ���簡 �ȵȴ�. (������ ���� ��ü�� �ȴ�)
//	int type = 1;							// ���� Ÿ�� - Directional Light = 0, Point Light = 1, Spot Light = 2
//	float dummy[3];							// 16byte������ ���۵Ǳ⿡ ���� ������
};
