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
	void LoadFromFile(const std::string& sourcePath) override;
	void CleanUp() override;

	// Functions
	virtual void Draw(Shader& shader) override;
	virtual void Draw(Camera& RenderCamera) override;
	virtual void Draw(Shader& shader, Camera& RenderCamera) override;
	virtual void DrawUpdateMaterials(Shader& shader) override;
	virtual void DrawUpdateTransforms(Camera& renderCam) override;
	virtual void BindDrawVAO();

	// Getters/Setters
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
	StaticMesh(const std::string& sourcePath);
	StaticMesh();
	virtual ~StaticMesh();

protected:
	// mesh data
	unsigned int VAO{ 0 }, VBO{ 0 }, EBO{ 0 };
	std::vector<ChromaVertex> m_vertices;
	std::vector<unsigned int> m_Indices;

	// functions
	virtual void CalculateBBox();
	virtual void CalculateCentroid();
	virtual void SetupMesh() override;
	// render functions
	virtual void UpdateUniforms(Shader& shader, Camera& RenderCam);
	virtual void UpdateTransformUniforms(Shader& shader, Camera& renderCam);
	virtual void UpdateMaterialUniforms(Shader& shader);
	virtual void updateTextureUniforms(Shader& shader);
	void UpdatePBRLightingTextureUniforms(Shader& shader);
};

#endif