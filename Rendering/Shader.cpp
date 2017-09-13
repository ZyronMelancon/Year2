#include "Shader.h"
#include <gl_core_4_4.h>


Shader::Shader()
{
}


Shader::~Shader()
{
}

void Shader::bind()
{
}

void Shader::unbind()
{
}

void Shader::load(const char* filename, unsigned type)
{
}

void Shader::attach()
{
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
}

void Shader::defaultLoad()
{
}

unsigned Shader::getUniform(const char*)
{
	return 'a';
}
