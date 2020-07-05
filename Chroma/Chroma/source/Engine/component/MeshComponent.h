#ifndef _CHROMA_MESHCOMPONENT_
#define _CHROMA_MESHCOMPONENT_

// Chroma
#include <component/IComponent.h>
#include <camera/Camera.h>
#include <light/Light.h>
#include <shader/Shader.h>
#include <material/Material.h>
#include <resources/ResourceManager.h>

namespace Chroma
{
	class MeshComponent : public IComponent
	{
	public:
		// Component Functions
		void Init() override;
		virtual void SetupMesh();
		virtual void LoadFromFile(const std::string& sourcePath);
		virtual void RebuildMesh() { LoadFromFile(m_MeshData.sourcePath); };
		void OnUpdate() override;
		void Destroy() override;
		void Serialize(ISerializer*& serializer) override;
		virtual void CleanUp();

		// Mesh Attrs
		virtual bool GetMeshLoaded() { return m_MeshData.isLoaded; };
		virtual void SetIsSkinned(bool const& check) { m_MeshData.isSkinned = check; };
		inline bool GetIsSkinned() { return  m_MeshData.isSkinned; }

		// RenderFlags
		virtual void SetIsRenderable(bool const& check) { m_Material.SetIsRenderable(check);  ProcessRenderFlags(); };
		inline bool GetIsRenderable() { return m_Material.GetIsRenderable(); }
		virtual void SetIsTransparent(bool const& check) { m_Material.SetIsTransparent(check);  ProcessRenderFlags(); };
		inline bool GetIsTransparent() { return m_Material.GetIsTransparent(); }
		virtual void SetIsLit(bool const& check) { m_Material.SetIsLit(check); ProcessRenderFlags(); };
		inline bool GetIsLit() { return m_Material.GetIsLit(); }
		virtual void SetIsUnlit(bool const& check) { m_Material.SetIsUnlit(check);  ProcessRenderFlags();};
		inline bool GetIsUnlit() { return m_Material.GetIsUnlit(); }
		virtual void SetIsForwardLit(bool const& check) { m_Material.SetIsForwardLit(check);  ProcessRenderFlags();};
		inline bool GetIsForwardLit() { return m_Material.GetIsForwardLit(); }
		virtual void SetIsDoubleSided(bool const& check) { m_Material.SetIsDoubleSided(check);  ProcessRenderFlags();};
		inline bool GetIsDoubleSided() { return m_Material.GetIsDoubleSided(); }
		virtual void SetCastsShadows(bool const& check) { m_Material.SetCastsShadows(check);  ProcessRenderFlags(); };
		inline bool GetCastsShadows() { return m_Material.GetCastsShadows(); }
		virtual void SetReceivesShadows(bool const& check) { m_Material.SetReceivesShadows(check);  ProcessRenderFlags();};
		inline bool GetReceivesShadows() { return m_Material.GetReceivesShadows(); }

		// Transforms
		virtual glm::mat4 GetWorldTransform();
		virtual void SetTransform(glm::mat4 const& newTransformMat);
		virtual inline void SetScale(glm::vec3 const& newscale) { m_Scale = newscale; RebuildTransform(); }
		virtual inline void SetTranslation(glm::vec3 const& newposition) { m_Translation = newposition; RebuildTransform(); }
		virtual inline glm::vec3& GetLocalTranslation() { return m_Translation; };
		virtual glm::vec3 GetWSTranslation();
		virtual void SetRotation(glm::quat const& newRotation) { m_Rotation = newRotation; RebuildTransform(); }

		// Dimensions
		virtual std::pair<glm::vec3, glm::vec3> GetBBox();
		virtual glm::vec3 GetCentroid();
		virtual glm::mat4 GetTransform() { return m_Transform; };
		virtual std::vector<ChromaVertex> GetVertices() = 0;

		// Filepaths
		virtual std::string& GetSourcePath() { return m_MeshData.sourcePath; }
		virtual void SetSourcePath(const std::string& newSourcePath) { m_MeshData.sourcePath = newSourcePath; }

		// Draw
		virtual void Draw(Shader& shader) = 0;
		virtual void Draw(Camera& RenderCamera) = 0;
		virtual void Draw(Shader& shader, Camera& RenderCamera) = 0;
		virtual void DrawUpdateMaterials(Shader& shader) = 0;
		virtual void DrawUpdateTransforms(Camera& renderCam) = 0;

		// Shader Uniforms
		virtual void SetMat4(std::string name, glm::mat4 value) = 0;
		virtual void SetInt(std::string name, int value) = 0;
		virtual void SetFloat(std::string name, float value) = 0;
		virtual void SetJointUniforms(Shader& shader) {};

		// Materials
		virtual void SetMaterial(const Material& newMaterial);
		virtual Material& GetMaterial() { return m_Material; };

		virtual void SetShader(Shader& shader) { m_Material.SetShader(shader); };
		virtual Shader& GetShader() { return m_Material.GetShader(); };

		virtual void SetTextureSet(std::vector<Texture>& textures_val) { m_Material.SetTextureSet(textures_val); };
		virtual std::vector<Texture>& GetTextureSet() { return m_Material.GetTextureSet(); };
		virtual void AddTexture(Texture& texture_val) { m_Material.AddTexture(texture_val); };
		virtual int GetNumTextures() { return (int)m_Material.GetTextureSet().size(); };

		// Constructors
		MeshComponent();
		virtual ~MeshComponent();

	protected:
		// MeshData
		MeshData m_MeshData;

		//Material
		Material m_Material;
		void ProcessRenderFlags();

		// Transforms
		glm::mat4 m_Transform{ glm::mat4(1.0f) };
		glm::vec3 m_Translation{ glm::vec3(0.0f) };
		glm::quat m_Rotation{ glm::quat() };
		glm::vec3 m_Scale{ glm::vec3(1.0f) };
		void RebuildTransform();

		// Dimensions
		glm::vec3 m_BBoxMin{ 0.0 }, m_BBoxMax{ 0.0 };
		glm::vec3 m_Centroid{ 0.0 };

		// calculate attrs
		virtual void CalculateBBox();
		virtual void CalculateCentroid();
	};
}

#endif