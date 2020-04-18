#ifndef _CHROMA_SCENE_MANAGER_H_
#define _CHROMA_SCENE_MANAGER_H_






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