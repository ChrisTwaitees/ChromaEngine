#ifndef _CHROMA_COMPONENT_
#define _CHROMA_COMPONENT_
#include <string>
#include <typeinfo>
#include "../memory/ChromaUID.h"
#include "../shaders/Shader.h"
#include "../cameras/Camera.h"
#include "../lights/Light.h"
#include "../texture/Texture.h"

class ChromaComponent
{
	ChromaUID uid;
	std::string name;
	std::string typeName;
public:
	bool isRenderable{ false };
	// Getters/Setters
	std::string getUID() { return uid.UID; };
	std::string getName() { return name; };
	std::string getType() { return typeName; }
	virtual Shader* getShader() = 0;

	void setName(std::string newName) { name = newName; };

	//Bind
	virtual void bindShader(Shader* newShader) = 0;
	virtual void bindTextures(std::vector<Texture> textures_val) = 0;
	virtual void bindTexture(Texture texture_val) = 0;

	// Draw
	virtual void Draw(Shader& shader) = 0;
	virtual void Draw(Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix) = 0;
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix) = 0;

	ChromaComponent();
	~ChromaComponent();
};

#endif