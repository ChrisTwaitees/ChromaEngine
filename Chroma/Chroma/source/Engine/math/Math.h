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
static glm::mat4 AIToGLM(const aiMatrix4x4& from)
{
	glm::mat4 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}

static glm::mat3 AIToGLM(const aiMatrix3x3& from)
{
	glm::mat3 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3;
	return to;
}


static glm::vec3 AItoGLM(const aiVector3D& aiVec3)
{
	return glm::vec3(aiVec3[0], aiVec3[1], aiVec3[2]);
}

static glm::quat AItoGLM(const aiQuaternion& aiQuat)
{
	return glm::quat(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
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
static glm::vec3 getScale(glm::mat4 const& mat4)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat4, scale, rotation, translation, skew, perspective);

	return scale;
}

static glm::vec3 getTranslation(glm::mat4 const& mat4)
{
	return glm::vec3(mat4[3]);
}


static glm::vec3 GLMGetTranslation(glm::mat4 const& mat4)
{
	return glm::vec3(mat4[3]);
}

static glm::quat getRotation(glm::mat4 const& mat4)
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