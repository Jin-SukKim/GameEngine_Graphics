#include "Camera.h"

#include <iostream>
Camera::Camera(Vector3 pos) : m_camPos(pos) {}

Matrix Camera::GetFPPViewRowMatrix()
{
    // View 좌표계에서는 정면 방향이 +z, 오른쪽이 +x, 위쪽이 +y이다.
    // 시점 변환은 가상 세계가 통째로 반대로 움직이는 것과 같다.
    return Matrix::CreateTranslation(-m_camPos)
        * Matrix::CreateRotationY(-m_yaw)
        * Matrix::CreateRotationX(m_pitch);
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

// 회전하는 것이 아니라면 방향 벡터들은 업데이트할 필요없다.
void Camera::MoveForward(float dt)
{
    // 컴퓨터마다 속도가 다르기에 시간값(delta time)을 곱해준다.
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
    
    // NDC좌표계 범위 [-1, 1]로 회전 반경 제한
    // 좌우 회전 (y 축을 중심으로 회전) - 좌우 360도 회전
    m_yaw = mouseNdcX * m_sensitivityX * DirectX::XM_2PI; 
    // mouseNdcY가 -값일 떄 아래로 회전하고 +면 위로 회전하려면 -를 붙여서 저장해준다.
    // 스크린 좌표계는 y값이 아래로 갈수록 커지기에 -로 반전시켜주는 것이다.
    // 위/아래 회전(z 축을 중심으로 회전) - 위 아래 90도 회전
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
