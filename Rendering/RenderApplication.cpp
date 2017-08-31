#include "RenderApplication.h"
#include "gl_core_4_4.h"

#include <cstdio>
#include "ext.hpp"


void RenderApplication::generateGrid(unsigned int rows, unsigned int cols)
{
	m_row = rows;
	m_column = cols;
	Vertex * aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; r++)
		for (unsigned int c = 0; c < cols; c++)
		{
			aoVertices[r * cols + c].position = glm::vec4((float)c, 0, (float)r, 1);
			glm::vec3 color = glm::vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));
			aoVertices[r * cols * c].color = glm::vec4(color, 1);
		}
	const unsigned int numitems = (rows - 1) * (cols - 1) * 6;
	auto auiIndices = new unsigned int[numitems];

	unsigned int index = 0;
	for (unsigned int r = 0; r < rows - 1; r++)
		for (unsigned int c = 0; c < cols - 1; c++)
		{
			//Triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			//Triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}


	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Generate VBO
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);



	//Index Buffer
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	


	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	




	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	cam->setPerspective(1, 16 / 9, 0, 100);	
	indexCount = (rows - 1) * (cols - 1) * 6;


	delete[] aoVertices;
	delete[] auiIndices;
}

RenderApplication::RenderApplication()
{
}


RenderApplication::~RenderApplication()
{
}

void RenderApplication::startup()
{

	generateGrid(5, 5);
	//create shaders
	const char * vsSource = "#version 410\n \
							layout(location=0) in glm::vec4 position; \
							layout(location=1) in glm::vec4 color; \
							out glm::vec4 vColor; \
							uniform glm::mat4 projectionViewWorldMatrix; \
							void main() { vColor = color; gl_Position = projectionViewWorldMatrix * position; }";

	const char * fsSource = "#version 410\n \
							in glm::vec4 vColor; \
							out glm::vec4 fragColor; \
							void main() { fragColor = vColor; }";
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();

	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
}

void RenderApplication::shutdown()
{
}

void RenderApplication::update(float)
{
}

void RenderApplication::draw()
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(m_programID);
	int mvpUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	cam->setPosition(glm::vec3(0, 4, 10));
	glm::mat4 pv = cam->getProjectionView();
	glm::mat4 model = glm::mat4(1);
	glm::mat4 mvp = pv * model;
	glUniformMatrix4fv(mvpUniform, 1, false, glm::value_ptr(mvp));

	glBindVertexArray(m_VAO);	
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);	
	glBindVertexArray(0);
	glUseProgram(0);
}
