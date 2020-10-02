#ifndef CHROMA_VXGIBUFFER_H
#define CHROMA_VXGIBUFFER_H

#include "buffer/IFramebuffer.h"
#include "texture/Texture3D.h"
#include "model/BoxPrimitive.h"

namespace Chroma
{
	class CameraMovedEvent;

	class VXGIBuffer : public IFramebuffer
	{
	public:
		VXGIBuffer() { Init(); }
		~VXGIBuffer();

		virtual void BindAndClear() override;
		virtual void Draw() override;
		void Draw(const bool& visualizeVoxelization);

		void SetVoxelGridWSSize(const float& newSize) { m_VoxelGridWSSize = newSize;  UpdateVoxelGridSize(); }
		void SetVoxelGridCenter(const glm::vec3& newCenter) { m_VoxelGridCentroid = newCenter; };
		std::pair<glm::vec3, glm::vec3> GetVoxelGridHalfExtents();

		void Voxelize();

		// events
		bool OnCameraMoved(CameraMovedEvent& e);

	private: // textures
		virtual void GenTexture() override;
		virtual void ResizeBuffers() override;
		virtual void Init() override;

		Texture3D* m_Voxel3DTexture;
		const unsigned int m_VoxelGridTextureSize{ 128 };
		void SetupVoxelVisualizationVAO();

	private: // voxelization
		Shader m_VoxelShader{ "resources/shaders/fragVoxelization.glsl" , "resources/shaders/vtxVoxelization.glsl", "resources/shaders/geomVoxelization.glsl" };
		float m_VoxelGridWSSize{ 8.5f };
		float m_VoxelGridSize{ 0.0390625f };
		glm::vec3 m_VoxelGridCentroid{ 0.0f };
		void UpdateVoxelGridSize() { m_VoxelGridSize = m_VoxelGridWSSize / (float)m_VoxelGridTextureSize; }
		void UpdateVoxelShaderUniforms(Shader& shader);
		bool m_VoxelGridMovedThisFrame{ false };

	private: // voxel visualization
		Shader m_VoxelVisualizationShader{ "resources/shaders/fragVoxelVisualization.glsl" , "resources/shaders/vtxVoxelVisualization.glsl", "resources/shaders/geomVoxelVisualization.glsl" };
		unsigned int m_VoxelVisualizationVAO{ 0 };
		const unsigned int m_NumVoxels{ m_VoxelGridTextureSize * m_VoxelGridTextureSize * m_VoxelGridTextureSize };
		void DrawVoxelVisualization();

	private: // cone tracing
		Shader m_VoxelConeTracing{ "resources/shaders/voxelConeTracing.glsl", "resources/shaders/frameBufferVertex.glsl"};
		void ConeTraceScene();
		friend class Render;
	};
}


#endif // CHROMA_VXGIBUFFER_H