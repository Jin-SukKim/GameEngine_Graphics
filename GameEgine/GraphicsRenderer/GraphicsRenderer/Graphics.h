#pragma once

#include "AppBase.h"
#include <vector>

#include "MeshModel.h"
#include "CubeMap.h"

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
	CubeMap m_cubeMap;

	// ImGUI parameters
	// 원근 투영을 사용할 것인지
	bool m_usePerspectiveProjection = true;
	bool m_wireFrame = false;
	bool m_useTexture = false;
	bool m_normalLine = false;

	Vector3 modelAxis = {1.f, 0.f, 0.f};
	Vector3 m_scale = Vector3(0.3f);

	Vector3 m_translation = Vector3(0.f);
	Vector3 m_rotation = Vector3(0.f);

	float m_normalScale = 0.05f;

	Light m_light;
	int lightType = 1;
	float m_diffuse = 0.5f;
	float m_specular = 1.f;
	float m_shininess = 8.f; 
};