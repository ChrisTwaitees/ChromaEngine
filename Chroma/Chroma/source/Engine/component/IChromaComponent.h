#ifndef _CHROMA_COMPONENT_
#define _CHROMA_COMPONENT_
#include <string>
#include <typeinfo>
#include "memory/ChromaUID.h"
#include "shader/Shader.h"
#include "camera/Camera.h"
#include "light/Light.h"
#include "texture/Texture.h"

class IChromaComponent
{
	ChromaUID uid;
	std::string name;
	std::string typeName;
public:
	bool isRenderable{ false };
	bool isTransparent{ false };
	bool isLit{ true };
	bool castShadows{ true };

	// Getters/Setters
	std::string getUID() { return uid.UID; };
	std::string getName() { return name; };
	std::string getType() { return typeName; }

	virtual Shader* getShader() = 0;
	virtual int getNumTextures() = 0;
	virtual glm::mat4 getTransformationMatrix() = 0;
	virtual glm::vec3 getPosition() { return glm::vec3(getTransformationMatrix()[3]); };

	void setName(std::string newName) { name = newName; };

	//Bind
	virtual void bindShader(Shader* newShader) = 0;
	virtual void bindTextures(std::vector<Texture> textures_val) = 0;
	virtual void bindTexture(Texture texture_val) = 0;

	// Draw
	virtual void Draw(Shader& shader) = 0;
	virtual void Draw(Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix) = 0;
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights, glm::mat4& transformMatrix) = 0;
	virtual void DrawUpdateMaterials(Shader& shader) = 0;
	virtual void DrawUpdateTransforms(Camera& renderCam, glm::mat4& modelMatrix) = 0;

	// Shader updates
	virtual void setMat4(std::string name, glm::mat4 value) = 0;
	virtual void setInt(std::string name, int value) = 0;

	IChromaComponent();
	~IChromaComponent();
};

#endif