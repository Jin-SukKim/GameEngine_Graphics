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

protected:
	MeshModel m_mesh;

	// ImGUI parameters
	// ���� ������ ����� ������
	bool m_usePerspectiveProjection = true;
	bool m_wireFrame = false;
};