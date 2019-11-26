#include "IBL.h"



void IBL::initialize()
{
	m_shader.use();
	m_shader.setInt("equirectangularMap", 0);
}

void IBL::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_HDRtexture.ID);
	m_skyBox->Draw(m_shader);
}

IBL::IBL()
{
	initialize();
}


IBL::~IBL()
{
}
