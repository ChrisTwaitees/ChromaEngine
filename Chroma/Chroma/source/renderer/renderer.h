#ifndef _CHROMA_RENDERER_
#define _CHROMA_RENDERER_

#include "../scene/ChromaScene.h"

class Renderer
{
protected:

	ChromaScene* mScene;
	void Init();
public:

	void Render();
	Renderer(ChromaScene* Scene);
	~Renderer();
};

#endif