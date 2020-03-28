#ifndef _CHROMA_MATH_
#define _CHROMA_MATH_

//common
#include <common/PrecompiledHeader.h>

#include <bullet/btBulletCollisionCommon.h>

#include <assimp/Importer.hpp>


// CONSTS
#define CHROMA_UP glm::vec3(0.0 ,1.0, 0.0)
#define CHROMA_DOWN glm::vec3(0.0 ,-1.0, 0.0)
#define CHROMA_FORWARD glm::vec3(0.0 ,0.0, 1.0)
#define CHROMA_BACK glm::vec3(0.0 ,0.0, -1.0)
#define CHROMA_RIGHT glm::vec3(1.0, 0.0, 0.0)
#define CHROMA_LEFT glm::vec3(-1.0 ,0.0, 0.0)

// UTILITIES
namespace Chroma
{
	class Math
	{
	public:
		static float Remap(float value, float inMin, float inMax, float outMin, float outMax)
		{
			return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
		}

		static float Remap01(float value, float inMin, float inMax)
		{
			return (value - inMin)  / (inMax - inMin);
		}

		static glm::quat GetQuatRotation(const glm::mat4& transform) 
		{ 
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(transform, scale, rotation, translation, skew, perspective);
			return glm::conjugate(rotation);
		}

		static glm::mat4 GetRotationMat4(glm::mat4 const& from)
		{
			glm::mat4 to = from;
			to[3] = glm::vec4(0.0,0.0,0.0,1.0);
			return to;
		}

		static glm::mat4 BuildMat4(glm::vec3 const& translation, glm::quat const& rotation, glm::vec3 const& scale)
		{
			glm::mat4 ident(1.0);
			ident = glm::translate(ident, translation);
			ident = ident * glm::toMat4(rotation);
			return glm::scale(ident, scale);
		}

		static glm::quat ToQuaternion(glm::mat4 const& transform)
		{
			float w, x, y, z;
			float diagonal = transform[0][0] + transform[1][1] + transform[2][2];
			if (diagonal > 0) {
				float w4 = (float)(glm::sqrt(diagonal + 1.0f) * 2.0f);
				w = w4 / 4.0f;
				x = (transform[2][1] - transform[1][2]) / w4;
				y = (transform[0][2] - transform[2][0]) / w4;
				z = (transform[1][0] - transform[0][1]) / w4;
			}
			else if ((transform[0][0] > transform[1][1]) && (transform[0][0] > transform[2][2])) {
				float x4 = (float)(glm::sqrt(1.0f + transform[0][0] - transform[1][1] - transform[2][2]) * 2.0f);
				w = (transform[2][1] - transform[1][2]) / x4;
				x = x4 / 4.0f;
				y = (transform[0][1] + transform[1][0]) / x4;
				z = (transform[0][2] + transform[2][0]) / x4;
			}
			else if (transform[1][1] > transform[2][2]) {
				float y4 = (float)(glm::sqrt(1.0f + transform[1][1] - transform[0][0] - transform[2][2]) * 2.0f);
				w = (transform[0][2] - transform[2][0]) / y4;
				x = (transform[0][1] + transform[1][0]) / y4;
				y = y4 / 4.0f;
				z = (transform[1][2] + transform[2][1]) / y4;
			}
			else {
				float z4 = (float)(glm::sqrt(1.0f + transform[2][2] - transform[0][0] - transform[1][1]) * 2.0f);
				w = (transform[1][0] - transform[0][1]) / z4;
				x = (transform[0][2] + transform[2][0]) / z4;
				y = (transform[1][2] + transform[2][1]) / z4;
				z = z4 / 4.0f;
			}
			return glm::quat(x, y, z, w);
		}

		static glm::vec3 GetScale(glm::mat4 const& transform)
		{
			float scaleX = glm::length(glm::vec3(transform[0][0], transform[0][1], transform[0][2]));
			float scaleY = glm::length(glm::vec3(transform[1][0], transform[1][1], transform[1][2]));
			float scaleZ = glm::length(glm::vec3(transform[2][0], transform[2][1], transform[2][2]));

			return glm::vec3(scaleX, scaleY, scaleZ);
		}

		inline static glm::vec3 GetTranslation(glm::mat4 const& transform) { return glm::vec3(transform[3]); }

		inline static glm::mat4 GetIdentityMatrix() { return glm::mat4(1.0f); };


		inline static float CartesianToPolar(float const& x, float const& y) { return -( glm::atan(y, x)); }

		static glm::vec3 RotateAroundPivot(glm::vec3 const& point, glm::vec3 const& pivot, glm::quat const& rotation)
		{
			glm::vec3 toPoint = point - pivot; // get vector to point
			toPoint = rotation * toPoint; // rotate it
			return pivot + toPoint; // shift pivot to new rotated position
		}

		static float GetFacingAngleEuler(glm::vec3 pivot, glm::vec3 lookAt)
		{
			float xa_xb = pivot.x - lookAt.x;
			float ya_yb = pivot.z - lookAt.z;

			float x = glm::sqrt(glm::pow(xa_xb, 2) + glm::pow(ya_yb, 2));

			// y dist simple
			float y = pivot.y - lookAt.y;

			return glm::degrees(Chroma::Math::CartesianToPolar(x, y)) * 2.0f;
		}

		static float InertiaForHeight(float const& gravityStrength, float const& desiredHeight)
		{
			return glm::sqrt(2.0f * gravityStrength * desiredHeight);
		}

		static float DegreesBetweenVectors(glm::vec3 const& vec1, glm::vec3 const& vec2)
		{
			return glm::acos(glm::dot(vec1, vec2));
		}

		static float DegreesBetweenVectors2D(glm::vec3 const& vec1, glm::vec3 const& vec2)
		{
			float degreesRotated = -glm::acos(glm::dot(vec1, vec2));
			if (glm::dot(glm::cross(vec2, CHROMA_UP), vec1) < 0.0f)
				degreesRotated *= -1.0;
			return degreesRotated;
			
		}

		static void TransposeViewToModelMatrixParticles(glm::mat4& modelMatrix, glm::mat4 const& viewMatrix)
		{
			// negates rotation on the model matrix from the view matrix by replacing the rotation portion of the 
			// model matrix with the transpose of the rotation of the view matrix as explained here : 
			// https://www.mathsisfun.com/algebra/matrix-introduction.html
			//https://www.youtube.com/watch?v=6PkjU9LaDTQ
			modelMatrix[0][0] = viewMatrix[0][0]; 
			modelMatrix[0][1] = viewMatrix[1][0];
			modelMatrix[0][2] = viewMatrix[2][0];

			modelMatrix[1][0] = viewMatrix[0][1];
			modelMatrix[1][1] = viewMatrix[1][1];
			modelMatrix[1][2] = viewMatrix[2][1];

			modelMatrix[2][0] = viewMatrix[0][2];
			modelMatrix[2][1] = viewMatrix[1][2];
			modelMatrix[2][2] = viewMatrix[2][2];

		}

	};
}



// CONVERSIONS

// ASSIMP
static inline glm::mat4 AIToGLM(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }


static glm::mat3 AIToGLM(const aiMatrix3x3& from)
{
	glm::mat3 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3;
	return to;
}

static inline glm::vec3 AIToGLM(const aiVector3D& v) { return glm::vec3(v.x, v.y, v.z); }

static inline glm::quat AIToGLM(const aiQuaternion& q) { return glm::quat(q.w, q.x, q.y, q.z); }

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

static glm::quat GetRotation(glm::mat4 const& mat4)
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