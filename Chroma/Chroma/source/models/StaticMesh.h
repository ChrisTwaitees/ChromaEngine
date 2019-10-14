#ifndef _MESH_H_
#define _MESH_H_
// stl
#include <vector>
#include <string>

// chroma
#include "../shaders/Shader.h"
#include "../texture/Texture.h"
#include "../cameras/Camera.h"
#include "../lights/Light.h"
#include "../component/ChromaMeshComponent.h"

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

class StaticMesh : public ChromaMeshComponent
{
protected:
	// Default Shader
	std::string fragShaderSource = "resources/shaders/fragLitReflect.glsl";
	std::string vtxShaderSource = "resources/shaders/vertexShaderLighting.glsl";

	/*  RenderScene Data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions  */
	virtual void setupMesh();
	virtual void updateUniforms(Shader& updateShader, std::vector < std::shared_ptr<Light>> Lights, Camera& RenderCam, glm::mat4& TransformMatrix);
	virtual void updateTransformUniforms(Shader& shader, Camera& renderCam, glm::mat4& modelMatrix);
	virtual void updateMaterialUniforms(Shader& shader);
	virtual void updateLightingUniforms(Shader& shader, std::vector < std::shared_ptr<Light>> Lights, Camera& renderCam);
	virtual void updateTextureUniforms(Shader& shader);
public:
	// Mesh Data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Shader* pShader = new Shader(fragShaderSource, vtxShaderSource);

	// Functions
	virtual void Draw(Shader &shader) override;
	virtual void Draw(Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix) override;
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix) override;
	virtual void BindDrawVAO();

	// Bindings
	virtual void bindShader(Shader* newShader);
	virtual void bindTextures(std::vector<Texture> textures_val);
	virtual void bindTexture(Texture texture_val);

	// Getters/Setters
	virtual Shader* getShader() { return pShader; };
	int getNumTextures() override { return textures.size(); };
	virtual glm::mat4 getTransformationMatrix() override { return TransformationMatrix; };

	// Shader Uniforms
	virtual void setMat4(std::string name, glm::mat4 value);
	virtual void setInt(std::string name, int value);

	StaticMesh(std::vector<Vertex> vertices_val, std::vector<unsigned int> indices_val, std::vector<Texture> textures_val);
	StaticMesh();
	virtual ~StaticMesh();
};

#endif