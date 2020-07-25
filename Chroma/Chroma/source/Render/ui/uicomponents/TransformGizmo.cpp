#include "TransformGizmo.h"
#include "scene/Scene.h"
#include "input/Input.h"
#include "physics/PhysicsEngine.h"
#include "physics/RigidBody.h"

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
					break;
				}
				case(Rotation):
				{
					m_RotationShader.Use();
					m_RotationShader.SetUniform("u_Model", m_Transform);
					m_RotationShader.SetUniform("u_Size", m_Size);
					break;
				}
				case(Scale):
				{
					m_ScaleShader.Use();
					m_ScaleShader.SetUniform("u_Model", m_Transform);
					m_ScaleShader.SetUniform("u_Size", m_Size);
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

	void TransformGizmo::GeneratePointBuffers()
	{
		// create vert array
		std::vector<ChromaVertex> vertArray;
		for (int i = 0; i < 3; i++)
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
			case Scale :

			case Translation :
			{
				// Y
				RigidBodyConstructionData RBDConstruction;
				RBDConstruction.m_ColliderShape = Cylinder;
				RBDConstruction.m_HalfExtents = glm::vec3(m_Size * 0.05, m_Size * 0.5, 0.0);
				RBDConstruction.m_LocalTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, m_Size * 0.5,0.0));

				RigidBody* Y = new RigidBody(RBDConstruction);
				m_RigidBodies.push_back(Y);

				// X
				RBDConstruction.m_ColliderShape = Cylinder;
				RBDConstruction.m_LocalTransform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Size * 0.5, 0.0, 0.0));
				RBDConstruction.m_LocalTransform = glm::rotate(RBDConstruction.m_LocalTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

				RigidBody* X = new RigidBody(RBDConstruction);
				m_RigidBodies.push_back(X);

				// Z
				RBDConstruction.m_ColliderShape = Cylinder;
				RBDConstruction.m_LocalTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, m_Size * 0.5f));
				RBDConstruction.m_LocalTransform = glm::rotate(RBDConstruction.m_LocalTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

				RigidBody* Z = new RigidBody(RBDConstruction);
				m_RigidBodies.push_back(Z);
				break;
			}
			case Rotation:
			{
				break;
			}
		}


	}

}
