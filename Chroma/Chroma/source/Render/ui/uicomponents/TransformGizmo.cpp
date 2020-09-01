#include "TransformGizmo.h"
#include "scene/Scene.h"
#include "input/Input.h"
#include "physics/PhysicsEngine.h"
#include "physics/RigidBody.h"
#include "editor/ui/EditorUI.h"
#include "event/MouseEvent.h"


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

		for (RigidBody*& rigidBody : m_RigidBodies)
			if (rigidBody) delete rigidBody;
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
					SetActiveAxisUniforms(m_TranslateShader);
					break;
				}
				case(Rotation):
				{
					m_RotationShader.Use();
					m_RotationShader.SetUniform("u_Model", m_Transform);
					m_RotationShader.SetUniform("u_Size", m_Size);
					SetActiveAxisUniforms(m_RotationShader);
					break;
				}
				case(Scale):
				{
					m_ScaleShader.Use();
					m_ScaleShader.SetUniform("u_Model", m_Transform);
					m_ScaleShader.SetUniform("u_Size", m_Size);
					SetActiveAxisUniforms(m_ScaleShader);
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
			// check collisions
			CheckIfHovering();

			// Set Size
			if (Input::IsPressed(KeyCode::KPAdd))
				m_Size += 0.05;

			if (Input::IsPressed(KeyCode::KPSubtract))
				m_Size -= 0.05;

			// Set Mode
			if (Input::IsPressed(KeyCode::W))
			{
				m_Mode = Translation;
				GenerateColliders();
			}
			if (Input::IsPressed(KeyCode::E))
			{
				m_Mode = Rotation;
				GenerateColliders();
			}
			if (Input::IsPressed(KeyCode::R))
			{
				m_Mode = Scale;
				GenerateColliders();
			}
	
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

			// collisions
			for (RigidBody* rigid : m_RigidBodies)
			{
				rigid->SetTransform(m_Transform);
			}

		}
	}

	void TransformGizmo::OnEvent(Event& e)
	{
		if (m_Active)
		{
			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<MouseButtonPressedEvent>(CHROMA_BIND_EVENT_FN(TransformGizmo::OnMouseButtonPressed));
		}
	}

	bool TransformGizmo::RayHitCheck(const glm::vec3& startRay, const glm::vec3& endRay)
	{
		for (RigidBody*& rigidbody : m_RigidBodies)
		{
			glm::vec3 start = Chroma::Scene::GetRenderCamera()->GetPosition();
			glm::vec3 end = Math::ScreenToWorldRay(EditorUI::GetViewportMouseCursorCoords());
			end = start + (end * glm::vec3(1000.0f));
			RayHitData rayHit = Physics::GetRayHitData(rigidbody->GetRawRigid(), start, end);
			
			if (rayHit.m_Hit)
				return true;
		}

		// gizmo not hit
		return false;
	}

	void TransformGizmo::CheckIfHovering()
	{
		m_XHovered = false;
		m_YHovered = false;
		m_ZHovered = false;
		for (RigidBody*& rigidbody : m_RigidBodies)
		{
			glm::vec3 start = Chroma::Scene::GetRenderCamera()->GetPosition();
			glm::vec3 end = Math::ScreenToWorldRay(EditorUI::GetViewportMouseCursorCoords());
			end = start + (end * glm::vec3(1000.0f));
			RayHitData rayHit = Physics::GetRayHitData(rigidbody->GetRawRigid(), start, end);
			if (rayHit.m_Hit)
			{
				Axis* axisHovered = static_cast<Axis*>(rigidbody->GetUserPointer());
				if(axisHovered)
				{
					switch (*axisHovered)
					{
						case X :
						{
							m_XHovered = true;
							break;
						}
						case Y:
						{
							m_YHovered = true;
							break;
						}
						case Z:
						{
							m_ZHovered = true;
							break;
						}
					}
				}
			}
		}	
	}

	bool TransformGizmo::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == MouseCode::ButtonLeft)
		{
			for (RigidBody*& rigidbody : m_RigidBodies)
			{
				glm::vec3 start = Chroma::Scene::GetRenderCamera()->GetPosition();
				glm::vec3 end = Math::ScreenToWorldRay(EditorUI::GetViewportMouseCursorCoords());
				end = start + (end * glm::vec3(1000.0f));
				RayHitData rayHit = Physics::GetRayHitData(rigidbody->GetRawRigid(), start, end);
				if (rayHit.m_Hit)
				{
					Axis* axisSelected = static_cast<Axis*>(rigidbody->GetUserPointer());
					if (axisSelected)
					{
						m_X = false;
						m_Y = false;
						m_Z = false;

						switch (*axisSelected)
						{
							case X:
							{
								m_X = true;
								break;
							}
							case Y:
							{
								m_Y = true;
								break;
							}
							case Z:
							{
								m_Z = true;
								break;
							}
						}
					}
				}
			}
		}


		return false;
	}

	void TransformGizmo::SetActiveAxisUniforms(Shader& shader)
	{
		shader.SetUniform("u_XAxisEnabled", m_X);
		shader.SetUniform("u_YAxisEnabled", m_Y);
		shader.SetUniform("u_ZAxisEnabled", m_Z);

		shader.SetUniform("u_XAxisHovered", m_XHovered);
		shader.SetUniform("u_YAxisHovered", m_YHovered);
		shader.SetUniform("u_ZAxisHovered", m_ZHovered);
	}

	void TransformGizmo::GeneratePointBuffers()
	{
		// create vert array
		std::vector<ChromaVertex> vertArray(3);

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
		glDrawArrays(GL_POINTS, 0, 3);
		glBindVertexArray(0);
	}

	void TransformGizmo::GenerateColliders()
	{
		// Clear colliders
		for (RigidBody*& rigidBody : m_RigidBodies)
		{
			rigidBody->RemoveFromWorld();
		}
		m_RigidBodies.clear();

		switch (m_Mode)
		{
			case Rotation:
			{
				// Y
				RigidBodyConstructionData RBDConstruction;
				RBDConstruction.m_ColliderShape = Cylinder;
				RBDConstruction.m_HalfExtents = glm::vec3(m_Size, 0.08f, 0.0f);

				RigidBody* Y = new RigidBody(RBDConstruction);
				Y->SetUserPointer(&m_YAxis);
				m_RigidBodies.push_back(Y);

				// X
				RBDConstruction.m_ColliderShape = Cylinder;
				RBDConstruction.m_LocalTransform = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

				RigidBody* X = new RigidBody(RBDConstruction);
				X->SetUserPointer(&m_XAxis);
				m_RigidBodies.push_back(X);

				// Z
				RBDConstruction.m_ColliderShape = Cylinder;
				RBDConstruction.m_LocalTransform = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

				RigidBody* Z = new RigidBody(RBDConstruction);
				Z->SetUserPointer(&m_ZAxis);
				m_RigidBodies.push_back(Z);
				break;

				break;
			}
			case Scale :
			case Translation :
			{
				// Y
				RigidBodyConstructionData RBDConstruction;
				RBDConstruction.m_ColliderShape = Cylinder;
				RBDConstruction.m_HalfExtents = glm::vec3(m_Size * 0.05, m_Size * 0.5, 0.0);
				RBDConstruction.m_LocalTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, m_Size * 0.5,0.0));

				RigidBody* Y = new RigidBody(RBDConstruction);
				Y->SetUserPointer(&m_YAxis);
				m_RigidBodies.push_back(Y);

				// X
				RBDConstruction.m_ColliderShape = Cylinder;
				RBDConstruction.m_LocalTransform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Size * 0.5, 0.0, 0.0));
				RBDConstruction.m_LocalTransform = glm::rotate(RBDConstruction.m_LocalTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

				RigidBody* X = new RigidBody(RBDConstruction);
				X->SetUserPointer(&m_XAxis);
				m_RigidBodies.push_back(X);

				// Z
				RBDConstruction.m_ColliderShape = Cylinder;
				RBDConstruction.m_LocalTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, m_Size * 0.5f));
				RBDConstruction.m_LocalTransform = glm::rotate(RBDConstruction.m_LocalTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

				RigidBody* Z = new RigidBody(RBDConstruction);
				Z->SetUserPointer(&m_ZAxis);
				m_RigidBodies.push_back(Z);
				break;
			}
		}


	}

}
