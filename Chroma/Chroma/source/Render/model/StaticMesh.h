#ifndef _MESH_H_
#define _MESH_H_

//common
#include <common/PrecompiledHeader.h>

// chroma
#include <camera/Camera.h>
#include <component/MeshComponent.h>


class StaticMesh : public MeshComponent
{
public:
	// Component Functions
	void Init() override;
	void Destroy() override;
	void Serialize(ISerializer*& serializer) override;

	// Functions
	virtual void Draw(Shader& shader) override;
	virtual void Draw(Camera& RenderCamera) override;
	virtual void Draw(Shader& shader, Camera& RenderCamera) override;
	virtual void DrawUpdateMaterials(Shader& shader) override;
	virtual void DrawUpdateTransforms(Camera& renderCam) override;
	virtual void BindDrawVAO();

	// Bindings
	virtual void SetShader(Shader const& shader) override { m_shader = shader; } ;
	virtual void SetTextures(std::vector<Texture> textures_val) override;
	virtual void AddTexture(Texture texture_val) override;

	// Getters/Setters
	virtual Shader& GetShader() { return m_shader; };
	int GetNumTextures() override { return (int)m_Textures.size(); };
	virtual glm::mat4 GetTransform() override { return m_Transform; };
	virtual std::vector<ChromaVertex> GetVertices() { return m_vertices; };
	virtual std::pair<glm::vec3, glm::vec3> GetBBox();
	virtual glm::vec3 GetCentroid();

	// Shader Uniforms
	virtual void SetMat4(std::string name, glm::mat4 value) override;
	virtual void SetInt(std::string name, int value) override;
	virtual void SetFloat(std::string name, float value) override;

	StaticMesh(std::vector<ChromaVertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val);
	StaticMesh(MeshData const& newMeshData);
	StaticMesh();
	virtual ~StaticMesh();

protected:
	// mesh data
	unsigned int VAO{ 0 }, VBO{ 0 }, EBO{ 0 };
	std::vector<ChromaVertex> m_vertices;
	std::vector<unsigned int> m_Indices;
	// default shader
	std::string fragShaderSource = "resources/shaders/fragPBR.glsl";
	std::string vtxShaderSource = "resources/shaders/vertexLitShadowsNormals.glsl";
	Shader m_shader{ fragShaderSource, vtxShaderSource };

	// functions
	virtual void CalculateBBox();
	virtual void CalculateCentroid();
	virtual void SetupMesh();
	// render functions
	virtual void UpdateUniforms(Shader& shader, Camera& RenderCam);
	virtual void UpdateTransformUniforms(Shader const& shader, Camera& renderCam);
	virtual void UpdateMaterialUniforms(Shader const& shader);
	virtual void UpdateLightingUniforms(Shader& shader, Camera& renderCam);
	virtual void updateTextureUniforms(Shader const& shader);
	void UpdatePBRLightingTextureUniforms(Shader const& shader);
};

#endif