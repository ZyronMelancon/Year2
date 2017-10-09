#include "GenerationApplication.h"
#include "gl_core_4_4.h"
#include "glm.hpp"
#include "ext.hpp"
#include <Mesh.h>
#include <Shader.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <GLFW/glfw3.h>
#include <gl_core_4_4.h>

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
				uvvec = glm::vec2(static_cast<float>(c) / cols, 0);
			else if (c == 0)
				uvvec = glm::vec2(0, static_cast<float>(r) / rows);
			else
				uvvec = glm::vec2(static_cast<float>(c) / cols, static_cast<float>(r) / rows);
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
	genGrid(64, 64, 0.4f, m_plane);
	m_plane->create_buffers();

	//Shader setup
	m_shader = new Shader();
	m_shader->load("shaders/vs.vert", GL_VERTEX_SHADER);
	m_shader->load("shaders/fs.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

	//Perlin setup
	int dims = 64;
	float * perlinData = new float[64 * 64];
	float scale = (1.0f / dims) * 5;
	int octaves = 6;

	//float g1, g2, g3, g4;
	//float u, v;

	//float x1 = glm::lerp(g1, g2, u);
	//float x2 = glm::lerp(g3, g4, u);

	//float average = glm::lerp(x1, x2, v);

	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			float amplitude = 1.f;
			float persistence = .3f;
			perlinData[y * dims + x] = 0;

			for (int o = 0; o < octaves; o++)
			{
				float freq = powf(2, (float)0);
				float perlinSample = perlin(glm::vec2((float)x, (float)y) * scale) * 0.5f + 0.5f;
				perlinData[y * dims + x] += perlinSample * amplitude;
				amplitude *= persistence;
			}
			
		}
	}
	
	glGenTextures(1, &m_perlinTex);
	glBindTexture(GL_TEXTURE_2D, m_perlinTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, perlinData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void GenerationApplication::shutdown()
{
}

static double c_mx, c_my; //Current mouse pos
static double p_mx, p_my; //Previous mouse pos
static double d_mx, d_my; //Delta mouse
float pastTime = 0;
void GenerationApplication::update(float deltaTime)
{
	pastTime += deltaTime;

	//Camera rotation
	bool pressed = false;
	glfwGetCursorPos(m_window, &c_mx, &c_my);
	d_mx = c_mx - p_mx;
	d_my = c_my - p_my;
	p_mx = c_mx;
	p_my = c_my;
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		pressed = true;
		glm::vec2 mousevector = glm::vec2(c_mx, c_my);
		//printf("Mouse speed: %f, %f\n", d_mx, d_my);

		m_cam->setRotationX(d_mx / 800);
	}
	{
		//Camera movement
		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
		{
			glm::vec3 move = glm::vec3(m_cam->getWorldTransform()[3].x + m_cam->getWorldTransform()[0].z * deltaTime * 6, m_cam->getWorldTransform()[3].y, m_cam->getWorldTransform()[3].z - m_cam->getWorldTransform()[2].z * deltaTime * 6);
			m_cam->setPosition(move);
		}
		if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
		{
			glm::vec3 move = glm::vec3(m_cam->getWorldTransform()[3].x - m_cam->getWorldTransform()[0].z * deltaTime * 6, m_cam->getWorldTransform()[3].y, m_cam->getWorldTransform()[3].z + m_cam->getWorldTransform()[2].z * deltaTime * 6);
			m_cam->setPosition(move);
		}
		if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
		{
			glm::vec3 move = glm::vec3(m_cam->getWorldTransform()[3].x - m_cam->getWorldTransform()[2].z * deltaTime * 6, m_cam->getWorldTransform()[3].y, m_cam->getWorldTransform()[3].z - m_cam->getWorldTransform()[0].z * deltaTime * 6);
			m_cam->setPosition(move);
		}
		if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
		{
			glm::vec3 move = glm::vec3(m_cam->getWorldTransform()[3].x + m_cam->getWorldTransform()[2].z * deltaTime * 6, m_cam->getWorldTransform()[3].y, m_cam->getWorldTransform()[3].z + m_cam->getWorldTransform()[0].z * deltaTime * 6);
			m_cam->setPosition(move);
		}
		if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			glm::vec3 move = glm::vec3(m_cam->getWorldTransform()[3].x, m_cam->getWorldTransform()[3].y + 6 * deltaTime, m_cam->getWorldTransform()[3].z);
			m_cam->setPosition(move);
		}
		if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			glm::vec3 move = glm::vec3(m_cam->getWorldTransform()[3].x, m_cam->getWorldTransform()[3].y - 6 * deltaTime, m_cam->getWorldTransform()[3].z);
			m_cam->setPosition(move);
		}
		if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
		{
			m_cam->setLookAt(glm::vec3(m_cam->getWorldTransform()[3][0], m_cam->getWorldTransform()[3][1], m_cam->getWorldTransform()[3][2]), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		}
	}
}

void GenerationApplication::draw()
{
	ImGui_ImplGlfwGL3_NewFrame();

	m_shader->bind();
	
	glUniform3fv(m_shader->getUniform("campos"), 1, value_ptr(m_cam->getPosition()));

	auto mvp = m_cam->getProjectionView() * glm::mat4(1);

	glUniformMatrix4fv(m_shader->getUniform("model"), 1, true, glm::value_ptr(glm::mat4(1)));
	glUniformMatrix4fv(m_shader->getUniform("projectionViewMatrix"), 1, true, glm::value_ptr(m_cam->getProjectionView()));

	glUniform1i(m_shader->getUniform("perlinTex"), 0);

	m_plane->draw(m_shader->program(), GL_TRIANGLES, mvp);

	m_shader->unbind();
}