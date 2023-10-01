#pragma once
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

// 시점 변환 - 원리 = 시점이 움직이는 반대로 세상의 모든 모델을 움직인다.
class Camera {
public:
	// 1인칭 시점 Control을 사용할 때 view 행렬
	Matrix GetFPPViewRowMatrix();
	Matrix GetProjRowMatrix();
	Matrix GetFocusViewRowMatrix(float focusDis = 1.f);
	
	void MoveForward(float dt);
	void MoveRight(float dt);
	void UpdateMouse(float mouseX, float mouseY);
	
	void SetAspectRatio(float aspect);

	void SetFovAngle(float angle);
	void SetCameraTo(Vector3 pos, Vector3 dir, Vector3 up);
	Vector3 GetCameraPos();

	void SetPerspectiveProjection(bool usePerspective);

private:
	// 월드 좌표계에서 카메라의 위치
	Vector3 m_camPos = { 0.0f, 0.0f, -3.f };
	// 카메라 시점 방향 - 카메라가 보는 방향, 걸어가는 방향
	Vector3 m_camDir = { 0.f, 0.f, 1.f };
	// 카메라 up vector - 위쪽 방향, 중력의 반대방향이 기본
	Vector3 m_camUp = { 0.f, 1.f, 0.f };
	// 카메라 오른쪽 vector - 오른쪽 방향, eyeDir과 upDir로부터 계산
	Vector3 m_camRight;	

	float m_pitch;	// x-축
	float m_yaw;	// y-축
	float m_roll;	// z-축

	float speed = 1.f; // 카메라 속도
	
	float m_fovAngleY = 70.f;		 // 시야각
	float m_nearZ = 0.01f;		 // viewing-frustum의 near plane
	float m_farZ = 100.f;		 // viewing-frustum의 far plane
	float m_aspect = 16.f / 9.f; // 대중적인 모니터 비율

	// 원근 투영을 사용할 것인지
	bool m_usePerspectiveProjection = true;
};
