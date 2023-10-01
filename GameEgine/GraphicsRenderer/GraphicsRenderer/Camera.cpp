#include "Camera.h"

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
}

void Camera::MoveRight(float dt)
{
}

void Camera::UpdateMouse(float mouseX, float mouseY)
{
}

void Camera::SetAspectRatio(float aspect)
{
    m_aspect = aspect;
}

void Camera::SetFovAngle(float angle)
{
    m_fovAngleY = angle;
}

void Camera::SetCameraTo(Vector3 pos, Vector3 dir, Vector3 up)
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
