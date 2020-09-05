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

		void Voxelize();
	private:
		virtual void GenTexture() override;
		virtual void ResizeBuffers() override;
		virtual void Init() override;
		
	private:
		const float m_VoxelGridSize{0.05f };
		glm::vec3 m_VoxelGridCentroid{ 0.0f };
		const const unsigned int m_VoxelGridTextureSize{ 128 };
		const unsigned int m_NumVoxels{ m_VoxelGridTextureSize * m_VoxelGridTextureSize * m_VoxelGridTextureSize };
		Texture3D* m_Voxel3DTexture;
		Shader m_VoxelShader{ "resources/shaders/fragVoxelization.glsl" , "resources/shaders/vtxVoxelization.glsl", "resources/shaders/geomVoxelization.glsl" };
		void SetupVoxelVisualizationVAO();

	private:
		Shader m_VoxelVisualizationShader{ "resources/shaders/fragVoxelVisualization.glsl" , "resources/shaders/vtxVoxelVisualization.glsl", "resources/shaders/geomVoxelVisualization.glsl" };
		unsigned int m_VoxelVisualizationVAO{ 0 };
		void DrawVoxelVisualization();

	private:
		void UpdateVoxelShaderUniforms(Shader& shader);
	};
}


#endif // CHROMA_VXGIBUFFER_H