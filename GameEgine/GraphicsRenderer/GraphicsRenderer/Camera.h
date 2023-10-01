#pragma once
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

// ���� ��ȯ - ���� = ������ �����̴� �ݴ�� ������ ��� ���� �����δ�.
class Camera {
public:
	// 1��Ī ���� Control�� ����� �� view ���
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
	// ���� ��ǥ�迡�� ī�޶��� ��ġ
	Vector3 m_camPos = { 0.0f, 0.0f, -3.f };
	// ī�޶� ���� ���� - ī�޶� ���� ����, �ɾ�� ����
	Vector3 m_camDir = { 0.f, 0.f, 1.f };
	// ī�޶� up vector - ���� ����, �߷��� �ݴ������ �⺻
	Vector3 m_camUp = { 0.f, 1.f, 0.f };
	// ī�޶� ������ vector - ������ ����, eyeDir�� upDir�κ��� ���
	Vector3 m_camRight;	

	float m_pitch;	// x-��
	float m_yaw;	// y-��
	float m_roll;	// z-��

	float speed = 1.f; // ī�޶� �ӵ�
	
	float m_fovAngleY = 70.f;		 // �þ߰�
	float m_nearZ = 0.01f;		 // viewing-frustum�� near plane
	float m_farZ = 100.f;		 // viewing-frustum�� far plane
	float m_aspect = 16.f / 9.f; // �������� ����� ����

	// ���� ������ ����� ������
	bool m_usePerspectiveProjection = true;
};
