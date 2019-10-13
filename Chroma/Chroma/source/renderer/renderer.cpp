#include "Renderer.h"



void Renderer::Init()
{
}

void Renderer::Render()
{
	for (ChromaEntity* entity : mScene->Entities)
	{
		entity->Draw(*mScene->RenderCamera, mScene->Lights);
	}
	// Renders Object
	// for(uint32_t objectID : mScene->Objects) 
	//	  const Object* oject  = &mScene->Objects[objectID];
	//    const Mesh* mesh = &mScene->Meshes[object->MeshID];
	//    const Transform* transform = &mScene->Transforms[object->TransformID];

	// Renders Framebuffers/ Screen Effects

	// Render GUI



}

Renderer::Renderer(ChromaScene* Scene)
{
	mScene = Scene;
}


Renderer::~Renderer()
{
}
