#include "Light.h"



void Light::updatePointRadius()
{
	diffuse *= intensity;
	lightMax = std::fmaxf(std::fmaxf(diffuse.r, diffuse.b), diffuse.b);
	radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax))) / (2 * quadratic);
}

Light::Light()
{
	updatePointRadius();
}


Light::~Light()
{
}
