#ifndef _CHROMA_PHYSICS_DEBUG_
#define _CHROMA_PHYSICS_DEBUG_

//common
#include <common/PrecompiledHeader.h>
// thirdparty
#include <bullet/LinearMath/btIDebugDraw.h>
//chroma
#include <math/Math.h>
#include <buffer/DebugBuffer.h>
#include <common/CoreCommon.h>


class PhysicsDebug : public btIDebugDraw
{

public:
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

	virtual void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color) override;
	virtual void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btTransform& trans, const btVector3& color) override;

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
	virtual void reportErrorWarning(const char* warningString) override;

	virtual void draw3dText(const btVector3& location, const char* textString) override;
	virtual void setDebugMode(int debugMode) override { m_debugMode = debugMode; };
	virtual int  getDebugMode() const override { return m_debugMode; };

private:
	int m_debugMode;

};

#endif