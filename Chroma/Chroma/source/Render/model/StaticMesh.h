#ifndef _MESH_H_
#define _MESH_H_
// stl
#include <vector>
#include <string>

// chroma
#include <shader/Shader.h>
#include <texture/Texture.h>
#include <camera/Camera.h>
#include <light/Light.h>
#include <component/ChromaMeshComponent.h>
#include <model/Vertex.h>


class StaticMesh : public ChromaMeshComponent
{
protected:
	// mesh data
	unsigned int VAO, VBO, EBO;
	std::vector<ChromaVertex> vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	// default shader
	std::string fragShaderSource = "resources/shaders/fragPBR.glsl";
	std::string vtxShaderSource = "resources/shaders/vertexLitShadowsNormals.glsl";
	Shader* m_shader{ new Shader(fragShaderSource, vtxShaderSource) };

	// functions
	virtual void calcBBox();
	virtual void calcCentroid();
	virtual void setupMesh();
	// render functions
	virtual void updateUniforms(const Shader* shader, std::vector<Light*> Lights, Camera& RenderCam, glm::mat4& TransformMatrix);
	virtual void updateTransformUniforms(const Shader* shader, Camera& renderCam, glm::mat4& modelMatrix);
	virtual void updateMaterialUniforms(const Shader* shader);
	virtual void updateLightingUniforms(const Shader* shader, std::vector<Light*> Lights, Camera& renderCam);
	virtual void updateTextureUniforms(const Shader* shader);

public:
	// Functions
	virtual void Draw(Shader &shader) override;
	virtual void Draw(Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) override;
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights, glm::mat4& transformMatrix) override;
	virtual void DrawUpdateMaterials(Shader& shader) override;
	virtual void DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix) override;
	virtual void BindDrawVAO();

	// Bindings
	virtual void bindShader(Shader* const& newShader) override;
	virtual void bindTextures(std::vector<Texture> textures_val) override;
	virtual void bindTexture(Texture texture_val) override;

	// Getters/Setters
	virtual Shader* getShader() { return m_shader; };
	int getNumTextures() override { return m_textures.size(); };
	virtual glm::mat4 getTransformationMatrix() override { return m_transformationMatrix; };
	virtual std::vector<ChromaVertex> getVertices() { return vertices; };
	virtual std::pair<glm::vec3, glm::vec3> getBBox();
	virtual glm::vec3 getCentroid();

	// Shader Uniforms
	virtual void setMat4(std::string name, glm::mat4 value) override;
	virtual void setInt(std::string name, int value) override;
	virtual void setFloat(std::string name, float value) override;

	StaticMesh(std::vector<ChromaVertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val);
	StaticMesh();
	virtual ~StaticMesh();
};

#endif