#ifndef _CHROMA_MESHCOMPONENT_
#define _CHROMA_MESHCOMPONENT_

// Chroma
#include <component/IComponent.h>
#include <model/Vertex.h>
#include <texture/Texture.h>
#include <camera/Camera.h>
#include <light/Light.h>


class MeshComponent : public IComponent
{
protected:
	glm::mat4 m_TransformationMatrix{ 1.0 };
	glm::vec3 m_BBoxMin{ 0.0 }, m_BBoxMax{ 0.0 };
	glm::vec3 m_Centroid{ 0.0 };
	// calculate attrs
	virtual void CalculateBBox() = 0;
	virtual void CalculateCentroid() = 0;
public:
	bool m_IsRenderable{ false };
	bool m_IsTransparent{ false };
	bool m_IsLit{ true };
	bool m_IsForwardLit{ false };
	bool m_CastShadows{ true };
	bool m_IsSkinned{ false };

	// Accessors
	virtual std::pair<glm::vec3, glm::vec3> GetBBox() = 0;
	virtual glm::vec3 GetCentroid() = 0;
	virtual Shader* GetShader() = 0;
	virtual int GetNumTextures() = 0;
	virtual glm::mat4 GetTransform() = 0;
	virtual std::vector<ChromaVertex> GetVertices() = 0;
	//virtual Skeleton* GetSkeleton() {};


	virtual void SetShader(Shader* const& newShader) = 0;
	virtual void SetTextures(std::vector<Texture> textures_val) = 0;
	virtual void AddTexture(Texture texture_val) = 0;

	// Draw
	virtual void Draw(Shader& shader) = 0;
	virtual void Draw(Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) = 0;
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) = 0;
	virtual void DrawUpdateMaterials(Shader& shader) = 0;
	virtual void DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix) = 0;

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