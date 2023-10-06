#pragma once
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

// 시점 변환 - 원리 = 시점이 움직이는 반대로 세상의 모든 모델을 움직인다.
class Camera {
public:
	Camera(Vector3 pos = { 0.0f, 0.0f, -1.f }, Vector3 lookAt = { 0.0f, 0.0f, 0.0f });
	// 1인칭 시점 Control을 사용할 때 view 행렬
	Matrix GetFPPViewRowMatrix();
	// projection 행렬
	Matrix GetProjRowMatrix();
	// XMMatrixLookAtLH를 활용한 view 행렬 생성
	Matrix GetFocusViewRowMatrix();
	
	// 카메라 앞/뒤 움직임
	void MoveForward(float dt);
	// 카메라 좌/우 움직임
	void MoveRight(float dt);
	// 카메라 상/하 움직임
	void MoveUp(float dt);
	// 마우스 움직임에 따른 카메라 회전
	void MouseRotate(float mouseNdcX, float mouseNdcY);

	// 화면비율 설정
	void SetAspectRatio(float aspect);
	// 카메라 움직임 속도 설정
	void SetCameraSpeed(float speed);
	// 시야각 설정
	void SetFovAngle(float angle);
	
	// 카메라 위치
	Vector3 GetCameraPos();

	// 원근투영을 사용할 지 직교투영을 사용할 지 설정
	void SetPerspectiveProjection(bool usePerspective);

private:
	// 월드 좌표계에서 카메라의 위치
	Vector3 m_camPos;
	// 카메라가 보는 위치
	Vector3 m_camLookAt;
	// 카메라 시점 방향 - 카메라가 보는 방향, 걸어가는 방향
	Vector3 m_camDir = { 0.f, 0.f, 1.f };
	// 카메라 up vector - 위쪽 방향, 중력의 반대방향이 기본
	Vector3 m_camUp = { 0.f, 1.f, 0.f };
	// 카메라 오른쪽 vector - 오른쪽 방향, eyeDir과 upDir로부터 계산
	Vector3 m_camRight = { 1.f, 0.f, 0.f };

	float m_pitch;	// x-축
	float m_yaw;	// y-축
	float m_roll;	// z-축

	float m_camSpeed = 1.f; // 카메라 속도
	
	float m_fovAngleY = 70.f;		 // 시야각
	float m_nearZ = 0.01f;		 // viewing-frustum의 near plane
	float m_farZ = 100.f;		 // viewing-frustum의 far plane
	float m_aspect = 16.f / 9.f; // 대중적인 모니터 비율

	// 마우스 감도
	float m_sensitivityX = 3.f;
	float m_sensitivityY = 1.f;

	// 원근 투영을 사용할 것인지
	bool m_usePerspectiveProjection = true;
};
