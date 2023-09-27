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
	// 원근 투영을 사용할 것인지
	bool m_usePerspectiveProjection = true;
	bool m_wireFrame = false;
};