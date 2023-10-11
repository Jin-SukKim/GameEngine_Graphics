#pragma once
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

// ���� ��ȯ - ���� = ������ �����̴� �ݴ�� ������ ��� ���� �����δ�.
class Camera {
public:
	Camera(Vector3 pos = { 0.0f, 0.0f, -1.f }, Vector3 lookAt = { 0.0f, 0.0f, 0.0f });
	// 1��Ī ���� Control�� ����� �� view ���
	Matrix GetFPPViewRowMatrix();
	// projection ���
	Matrix GetProjRowMatrix();
	// XMMatrixLookAtLH�� Ȱ���� view ��� ����
	Matrix GetFocusViewRowMatrix();
	
	// ī�޶� ��/�� ������
	void MoveForward(float dt);
	// ī�޶� ��/�� ������
	void MoveRight(float dt);
	// ī�޶� ��/�� ������
	void MoveUp(float dt);
	// ���콺 �����ӿ� ���� ī�޶� ȸ��
	void MouseRotate(float mouseNdcX, float mouseNdcY);

	// ȭ����� ����
	void SetAspectRatio(float aspect);
	// ī�޶� ������ �ӵ� ����
	void SetCameraSpeed(float speed);
	// �þ߰� ����
	void SetFovAngle(float angle);
	
	// ī�޶� ��ġ
	Vector3 GetCameraPos();

	// ���������� ����� �� ���������� ����� �� ����
	void SetPerspectiveProjection(bool usePerspective);

private:
	// ���� ��ǥ�迡�� ī�޶��� ��ġ
	Vector3 m_camPos;
	// ī�޶� ���� ��ġ
	Vector3 m_camLookAt;
	// ī�޶� ���� ���� - ī�޶� ���� ����, �ɾ�� ����
	Vector3 m_camDir = { 0.f, 0.f, 1.f };
	// ī�޶� up vector - ���� ����, �߷��� �ݴ������ �⺻
	Vector3 m_camUp = { 0.f, 1.f, 0.f };
	// ī�޶� ������ vector - ������ ����, eyeDir�� upDir�κ��� ���
	Vector3 m_camRight = { 1.f, 0.f, 0.f };

	float m_pitch;	// x-��
	float m_yaw;	// y-��
	float m_roll;	// z-��

	float m_camSpeed = 1.f; // ī�޶� �ӵ�
	
	float m_fovAngleY = 70.f;		 // �þ߰�
	float m_nearZ = 0.01f;		 // viewing-frustum�� near plane
	float m_farZ = 100.f;		 // viewing-frustum�� far plane
	float m_aspect = 16.f / 9.f; // �������� ����� ����

	// ���콺 ����
	float m_sensitivityX = 3.f;
	float m_sensitivityY = 1.f;

	// ���� ������ ����� ������
	bool m_usePerspectiveProjection = true;
};
