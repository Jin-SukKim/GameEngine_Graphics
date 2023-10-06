#pragma once

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;

struct Light {
	// 16 byte 단위 맞춰줘야 제대로 GPU에 전달된다.
	Vector3 pos;						// 조명의 위치
	float fallOffStart = 0.f;			// 빛의 중심과의 거리 시작점
	Vector3 dir;						// 빛을 쏘는 방향
	float fallOffEnd = 100.f;			// 빛의 중심으로부터 가장 멀어 이후엔 빛을 받지않는다.
	Vector3 strength = Vector3(1.f);	// 빛의 세기
	float spotPower = 100.f;			// 빛의 집중세기
	float lightType = 0;				// 조명 타입 - Directional Light = 0, Point Light = 1, Spot Light = 2
	float dummy[3];
};