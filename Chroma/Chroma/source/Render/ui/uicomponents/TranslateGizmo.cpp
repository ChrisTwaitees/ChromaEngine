#include "TranslateGizmo.h"

namespace Chroma
{
	TranslateGizmo::TranslateGizmo()
	{
	}

	void TranslateGizmo::Initialize()
	{
		m_GeomSource = "resources/shaders/geometryGizmoTranslation.glsl";
		m_Shader = Shader(m_FragSouce, m_VtxSource, m_GeomSource);
		GeneratePointVAO();
	}
}
