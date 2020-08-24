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
		const unsigned int m_VoxelTextureSize{ 64 };
		Texture3D* m_Voxel3DTexture;
		Shader m_VoxelShader{ "resources/shaders/fragVoxelization.glsl" , "resources/shaders/vtxVoxelization.glsl", "resources/shaders/geomVoxelization.glsl" };

	private:
		Shader m_VoxelWorldPositionShader{ "resources/shaders/fragWorldPosition.glsl" , "resources/shaders/vtxWorldPosition.glsl"};
		Shader m_VoxelVisualizationShader{ "resources/shaders/fragVoxelVisualization.glsl" , "resources/shaders/frameBufferVertex.glsl" };
		IFramebuffer m_VVFBO1, m_VVFBO2;
		BoxPrimitive m_Cube;
		void DrawVoxelVisualization();

	};
}


#endif // CHROMA_VXGIBUFFER_H