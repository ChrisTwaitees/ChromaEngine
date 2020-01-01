#ifndef _CHROMA_ANIMATION_ENGINE_H_
#define _CHROMA_ANIMATION_ENGINE_H_


// chroma
#include <scene/Scene.h>
#include <component/AnimationComponent.h>

class AnimationEngine
{
	Scene* m_Scene{ nullptr };

public:

	void BindScene(Scene*& newScene) { m_Scene = newScene; };
	void Update();
	void UpdateDebug(std::string const& debugAnimClipName, float const& debugTime);

	AnimationEngine();
	~AnimationEngine();
};

#endif