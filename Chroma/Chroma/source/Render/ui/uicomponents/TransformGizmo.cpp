#include "TransformGizmo.h"
#include "scene/Scene.h"
#include "input/Input.h"

namespace Chroma
{

	TransformGizmo::~TransformGizmo()
	{
		m_TranslateShader.Destroy();
		m_RotationShader.Destroy();
		m_ScaleShader.Destroy();
	}

	void TransformGizmo::Draw()
	{
		if (m_Active)
		{
			switch (m_Mode)
			{
				case(Translation):
				{
					m_TranslateShader.Use();
					m_TranslateShader.SetUniform("u_Model", m_Transform);
					m_TranslateShader.SetUniform("u_Size", 1.0f);
					m_TranslateShader.SetUniform("u_VPMat", Scene::GetRenderCamera()->GetViewProjMatrix());
					break;
				}
				case(Rotation):
				{
					m_RotationShader.Use();
					m_RotationShader.SetUniform("model", m_Transform);
					m_RotationShader.SetUniform("color", glm::vec3(0.0, 1.0, 0.0));
					m_RotationShader.SetUniform("Size", 10.0f);
					m_RotationShader.SetUniform("VPMat", Scene::GetRenderCamera()->GetViewProjMatrix());
					break;
				}
				case(Scale):
				{
					m_ScaleShader.Use();
					m_ScaleShader.SetUniform("model", m_Transform);
					m_ScaleShader.SetUniform("color", glm::vec3(0.0, 0.0, 1.0));
					m_ScaleShader.SetUniform("Size", 10.0f);
					m_ScaleShader.SetUniform("VPMat", Scene::GetRenderCamera()->GetViewProjMatrix());
					break;
				}
			}

			BindPointVAO();
		}
	}

	void TransformGizmo::OnUpdate()
	{
		if (m_Active)
		{
			// Set Mode
			if (Input::IsPressed(KeyCode::W))
				m_Mode = Translation;
			if (Input::IsPressed(KeyCode::E))
				m_Mode = Rotation;
			if (Input::IsPressed(KeyCode::R))
				m_Mode = Scale;
	
			// Check interactions
			switch (m_Mode)
			{
				case(Translation):
				{
					break;
				}
				case(Rotation):
				{
					break;
				}
				case(Scale):
				{
					break;
				}
			}
		}
	}

}
