#ifndef _CHROMA_MATH_
#define _CHROMA_MATH_

#include <glm/glm.hpp>
#include <bullet/btBulletCollisionCommon.h>

glm::mat4 convertBulletToGLM(btTransform& transform)
{
	glm::mat4 glmTransform;
	transform.getOpenGLMatrix(glm::value_ptr(glmTransform));
	return glmTransform;
}




#endif