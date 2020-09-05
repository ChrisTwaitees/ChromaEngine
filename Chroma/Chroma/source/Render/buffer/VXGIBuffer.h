#ifndef CHROMA_VXGIBUFFER_H
#define CHROMA_VXGIBUFFER_H

#include "buffer/IFramebuffer.h"
#include "texture/Texture3D.h"
#include "model/BoxPrimitive.h"

namespace Chroma
{
	class VXGIBuffer : public IFramebuffer
	{
	public:
		VXGIBuffer() { Init(); }
		~VXGIBuffer();

		virtual void Bind() override;
		virtual void Draw() override;
		void Draw(const bool& visualizeVoxelization);

		void SetVoxelGridWSSize(const float& newSize) { m_VoxelGridWSSize = newSize;  UpdateVoxelGridSize(); }
		void SetVoxelGridCenter(const glm::vec3& newCenter) { m_VoxelGridCentroid = newCenter; };
		std::pair<glm::vec3, glm::vec3> GetVoxelGridHalfExtents();

		void Voxelize();

	private:
		virtual void GenTexture() override;
		virtual void ResizeBuffers() override;
		virtual void Init() override;

		Texture3D* m_Voxel3DTexture;
		Shader m_VoxelShader{ "resources/shaders/fragVoxelization.glsl" , "resources/shaders/vtxVoxelization.glsl", "resources/shaders/geomVoxelization.glsl" };
		void SetupVoxelVisualizationVAO();

	private:
		float m_VoxelGridWSSize{ 5.0f };
		float m_VoxelGridSize{ 0.0390625f };
		glm::vec3 m_VoxelGridCentroid{ 0.0f };
		const unsigned int m_VoxelGridTextureSize{ 128 };
		const unsigned int m_NumVoxels{ m_VoxelGridTextureSize * m_VoxelGridTextureSize * m_VoxelGridTextureSize };
		void UpdateVoxelGridSize() { m_VoxelGridSize = m_VoxelGridWSSize / (float)m_VoxelGridTextureSize; }

	private:
		Shader m_VoxelVisualizationShader{ "resources/shaders/fragVoxelVisualization.glsl" , "resources/shaders/vtxVoxelVisualization.glsl", "resources/shaders/geomVoxelVisualization.glsl" };
		unsigned int m_VoxelVisualizationVAO{ 0 };
		void DrawVoxelVisualization();

	private:
		void UpdateVoxelShaderUniforms(Shader& shader);
	};
}


#endif // CHROMA_VXGIBUFFER_H