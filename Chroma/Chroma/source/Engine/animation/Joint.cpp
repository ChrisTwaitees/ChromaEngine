#include "Joint.h"


void Joint::CalculateInverseModelBindTransforms(glm::mat4 parentModelBindTransform)
{
	glm::mat4 modelBindTransform = parentModelBindTransform * m_LocalBindTransform;
	m_ModelInverseBindTransform = glm::inverse(modelBindTransform);

	for (Joint child : m_ChildJoints)
	{
		child.CalculateInverseModelBindTransforms(modelBindTransform);
	}
}

Joint::Joint()
{
}


Joint::~Joint()
{
}
