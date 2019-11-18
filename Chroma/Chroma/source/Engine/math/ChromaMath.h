#ifndef _CHROMA_MATH_
#define _CHROMA_MATH_

#include <glm/glm.hpp>
#include <bullet/btBulletCollisionCommon.h>

glm::mat4 BulletToGLM(btTransform& transform)
{
	glm::mat4 glmTransform;
	transform.getOpenGLMatrix(glm::value_ptr(glmTransform));
	return glmTransform;
}

glm::vec3 BulletToGLM(const btVector3& vector3)
{
	return glm::vec3(vector3[0], vector3[1], vector3[2]);
}


#endif