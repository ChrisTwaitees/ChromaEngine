#ifndef _CHROMA_MESHCOMPONENT_
#define _CHROMA_MESHCOMPONENT_
#include "../component/ChromaComponent.h"

// Chroma
#include "../texture/Texture.h"
#include "../cameras/Camera.h"
#include "../lights/Light.h"

class ChromaMeshComponent : public ChromaComponent
{
public:
	// Getters/Setters
	virtual Shader* getShader() = 0;

	// Bindings
	virtual void bindShader(Shader* newShader) = 0;
	virtual void bindTextures(std::vector<Texture> textures_val) = 0;
	virtual void bindTexture(Texture texture_val) = 0;

	// Draw
	virtual void Draw(Shader& shader) = 0;
	virtual void Draw(Camera& RenderCamera, std::vector<Light*>& Lights, glm::mat4& transformMatrix) = 0;
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*>& Lights, glm::mat4& transformMatrix) = 0;

	ChromaMeshComponent();
	~ChromaMeshComponent();
};

#endif