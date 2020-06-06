#ifndef _MODEL_H_
#define _MODEL_H_


// chroma
#include <math/Math.h>
#include <component/MeshComponent.h>
#include <resources/ResourceManager.h>


class Model : public MeshComponent
{
public:
	// Init
	virtual void Init() override;
	void RebuildMesh() override;
	void Destroy() override;
	void CleanUp() override;

	// Serialization
	virtual void Serialize(ISerializer*& serializer) override;

	// Draw
	void Draw(Shader& shader) override;
	void Draw(Camera& RenderCamera) override;
	void Draw(Shader& shader, Camera& RenderCamera) override;
	void DrawUpdateMaterials(Shader& shader) override;
	void DrawUpdateTransforms(Camera& renderCam) override;

	// Accessors	
	std::pair<glm::vec3, glm::vec3> GetBBox() override;
	glm::vec3 GetCentroid();
	std::string& GetSourcePath() override { return m_SourcePath; }
	void SetSourcePath(const std::string& newSourcePath) override { m_SourcePath = newSourcePath; }

	// render components
	Shader& GetShader();
	void SetShader(Shader& shader) override;
	std::vector<UID>& GetMeshUIDs() { return m_MeshUIDs; }
	// verts
	std::vector<ChromaVertex> GetVertices() override;

	// Material
	void SetIsLit(bool const& check) override;
	void SetIsForwardLit(bool const& check) override;
	void SetTextureSet(std::vector<Texture>& textures_val) override;
	void AddTexture(Texture& texture_val) override;
	void SetMaterial(const Material& newMaterial) override;

	// Shader Uniforms
	void SetMat4(std::string name, glm::mat4 value) override;
	void SetInt(std::string name, int value) override;
	void SetFloat(std::string name, float value) override;
	void SetJointUniforms(Shader& skinnedShader) override;

	// Constructors
	Model(std::string path) { LoadModel(path); };
	~Model();

protected:
	// calculate dimensions
	virtual void CalculateBBox() override;
	virtual void CalculateCentroid() override;

	// Model Data
	std::vector<UID> m_MeshUIDs;
	// verts
	std::vector<ChromaVertex> m_vertices;
	std::vector<ChromaSkinnedVertex> m_SkinnedVertices;

	//sourcePath
	std::string m_SourcePath;
	// Functions
	void LoadModel(std::string path);
};

#endif