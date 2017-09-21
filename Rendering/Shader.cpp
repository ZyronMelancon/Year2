#include "Shader.h"
#include <gl_core_4_4.h>
#include <cstdio>

FILE* fp;
const char* readIt(const char* fileName)
{
	fopen_s(&fp, fileName, "r");
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	char* buffer = new char[size + 1];
	fseek(fp, 0, SEEK_SET);
	size_t numerator = fread(buffer, sizeof(char), size + 1, fp);
	fclose(fp);
	buffer[numerator] = '\0';
	const char* ret = buffer;
	return ret;
}

void writeIt(const char* filename, const char* write)
{
	fopen_s(&fp, filename, "w");
	fputs(write, fp);
	fclose(fp);
}

Shader::Shader()
{
}


Shader::~Shader()
{
}

void Shader::bind()
{
	glUseProgram(m_program);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::load(const char* filename, unsigned type)
{

	//Need that file reading though

	if(type == GL_VERTEX_SHADER)
	{
		vsSource = readIt(filename);
		m_vertexShader = glCreateShader(type);
		glShaderSource(m_vertexShader, 1, (const char **)&vsSource, 0);
		glCompileShader(m_vertexShader);
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		fsSource = readIt(filename);
		m_fragmentShader = glCreateShader(type);
		glShaderSource(m_fragmentShader, 1, (const char **)&fsSource, 0);
		glCompileShader(m_fragmentShader);
	}
}

void Shader::attach()
{
	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	glLinkProgram(m_program);
	int success = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(m_fragmentShader);
	glDeleteShader(m_vertexShader);
}

void Shader::defaultLoad()
{
	vsSource = "#version 410\n \
				layout(location=0) in vec4 position; \
				layout(location=1) in vec4 color; \
				out vec4 vColor; \
				uniform mat4 projectionViewWorldMatrix; \
				void main() { vColor = color; \
				gl_Position = projectionViewWorldMatrix * position; }";

	fsSource = "#version 410\n \
				in vec4 vColor; \
				out vec4 fragColor; \
				uniform vec4 zColor; \
				void main() { fragColor = vColor * zColor; }";

	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(m_vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(m_vertexShader);
	glShaderSource(m_fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(m_fragmentShader);

	this->attach();

	//glLinkProgram(m_program);
	//int success = GL_FALSE;
	//glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	//if (success == GL_FALSE) {
	//	int infoLogLength = 0;
	//	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
	//	char* infoLog = new char[infoLogLength];
	//	glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
	//	printf("Error: Failed to link shader program!\n");
	//	printf("%s\n", infoLog);
	//	delete[] infoLog;
	//}


	//glDeleteShader(m_fragmentShader);
	//glDeleteShader(m_vertexShader);
}

unsigned Shader::getUniform(const char* uni)
{
	return glGetUniformLocation(m_program, uni);
}

unsigned Shader::program()
{
	return m_program;
}