#ifndef _CHROMA_MESHCOMPONENT_
#define _CHROMA_MESHCOMPONENT_
#include "component/IChromaComponent.h"

// Chroma
#include "texture/Texture.h"
#include "camera/Camera.h"
#include "light/Light.h"

class ChromaMeshComponent : public IChromaComponent
{
protected:
	glm::mat4 TransformationMatrix{ glm::mat4(1) };

public:
	// Getters/Setters
	virtual Shader* getShader() = 0;
	virtual int getNumTextures() = 0;
	virtual glm::mat4 getTransformationMatrix() = 0;

	// Bindings
	virtual void bindShader(Shader* newShader) = 0;
	virtual void bindTextures(std::vector<Texture> textures_val) = 0;
	virtual void bindTexture(Texture texture_val) = 0;

	// Draw
	virtual void Draw(Shader& shader) = 0;
	virtual void Draw(Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix) = 0;
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix) = 0;
	virtual void DrawUpdateMaterials(Shader& shader) = 0;
	virtual void DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix) = 0;

	// Shader Uniforms
	virtual void setMat4(std::string name, glm::mat4 value) = 0;
	virtual void setInt(std::string name, int value) = 0;

	ChromaMeshComponent();
	~ChromaMeshComponent();
};

#endif