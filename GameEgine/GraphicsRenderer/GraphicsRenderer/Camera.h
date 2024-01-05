#pragma once
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

// 시점 변환 - 원리 = 시점이 움직이는 반대로 세상의 모든 모델을 움직인다.
class Camera {
public:
	Camera(Vector3 pos = { 0.f, 0.f, 0.f });
	// 1인칭 시점 Control을 사용할 때 view 행렬
	Matrix GetFPPViewRowMatrix();
	// projection 행렬
	Matrix GetProjRowMatrix();
	
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
	// 카메라 좌표계에서 카메라의 위치는 {0.f, 0.f, 0.f}에 있다.
	// 월드 좌표계에서 카메라의 위치
	Vector3 m_camPos = { 0.0f, 0.0f, 0.f };
	// 카메라 시점 방향 - 카메라가 보는 방향, 걸어가는 방향
	Vector3 m_camDir = { 0.f, 0.f, 1.f };
	// 카메라 up vector - 위쪽 방향, 중력의 반대방향이 기본
	// 곱하는 순서 조심하기 (dir.Cross(right))
	Vector3 m_camUp = { 0.f, 1.f, 0.f };
	// 카메라 오른쪽 vector - 오른쪽 방향, eyeDir과 upDir로부터 계산
	// 곱하는 순서 조심하기 (up.Cross(dir))
	Vector3 m_camRight = { 1.f, 0.f, 0.f };

	float m_pitch = 0.f;// x-축 - 위 아래 회전
	float m_yaw = 0.f;	// y-축 - 좌우 회전
	float m_roll = 0.f;	// z-축

	float m_camSpeed = 1.f; // 카메라 속도
	
	float m_fovAngleY = 70.f;	 // 시야각
	float m_nearZ = 0.01f;		 // viewing-frustum의 near plane
	float m_farZ = 100.f;		 // viewing-frustum의 far plane
	float m_aspect = 16.f / 9.f; // 대중적인 모니터 비율

	// 마우스 감도
	float m_sensitivityX = 1.f;
	float m_sensitivityY = 1.f;

	// 원근 투영을 사용할 것인지
	bool m_usePerspectiveProjection = true;
};
