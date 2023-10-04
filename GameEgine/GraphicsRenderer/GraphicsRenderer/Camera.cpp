#include "Camera.h"

#include <iostream>
Matrix Camera::GetFPPViewRowMatrix()
{
    // View 좌표계에서는 정면 방향이 +z, 오른쪽이 +x, 위쪽이 +y이다.
    // 시점 변환은 가상 세계가 통째로 반대로 움직이는 것과 같다.
    return Matrix::CreateRotationX(-m_pitch)
        * Matrix::CreateRotationY(-m_yaw)
        * Matrix::CreateTranslation(-m_camPos);
}

Matrix Camera::GetProjRowMatrix()
{
    // 투영 행렬
    if (m_usePerspectiveProjection)
    {
        // 원근 투영
        // (시야각, 화면비율, Near-Plane, Far-Plane) - 시야각은 radian을 사용하기에 각도를 변환해준다. 
        return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_fovAngleY), m_aspect, m_nearZ, m_farZ);
    }
    else
    {
        // 정투영
        // (x 방향 범위 변수 2개, y 방향 범위 변수 2개, Near-Plane, Far-Plane)
        return DirectX::XMMatrixOrthographicOffCenterLH(-m_aspect, m_aspect, -1.0f, 1.0f, m_nearZ, m_farZ);
    }
}

Matrix Camera::GetFocusViewRowMatrix(float focusDis)
{
    // (카메라 위치, 바라보는 위치(초점), 카메라의 upVector)    
    return DirectX::XMMatrixLookAtLH(
        m_camPos, m_camDir * focusDis, m_camUp);
}

void Camera::MoveForward(float dt)
{
    // 컴퓨터마다 속도가 다르기에 시간값(delta time)을 곱해준다.
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
    
    // NDC좌표계 범위 [-1, 1]로 회전 반경 제한
    m_yaw = mouseNdcX * m_sensitivityX; // 위/아래 회전
    // mouseNdcY가 -값일 떄 아래로 회전하고 +면 위로 회전하려면
    // -를 붙여서 저장해준다.
    m_pitch = -mouseNdcY * m_sensitivityY; // 좌우 회전

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
