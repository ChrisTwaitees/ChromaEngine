#ifndef _CHROMA_MATH_
#define _CHROMA_MATH_

#include <bullet/btBulletCollisionCommon.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

// CONSTS
#define CHROMA_UP glm::vec3(0.0 ,1.0, 0.0)
#define CHROMA_DOWN glm::vec3(0.0 ,-1.0, 0.0)
#define CHROMA_FORWARD glm::vec3(0.0 ,0.0, 1.0)
#define CHROMA_BACK glm::vec3(0.0 ,0.0, -1.0)
#define CHROMA_RIGHT glm::vec3(1.0 ,0.0, 0.0)
#define CHROMA_LEFT glm::vec3(-1.0 ,0.0, 0.0)

// ASSIMP
static glm::mat4 AIToGLM(const aiMatrix4x4& aiMat)
{
	return glm::mat4{
		aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
		aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
		aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
		aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
	};
}

static glm::vec3 AItoGLM(const aiVector3D& aiVec3)
{
	return glm::vec3(aiVec3[0], aiVec3[1], aiVec3[2]);
}

static glm::quat AItoGLM(const aiQuaternion& aiVec3)
{
	return glm::quat(aiVec3.w, aiVec3.x, aiVec3.y, aiVec3.z);
}

// BULLET
static glm::mat4 BulletToGLM(const btTransform& transform)
{
	glm::mat4 glmTransform;
	transform.getOpenGLMatrix(glm::value_ptr(glmTransform));
	return glmTransform;
}

static glm::mat4 BulletToGLM(btTransform& transform)
{
	glm::mat4 glmTransform;
	transform.getOpenGLMatrix(glm::value_ptr(glmTransform));
	return glmTransform;
}

static glm::vec3 BulletToGLM(const btVector3& vector3)
{
	return glm::vec3(vector3[0], vector3[1], vector3[2]);
}

static btTransform GLMToBullet(const glm::mat4& mat4)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat4, scale, rotation, translation, skew, perspective);

	btQuaternion orient(rotation.x, rotation.y, rotation.z, rotation.w);
	btVector3 translate(translation.x, translation.y, translation.z);
	return btTransform(orient, translate);
}

static btVector3 GLMToBullet(const glm::vec3& vec3)
{
	return btVector3(vec3.x, vec3.y, vec3.z);
}


// MATRICES
static glm::vec3 getScale(const glm::mat4 mat4)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat4, scale, rotation, translation, skew, perspective);

	return scale;
}

static glm::vec3 getTranslation(const glm::mat4 mat4)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat4, scale, rotation, translation, skew, perspective);

	return translation;
}

static glm::quat getRotation(const glm::mat4 mat4)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat4, scale, rotation, translation, skew, perspective);

	return rotation;
}

#endif