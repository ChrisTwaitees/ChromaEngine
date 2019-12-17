#include "PhysicsDebug.h"

void ChromaPhysicsDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	m_debugBuffer->DrawLine(BulletToGLM(from), BulletToGLM(to), BulletToGLM(color));
}

void ChromaPhysicsDebug::drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color)
{
	m_debugBuffer->DrawBox(BulletToGLM(bbMin), BulletToGLM(bbMax), BulletToGLM(color));
}

void ChromaPhysicsDebug::drawBox(const btVector3& bbMin, const btVector3& bbMax, const btTransform& trans, const btVector3& color)
{
	m_debugBuffer->DrawBox(BulletToGLM(bbMin), BulletToGLM(bbMax), BulletToGLM(trans), BulletToGLM(color));
}

void ChromaPhysicsDebug::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

}

void ChromaPhysicsDebug::reportErrorWarning(const char* warningString)
{
	std::cout << "Bullet Error Warning : " << warningString << std::endl;
}

void ChromaPhysicsDebug::draw3dText(const btVector3& location, const char* textString)
{
	std::cout << "Bullet Draw 3dText : " << textString << std::endl;
}

void ChromaPhysicsDebug::setDebugMode(int debugMode)
{	
	m_debugMode = debugMode;
}

int ChromaPhysicsDebug::getDebugMode() const
{
	return m_debugMode;
}
