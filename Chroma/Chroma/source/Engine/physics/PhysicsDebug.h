#ifndef _CHROMA_PHYSICS_DEBUG_
#define _CHROMA_PHYSICS_DEBUG_

// stl
#include <iostream>
// thirdparty
#include <bullet/LinearMath/btIDebugDraw.h>
#include <glm/glm.hpp>
//chroma
#include <math/ChromaMath.h>
#include <renderer/DebugRenderer.h>


class ChromaPhysicsDebug : public btIDebugDraw
{
	ChromaDebugRenderer* m_debugRender;
public:
	void bindDebugRenderer(ChromaDebugRenderer*& DebugRenderer) { m_debugRender = DebugRenderer; };

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {};
	void reportErrorWarning(const char* warningString) override {} ;
	void draw3dText(const btVector3& location, const char* textString) override {};
	void setDebugMode(int debugMode) override {};
	int  getDebugMode() const override { return 1; };

	void Render() { m_debugRender->Render(); };

	ChromaPhysicsDebug() {};
	~ChromaPhysicsDebug() {};
};

#endif