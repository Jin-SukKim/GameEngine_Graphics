#include "Camera.h"

#include <iostream>
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

Matrix Camera::GetFocusViewRowMatrix(float focusDis)
{
    // (ī�޶� ��ġ, �ٶ󺸴� ��ġ(����), ī�޶��� upVector)    
    return DirectX::XMMatrixLookAtLH(
        m_camPos, m_camDir * focusDis, m_camUp);
}

void Camera::MoveForward(float dt)
{
    // ��ǻ�͸��� �ӵ��� �ٸ��⿡ �ð���(delta time)�� �����ش�.
    m_camPos += m_camDir * m_camSpeed * dt;
}

// TODO: right/left movement is weird (check it later)
void Camera::MoveRight(float dt)
{
    m_camPos += m_camRight * m_camSpeed * dt;
}

// TODO: Rotation doesn't work properly
void Camera::MouseRotate(float mouseNdcX, float mouseNdcY)
{
    
    // NDC��ǥ�� ���� [-1, 1]�� ȸ�� �ݰ� ����
    m_yaw = mouseNdcX * m_sensitivityX; // ��/�Ʒ� ȸ��
    // mouseNdcY�� -���� �� �Ʒ��� ȸ���ϰ� +�� ���� ȸ���Ϸ���
    // -�� �ٿ��� �������ش�.
    m_pitch = -mouseNdcY * m_sensitivityY; // �¿� ȸ��

    Matrix rot = Matrix::CreateRotationX(m_pitch) * Matrix::CreateRotationY(m_yaw);
    m_camDir = Vector3::Transform(m_camDir, rot);
    m_camDir.Normalize();
    m_camUp = Vector3::Transform(m_camUp, rot);
    m_camUp.Normalize();
    m_camRight = m_camUp.Cross(m_camDir);
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

void Camera::SetCamera(Vector3 pos, Vector3 dir, Vector3 up)
{
    m_camPos = pos;
    m_camDir = dir;
    m_camUp = up;
    m_camRight = m_camDir.Cross(m_camUp);
}

Vector3 Camera::GetCameraPos()
{
    return m_camPos;
}

void Camera::SetPerspectiveProjection(bool usePerspective)
{
    m_usePerspectiveProjection = usePerspective;
}
