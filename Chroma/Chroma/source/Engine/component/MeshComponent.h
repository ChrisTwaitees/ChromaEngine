#ifndef _CHROMA_MESHCOMPONENT_
#define _CHROMA_MESHCOMPONENT_

// Chroma
#include <component/IComponent.h>
#include <camera/Camera.h>
#include <light/Light.h>
#include <shader/Shader.h>
#include <material/Material.h>
#include <model/MeshData.h>

class MeshComponent : public IComponent
{
public:
	// Component Functions
	void Init() override;
	void Update() override;
	void Destroy() override;
	void Serialize(ISerializer*& serializer) override;

	// attrs
	virtual void SetIsRenderable(bool const& check) { m_IsRenderable = check; };
	inline bool& GetIsRenderable() { return m_IsRenderable; }
	virtual void SetIsTransparent(bool const& check) { m_IsTransparent = check; };
	inline bool& GetIsTransparent() { return m_IsTransparent; }
	virtual void SetIsLit(bool const& check) { m_IsLit = check; };
	inline bool& GetIsLit() { return m_IsLit; }
	virtual void SetIsUnlit(bool const& check) { m_IsUnlit = check; };
	inline bool& GetIsUnlit() { return m_IsUnlit; }
	virtual void SetIsForwardLit(bool const& check) { m_IsForwardLit = check; };
	inline bool& GetIsForwardLit() { return m_IsForwardLit; }
	virtual void SetIsSkinned(bool const& check) { m_IsSkinned = check; };
	inline bool& GetIsSkinned() { return m_IsSkinned; }
	virtual void SetIsDoubleSided(bool const& check) { m_IsDoubleSided = check; };
	inline bool& GetIsDoubleSided() { return m_IsDoubleSided; }
	virtual void SetCastsShadows(bool const& check) { m_CastShadows = check; };
	inline bool& GetCastsShadows() { return m_CastShadows; }

	// Transforms
	virtual glm::mat4 GetWorldTransform();
	virtual void SetTransform(glm::mat4 const& newTransformMat);
	virtual inline void SetScale(glm::vec3 const& newscale) { m_Scale = newscale; RebuildTransform(); }
	virtual inline void SetTranslation(glm::vec3 const& newposition) { m_Translation = newposition; RebuildTransform(); }
	virtual void SetRotation(glm::quat const& newRotation) { m_Rotation = newRotation; RebuildTransform(); }

	// Dimensions
	virtual std::pair<glm::vec3, glm::vec3> GetBBox() = 0;
	virtual glm::vec3 GetCentroid() = 0;
	virtual glm::mat4 GetTransform() { return m_Transform; };
	virtual std::vector<ChromaVertex> GetVertices() = 0;

	// Filepaths
	virtual std::string& GetSourcePath() { return m_SourcePath; }
	virtual void SetSourcePath(const std::string& newSourcePath) { m_SourcePath = newSourcePath; }

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
	virtual void SetMaterial(const Material& newMaterial) { m_Material = newMaterial; };
	virtual Material& GetMaterial() { return m_Material; };

	virtual void SetShader(Shader const& shader) { m_Material.SetShader(shader); };
	virtual Shader& GetShader() { return m_Material.GetShader(); };

	virtual void SetTextureSet(std::vector<Texture>& textures_val) { m_Material.SetTextureSet(textures_val); };
	virtual std::vector<Texture>& GetTextureSet() { return m_Material.GetTextureSet(); };
	virtual void AddTexture(Texture& texture_val) { m_Material.AddTexture(texture_val); };
	virtual int GetNumTextures() { return m_Material.GetTextureSet().size(); };

	// Constructors
	MeshComponent();
	virtual ~MeshComponent();

protected:
	// Resources
	std::string m_SourcePath{ "" };
	// Transforms
	glm::mat4 m_Transform{ glm::mat4(1.0f) };
	glm::vec3 m_Translation{ glm::vec3(0.0f) };
	glm::quat m_Rotation{ glm::quat() };
	glm::vec3 m_Scale{ glm::vec3(1.0f) };
	void RebuildTransform();
	glm::vec3 m_BBoxMin{ 0.0 }, m_BBoxMax{ 0.0 };
	glm::vec3 m_Centroid{ 0.0 };
	// calculate attrs
	virtual void CalculateBBox() = 0;
	virtual void CalculateCentroid() = 0;
	//Material
	Material m_Material;
	void SerializeMaterial(ISerializer*& serializer);
	// Render Attrs
	bool m_IsRenderable{ false };
	bool m_IsTransparent{ false };
	bool m_IsLit{ true };
	bool m_IsUnlit{ false };
	bool m_IsForwardLit{ false };
	bool m_CastShadows{ true };
	bool m_IsSkinned{ false };
	bool m_IsDoubleSided{ false };
};

#endif