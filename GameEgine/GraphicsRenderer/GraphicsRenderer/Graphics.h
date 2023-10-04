#pragma once

#include "AppBase.h"
#include <vector>

#include "MeshModel.h"

class Graphics : public AppBase {
public:
	Graphics();

	virtual bool Initialize() override;
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual void UpdateGUI() override;

	void UserInput(float dt);

protected:
	MeshModel m_mesh;

	// ImGUI parameters
	// 원근 투영을 사용할 것인지
	bool m_usePerspectiveProjection = true;
	bool m_wireFrame = false;
	
	Vector3 modelAxis = {1.f, 0.f, 0.f};
	float m_scale = 0.3f;

	float m_rotationX = 0.f;
	float m_rotationY = 0.f;
	float m_rotationZ = 0.f;

	float m_tranlationX = 0.f;
	float m_tranlationY = 0.f;
	float m_tranlationZ = 0.f;
};