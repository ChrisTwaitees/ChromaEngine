#include "TranslateGizmo.h"

TranslateGizmo::TranslateGizmo()
{
}

void TranslateGizmo::Init()
{
	m_Shader = Shader(m_FragSouce, m_VtxSource, m_GeomSource);
	GeneratePointVAO();
}