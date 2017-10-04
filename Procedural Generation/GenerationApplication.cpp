#include "GenerationApplication.h"
#include "gl_core_4_4.h"
#include "glm.hpp"
#include "ext.hpp"
#include <Mesh.h>
#include <Shader.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

GenerationApplication::GenerationApplication()
{
}

GenerationApplication::~GenerationApplication()
{
}

void genGrid(unsigned int rows, unsigned int cols, float spacing, Mesh *mesh)
{
	Vertex * aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
		for (unsigned int c = 0; c < cols; ++c)
		{
			aoVertices[r * cols + c].position = glm::vec4((float)c * spacing, 0, (float)r * spacing, 1);
			glm::vec3 color = glm::vec3((float)r / rows, (float)c / cols, 1);
			aoVertices[r * cols + c].color = glm::vec4(color, 1);
			aoVertices[r * cols + c].normal = glm::vec4(0, 1, 0, 1);
			aoVertices[r * cols + c].tan = glm::vec4(1);
			glm::vec2 uvvec = glm::vec2(0, 0);
			if (r == 0 && c == 0)
				uvvec = glm::vec2(0, 0);
			else if (r == 0)
				uvvec = glm::vec2(cols / c, 0);
			else if (c == 0)
				uvvec = glm::vec2(0, rows / r);
			else
				uvvec = glm::vec2(cols / c, rows / r);
			aoVertices[r * cols + c].uv = uvvec;
		}
	const unsigned int numitems = (rows - 1) * (cols - 1) * 6;
	auto auiIndices = new unsigned int[numitems];

	unsigned int index = 0;
	for (unsigned int r = 0; r < rows - 1; ++r)
		for (unsigned int c = 0; c < cols - 1; ++c)
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

	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < rows * cols; i++)
		verts.push_back(aoVertices[i]);

	for (unsigned int i = 0; i < numitems; i++)
		indices.push_back(auiIndices[i]);

	mesh->initialize(verts, indices);

	delete[] aoVertices;
	delete[] auiIndices;
}

GLuint m_perlinTex;
void GenerationApplication::startup()
{
	//Camera setup
	m_cam = new FlyCamera();
	m_cam->setPerspective(1, 900 / 650, 0.1f, 100);
	m_cam->setPosition(glm::vec3(1, 1, 10));

	//Plane setup
	m_plane = new Mesh();
	genGrid(20, 20, 0.4f, m_plane);
	m_plane->create_buffers();

	//Shader setup
	m_shader = new Shader();
	m_shader->load("shaders/vs.vert", GL_VERTEX_SHADER);
	m_shader->load("shaders/fs.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

	//Perlin setup
	int dims = 20;
	float * perlinData = new float[20 * 20];
	float scale = (1.0f / dims) * 3;
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			perlinData[y * dims + x] = perlin(glm::vec2(x, y) * scale) * 0.5f + 0.5f;
		}
	}
	
	glGenTextures(1, &m_perlinTex);
	glBindTexture(GL_TEXTURE_2D, m_perlinTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 20, 20, 0, GL_RED, GL_FLOAT, perlinData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void GenerationApplication::shutdown()
{
}

void GenerationApplication::update(float)
{
}

void GenerationApplication::draw()
{
	ImGui_ImplGlfwGL3_NewFrame();

	m_shader->bind();
	
	glUniform3fv(m_shader->getUniform("campos"), 1, value_ptr(m_cam->getPosition()));

	auto mvp = m_cam->getProjectionView() * glm::mat4(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlinTex);

	glUniform1i(m_shader->getUniform("perlinTex"), 0);

	m_plane->draw(m_shader->program(), GL_TRIANGLES, mvp);

	m_shader->unbind();
}
