#ifndef _CHROMA_PHYSICS_DEBUG_
#define _CHROMA_PHYSICS_DEBUG_

// stl
#include <iostream>
// thirdparty
#include <bullet/LinearMath/btIDebugDraw.h>
#include <glm/glm.hpp>
//chroma
#include <math/Math.h>
#include <buffer/DebugBuffer.h>


class PhysicsDebug : public btIDebugDraw
{
	DebugBuffer* m_debugBuffer;
	int m_debugMode;
public:
	virtual void BindDebugBuffer(DebugBuffer* const& debugBuffer) { m_debugBuffer = debugBuffer; };
	
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

	virtual void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color) override;
	virtual void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btTransform& trans, const btVector3& color) override;

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override ;
	virtual void reportErrorWarning(const char* warningString) override ;

	virtual void draw3dText(const btVector3& location, const char* textString) override ;
	virtual void setDebugMode(int debugMode) ;
	virtual int  getDebugMode() const  ; 
};

#endif