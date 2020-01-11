#ifndef _CHROMA_MESHCOMPONENT_
#define _CHROMA_MESHCOMPONENT_

// Chroma
#include <component/IComponent.h>
#include <camera/Camera.h>
#include <light/Light.h>
#include <shader/Shader.h>

#include <model/MeshData.h>

class MeshComponent : public IComponent
{
protected:
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
public:
	// Functions
	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	// attrs
	bool m_IsRenderable{ false };
	bool m_IsTransparent{ false };
	bool m_IsLit{ true };
	bool m_IsForwardLit{ false };
	bool m_CastShadows{ true };
	bool m_IsSkinned{ false };
	bool m_IsDoubleSided{ false };
	glm::vec2 m_UVMultiply{ 1.0 };

	// transforms
	// set
	virtual glm::mat4 GetWorldTransform();
	virtual void SetTransform(glm::mat4 const& newTransformMat);
	virtual inline void SetScale(glm::vec3 const& newscale) { m_Scale = newscale; RebuildTransform(); }
	virtual inline void SetTranslation(glm::vec3 const& newposition) { m_Translation = newposition; RebuildTransform(); }
	virtual void SetRotation(glm::quat const& newRotation) { m_Rotation = newRotation; RebuildTransform(); }

	// Accessors
	virtual std::pair<glm::vec3, glm::vec3> GetBBox() = 0;
	virtual glm::vec3 GetCentroid() = 0;
	virtual Shader* GetShader() = 0;
	virtual int GetNumTextures() = 0;
	virtual glm::mat4 GetTransform() { return m_Transform; };
	virtual std::vector<ChromaVertex> GetVertices() = 0;

	virtual void SetShader(Shader* const& newShader) = 0;
	virtual void SetTextures(std::vector<Texture> textures_val) = 0;
	virtual void AddTexture(Texture texture_val) = 0;

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

	// Constructors
	MeshComponent();
	virtual ~MeshComponent();
};

#endif