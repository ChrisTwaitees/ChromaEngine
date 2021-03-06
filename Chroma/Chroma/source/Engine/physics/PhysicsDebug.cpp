#include "PhysicsDebug.h"
#include <render/Render.h>

void PhysicsDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	Chroma::Render::GetDebugBuffer()->DrawLine(BulletToGLM(from), BulletToGLM(to), BulletToGLM(color));
}

void PhysicsDebug::drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color)
{
	Chroma::Render::GetDebugBuffer()->DrawBox(BulletToGLM(bbMin), BulletToGLM(bbMax), BulletToGLM(color));
}

void PhysicsDebug::drawBox(const btVector3& bbMin, const btVector3& bbMax, const btTransform& trans, const btVector3& color)
{
	Chroma::Render::GetDebugBuffer()->DrawBox(BulletToGLM(bbMin), BulletToGLM(bbMax), BulletToGLM(trans), BulletToGLM(color));
}

void PhysicsDebug::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

}

void PhysicsDebug::reportErrorWarning(const char* warningString)
{
	CHROMA_WARN("PHYSICS ERROR :: {0}", warningString);
}

void PhysicsDebug::draw3dText(const btVector3& location, const char* textString)
{
	std::cout << "Bullet Draw 3dText : " << textString << std::endl;
}

