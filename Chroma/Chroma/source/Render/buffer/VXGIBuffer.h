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
		~VXGIBuffer() { delete m_ScreenShader; };

		virtual void Bind() override;
		virtual void Draw() override;
		void Draw(const bool& visualizeVoxelization = false);

		void Voxelize();
	private:
		virtual void GenTexture() override;
		virtual void ResizeBuffers() override;
		virtual void Init() override;
		
	private:
		const unsigned int m_VoxelTextureSize{ 64 };
		std::unique_ptr<Texture3D> m_Voxel3DTexture;
		Shader m_VoxelShader{ "resources/shaders/fragVoxelization.glsl" , "resources/shaders/vtxVoxelization.glsl", "resources/shaders/geomVoxelization.glsl" };

	private:
		Shader m_VoxelWorldPositionShader{ "resources/shaders/fragWorldPosition.glsl" , "resources/shaders/vtxWorldPosition.glsl"};
		Shader m_VoxelVisualizationShader{ "resources/shaders/fragVoxelVisualization.glsl" , "resources/shaders/vtxVoxelVisualization.glsl"};
		IFramebuffer m_VVFBO1, m_VVFBO2;
		std::unique_ptr<StaticMesh> m_Box;
		void DrawVoxelVisualization();

	};
}


#endif // CHROMA_VXGIBUFFER_H