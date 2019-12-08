#ifndef _SKINNED_MODEL_
#define _SKINNED_MODEL_

// stl

// thirdparty
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//chroma
#include <model/SkinnedMesh.h>


class SkinnedModel : public ChromaMeshComponent
{
	// calculate attrs
	virtual void calcBBox() {};
	virtual void calcCentroid() {};
public:
	SkinnedModel();
	~SkinnedModel();
};

#endif