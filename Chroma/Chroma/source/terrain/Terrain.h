#ifndef _CHROMA_TERRAIN_
#define _CHROMA_TERRAIN_
#include "../models/Mesh.h"


class Terrain : public Mesh
{
private:
	// default shader
	std::string fragShaderSource = "resources/shaders/fragLitReflect.glsl";
	std::string vtxShaderSource = "resources/shaders/vertexShaderLighting.glsl";
	Shader shader;
	Texture texture;
	Camera camera;

	// transforms
	glm::mat4 modelMat;

	// quad data
	std::vector<float> quadData = {
		// positions           // normals            // texture coords
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f
	};

	// initialize
	void initialize();
	void setupQuad();

	virtual void updateMaterialUniforms(Shader& shader) override;
public:
	void Draw();
	void attachCamera(const Camera& newCamera);

	Terrain();
	Terrain(Camera& camera_val);
	~Terrain();
};

#endif