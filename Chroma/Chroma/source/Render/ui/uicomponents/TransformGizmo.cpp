#include "TransformGizmo.h"
#include "scene/Scene.h"
#include "input/Input.h"
#include "physics/PhysicsEngine.h"

namespace Chroma
{
	TransformGizmo::TransformGizmo()
	{
		GeneratePointBuffers();
		GenerateColliders();
	}

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
					m_TranslateShader.SetUniform("u_Size", m_Size);
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

			BindDrawVAO();
		}
	}

	void TransformGizmo::OnUpdate()
	{
		if (m_Active)
		{
			
			// Set Size
			if (Input::IsPressed(KeyCode::KPAdd))
				m_Size += 0.05;
			if (Input::IsPressed(KeyCode::KPSubtract))
				m_Size -= 0.05;

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

	void TransformGizmo::GeneratePointBuffers()
	{
		// create vert array
		std::vector<ChromaVertex> vertArray;
		for (int i = 0; i < m_PointArraySize; i++)
			vertArray.push_back(ChromaVertex());

		// Generate buffers
		// Vertex Array Object Buffer
		glGenVertexArrays(1, &m_PointVAO);
		// Vertex Buffer and Element Buffer
		glGenBuffers(1, &m_PointVBO);

		// Bind buffers
		glBindVertexArray(m_PointVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_PointVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ChromaVertex) * 3, &vertArray[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)0);
	}

	void TransformGizmo::BindDrawVAO()
	{
		glBindVertexArray(m_PointVAO);
		glDrawArrays(GL_POINTS, 0, m_PointArraySize);
		glBindVertexArray(0);
	}

	void TransformGizmo::GenerateColliders()
	{
		// Translate
		//Physics::CreateBox(glm::vec3(), )
	}

}
