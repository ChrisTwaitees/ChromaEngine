#include "IGizmo.h"
#include <model/Vertex.h>
#include <scene/Scene.h>

namespace Chroma
{
	IGizmo::IGizmo()
	{
	}

	void IGizmo::Draw()
	{
		m_Shader.Use();
		m_Shader.SetUniform("model", glm::mat4(1.0));
		m_Shader.SetUniform("color", glm::vec3(1.0,0.0,0.0));
		m_Shader.SetUniform("Size", 50.0f);
		m_Shader.SetUniform("VPMat", Chroma::Scene::GetRenderCamera()->GetViewProjMatrix());
		BindPointVAO();
	}

	void IGizmo::Init()
	{
		m_Shader = Shader(m_FragSouce, m_VtxSource, m_GeomSource);
		GeneratePointVAO();
	}

	void IGizmo::GeneratePointVAO()
	{
		ChromaVertex singleVert;

		// Generate buffers
		// Vertex Array Object Buffer
		glGenVertexArrays(1, &pointVAO);
		// Vertex Buffer and Element Buffer
		glGenBuffers(1, &pointVBO);

		// Bind buffers
		glBindVertexArray(pointVAO);
		glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ChromaVertex), &singleVert, GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)0);
	}

	void IGizmo::BindPointVAO()
	{
		glBindVertexArray(pointVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}
}
