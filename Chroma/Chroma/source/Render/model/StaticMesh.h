#ifndef _MESH_H_
#define _MESH_H_

// stl
#include <vector>
#include <string>

// chroma
#include <camera/Camera.h>
#include <component/MeshComponent.h>


class StaticMesh : public MeshComponent
{
protected:
	// mesh data
	unsigned int VAO, VBO, EBO;
	std::vector<ChromaVertex> m_vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;
	// default shader
	std::string fragShaderSource = "resources/shaders/fragPBR.glsl";
	std::string vtxShaderSource = "resources/shaders/vertexLitShadowsNormals.glsl";
	Shader* m_shader{ new Shader(fragShaderSource, vtxShaderSource) };

	// functions
	virtual void CalculateBBox();
	virtual void CalculateCentroid();
	virtual void SetupMesh();
	// render functions
	virtual void UpdateUniforms(const Shader* shader, std::vector<Light*> Lights, Camera& RenderCam, glm::mat4& TransformMatrix);
	virtual void UpdateTransformUniforms(const Shader* shader, Camera& renderCam, glm::mat4& modelMatrix);
	virtual void UpdateMaterialUniforms(const Shader* shader);
	virtual void UpdateLightingUniforms(const Shader*& shader, std::vector<Light*> Lights, Camera& renderCam);
	virtual void updateTextureUniforms(const Shader* shader);
	void UpdatePBRLightingTextureUniforms(const Shader*& shader);

public:
	// Functions
	virtual void Draw(Shader &shader) override;
	virtual void Draw(Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) override;
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) override;
	virtual void DrawUpdateMaterials(Shader& shader) override;
	virtual void DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix) override;
	virtual void BindDrawVAO();

	// Bindings
	virtual void SetShader(Shader* const& newShader) override;
	virtual void SetTextures(std::vector<Texture> textures_val) override;
	virtual void AddTexture(Texture texture_val) override;

	// Getters/Setters
	virtual Shader* GetShader() { return m_shader; };
	int GetNumTextures() override { return (int)m_Textures.size(); };
	virtual glm::mat4 GetTransform() override { return m_TransformationMatrix; };
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
};

#endif