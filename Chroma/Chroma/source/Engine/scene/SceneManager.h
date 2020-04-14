#ifndef _CHROMA_SCENE_MANAGER_H_
#define _CHROMA_SCENE_MANAGER_H_

#include <scene/Scene.h>
#include <serialization/scene/JSONScene.h>



namespace Chroma
{
	class SceneManager
	{
	public:
		static void Init();

		static void SaveScene(const char* sourceScenePath);
		static void LoadScene(const char* destinationScenePath);

	private:

		static void ClearScene();
	};

}


#endif