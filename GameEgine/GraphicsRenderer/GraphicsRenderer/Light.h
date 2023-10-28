#pragma once

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;

struct Light {
	// 16 byte 단위 맞춰줘야 제대로 GPU에 전달된다.
	Vector3 pos = Vector3(0.f, 0.f, -2.f);	// 조명의 위치
	float fallOffStart = 0.f;				// 빛의 중심과의 거리 시작점
	Vector3 dir = Vector3(0.f, 0.f, 1.f);	// 빛을 쏘는 방향
	float fallOffEnd = 10.f;				// 빛의 중심으로부터 가장 멀어 이후엔 빛을 받지않는다.
	Vector3 strength = Vector3(1.f);		// 빛의 세기
	float spotPower = 1.f;					// 빛의 집중세기

// TODO : 이 값들을 넣으면 제대로된 데이터가 복사가 안된다. (데이터 복사 자체는 된다)
//	int type = 1;							// 조명 타입 - Directional Light = 0, Point Light = 1, Spot Light = 2
//	float dummy[3];							// 16byte단위로 전송되기에 더미 데이터
};
