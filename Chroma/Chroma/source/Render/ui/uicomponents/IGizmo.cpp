#include "IGizmo.h"
#include <model/Vertex.h>
#include <scene/Scene.h>


namespace Chroma
{
	IGizmo::IGizmo()
	{
		GeneratePointVAO();
	}

	void IGizmo::GeneratePointVAO()
	{
		ChromaVertex singleVert;

		// Generate buffers
		// Vertex Array Object Buffer
		glGenVertexArrays(1, &m_PointVAO);
		// Vertex Buffer and Element Buffer
		glGenBuffers(1, &m_PointVBO);

		// Bind buffers
		glBindVertexArray(m_PointVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_PointVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ChromaVertex), &singleVert, GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChromaVertex), (void*)0);
	}

	void IGizmo::BindPointVAO()
	{
		glBindVertexArray(m_PointVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}
}
