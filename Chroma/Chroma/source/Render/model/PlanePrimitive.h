#ifndef _PLANEPRIMITIVE_
#define _PLANEPRIMITIVE_
#include "model/StaticMesh.h"

class PlanePrimitive : public StaticMesh
{
protected:
	std::vector<float> quadVertices;
	void SetupQuad();
public :
	// Functions
	void BindDrawVAO() override;

	PlanePrimitive();
	~PlanePrimitive();
};

#endif