#include "Camera.h"

#include <iostream>
Camera::Camera(Vector3 pos, Vector3 lookAt) : m_camPos(pos), m_camLookAt(lookAt)
{
    // ī�޶� ���� ���� - ī�޶� ���� ����, �ɾ�� ����
    m_camDir = m_camLookAt - m_camPos;
    m_camDir.Normalize();
    
    // �ӽ� up vector
    m_camUp = { 0.f, 1.f, 0.f };
    // ī�޶� ������ vector - ������ ����, eyeDir�� upDir�κ��� ���
    Vector3 m_camRight = m_camUp.Cross(m_camDir);
    m_camRight.Normalize();

    // Up Vector
    m_camUp = m_camDir.Cross(m_camRight);
    m_camUp.Normalize();

    m_pitch = 0.f;	// x-��
    m_yaw = 0.f;	// y-��
    m_roll = 0.f;	// z-��

    m_camSpeed = 1.f; // ī�޶� �ӵ�

    m_fovAngleY = 70.f;		 // �þ߰�
    m_nearZ = 0.01f;		 // viewing-frustum�� near plane
    m_farZ = 100.f;		 // viewing-frustum�� far plane
    m_aspect = 16.f / 9.f; // �������� ����� ����

    // ���콺 ����
    m_sensitivityX = 3.f;
    m_sensitivityY = 1.f;

    // ���� ������ ����� ������
    m_usePerspectiveProjection = true;
}

Matrix Camera::GetFPPViewRowMatrix()
{
    // View ��ǥ�迡���� ���� ������ +z, �������� +x, ������ +y�̴�.
    // ���� ��ȯ�� ���� ���谡 ��°�� �ݴ�� �����̴� �Ͱ� ����.
    return Matrix::CreateRotationX(-m_pitch)
        * Matrix::CreateRotationY(-m_yaw)
        * Matrix::CreateTranslation(-m_camPos);
}

Matrix Camera::GetProjRowMatrix()
{
    // ���� ���
    if (m_usePerspectiveProjection)
    {
        // ���� ����
        // (�þ߰�, ȭ�����, Near-Plane, Far-Plane) - �þ߰��� radian�� ����ϱ⿡ ������ ��ȯ���ش�. 
        return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_fovAngleY), m_aspect, m_nearZ, m_farZ);
    }
    else
    {
        // ������
        // (x ���� ���� ���� 2��, y ���� ���� ���� 2��, Near-Plane, Far-Plane)
        return DirectX::XMMatrixOrthographicOffCenterLH(-m_aspect, m_aspect, -1.0f, 1.0f, m_nearZ, m_farZ);
    }
}

Matrix Camera::GetFocusViewRowMatrix()
{
    // (ī�޶� ��ġ, �ٶ󺸴� ��ġ(����), ī�޶��� upVector)    
    return DirectX::XMMatrixLookAtLH(
        m_camPos, m_camLookAt, m_camUp);
}

// ȸ���ϴ� ���� �ƴ϶�� ���� ���͵��� ������Ʈ�� �ʿ����.
void Camera::MoveForward(float dt)
{
    // ��ǻ�͸��� �ӵ��� �ٸ��⿡ �ð���(delta time)�� �����ش�.
    m_camPos += m_camDir * m_camSpeed * dt;
    m_camLookAt += m_camDir * m_camSpeed * dt;
}

void Camera::MoveRight(float dt)
{
    m_camPos += m_camRight * m_camSpeed * dt;
    m_camLookAt += m_camRight * m_camSpeed * dt;
}

void Camera::MoveUp(float dt)
{
    m_camPos += m_camUp * m_camSpeed * dt;
    m_camLookAt += m_camUp * m_camSpeed * dt;
}

// TODO: Rotation doesn't work properly
void Camera::MouseRotate(float mouseNdcX, float mouseNdcY)
{
    
    // NDC��ǥ�� ���� [-1, 1]�� ȸ�� �ݰ� ����
    m_yaw = mouseNdcX * m_sensitivityX; // ��/�Ʒ� ȸ��
    // mouseNdcY�� -���� �� �Ʒ��� ȸ���ϰ� +�� ���� ȸ���Ϸ���
    // -�� �ٿ��� �������ش�.
    m_pitch = -mouseNdcY * m_sensitivityY; // �¿� ȸ��

    Matrix rot = Matrix::CreateRotationX(m_yaw) * Matrix::CreateRotationY(m_pitch);
    m_camLookAt = Vector3::Transform(m_camLookAt, rot);
    m_camDir = m_camLookAt - m_camPos;
    m_camDir.Normalize();
    m_camUp = Vector3::Transform(m_camUp, rot);
    m_camUp.Normalize();
    m_camRight = m_camUp.Cross(m_camDir);
    m_camRight.Normalize();
}

void Camera::SetAspectRatio(float aspect)
{
    m_aspect = aspect;
}

void Camera::SetCameraSpeed(float speed)
{
    m_camSpeed = speed;
}

void Camera::SetFovAngle(float angle)
{
    m_fovAngleY = angle;
}

Vector3 Camera::GetCameraPos()
{
    return m_camPos;
}

void Camera::SetPerspectiveProjection(bool usePerspective)
{
    m_usePerspectiveProjection = usePerspective;
}
