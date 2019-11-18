#include "PhysicsDebug.h"

void ChromaPhysicsDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	std::cout << "drawing line from physics debug" << std::endl;
	//m_debugBuffer->drawLine(glm::vec3(from[0], from[1], from[2]),
	//	glm::vec3(to[0], to[1], to[2]), glm::vec3(color[0], color[1], color[2]));
}

void ChromaPhysicsDebug::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	std::cout << "drawing contact point from physics debug" << std::endl;
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
