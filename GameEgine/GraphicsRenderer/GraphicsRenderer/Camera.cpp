#include "Camera.h"

#include <iostream>
Camera::Camera(Vector3 pos) : m_camPos(pos) {}

Matrix Camera::GetFPPViewRowMatrix()
{
    // View ��ǥ�迡���� ���� ������ +z, �������� +x, ������ +y�̴�.
    // ���� ��ȯ�� ���� ���谡 ��°�� �ݴ�� �����̴� �Ͱ� ����.
    return Matrix::CreateTranslation(-m_camPos)
        * Matrix::CreateRotationY(-m_yaw)
        * Matrix::CreateRotationX(m_pitch);
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

// ȸ���ϴ� ���� �ƴ϶�� ���� ���͵��� ������Ʈ�� �ʿ����.
void Camera::MoveForward(float dt)
{
    // ��ǻ�͸��� �ӵ��� �ٸ��⿡ �ð���(delta time)�� �����ش�.
    m_camPos += m_camDir * m_camSpeed * dt;
}

void Camera::MoveRight(float dt)
{
    m_camPos += m_camRight * m_camSpeed * dt;
}

void Camera::MoveUp(float dt)
{
    m_camPos += m_camUp * m_camSpeed * dt;
}

// TODO: Rotation doesn't work properly
void Camera::MouseRotate(float mouseNdcX, float mouseNdcY)
{
    
    // NDC��ǥ�� ���� [-1, 1]�� ȸ�� �ݰ� ����
    // �¿� ȸ�� (y ���� �߽����� ȸ��) - �¿� 360�� ȸ��
    m_yaw = mouseNdcX * m_sensitivityX * DirectX::XM_2PI; 
    // mouseNdcY�� -���� �� �Ʒ��� ȸ���ϰ� +�� ���� ȸ���Ϸ��� -�� �ٿ��� �������ش�.
    // ��ũ�� ��ǥ��� y���� �Ʒ��� ������ Ŀ���⿡ -�� ���������ִ� ���̴�.
    // ��/�Ʒ� ȸ��(z ���� �߽����� ȸ��) - �� �Ʒ� 90�� ȸ��
    m_pitch = mouseNdcY * m_sensitivityY * DirectX::XM_PIDIV2; 

    Matrix rot = Matrix();
    if (m_yaw)
        rot *= Matrix::CreateRotationX(m_yaw);
    if (m_pitch) 
        rot *= Matrix::CreateRotationY(m_pitch);

    m_camDir = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f),
        Matrix::CreateRotationY(m_yaw));
    m_camUp = Vector3::Transform({ 0.f, 1.f, 0.f }, Matrix::CreateRotationY(m_pitch));
    m_camRight = m_camUp.Cross(m_camDir);

    //m_camLookAt = Vector3::Transform(m_camLookAt, rot);
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
