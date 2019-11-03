#ifndef _PLANEPRIMITIVE_
#define _PLANEPRIMITIVE_
#include "model/StaticMesh.h"

class PlanePrimitive : public StaticMesh
{
protected:
	std::vector<float> quadVertices;
	void setupQuad();
public :
	// Functions
	void BindDrawVAO() override;

	PlanePrimitive();
	~PlanePrimitive();
};

#endif