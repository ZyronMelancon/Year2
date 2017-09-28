#define GLM_FORCE_SWIZZLE
#include "TextureApplication.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Mesh.h"
#include "Shader.h"
#include "glm.hpp"
#include "ext.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <FlyCamera.h>
#include <gl_core_4_4.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <gl_core_4_4.h>


std::vector<Vertex> genVertices(std::vector<glm::vec4> vecs, std::vector<glm::vec4> colors, std::vector<glm::vec2> uv)
{
	std::vector<Vertex> verts;
	for (int i = 0; i < vecs.size(); i++)
	{
		Vertex vert;
		vert.position = vecs[i];
		vert.color = colors[i];
		vert.normal = normalize(vecs[i]);
		vert.tan = glm::vec4(1, 1, 1, 1);
		vert.uv = uv[i];
		verts.push_back(vert);
	}
	return verts;
}

TextureApplication::TextureApplication()
{
}

TextureApplication::~TextureApplication()
{
}

glm::vec3 kA;
glm::vec3 kD;
glm::vec3 kS;
glm::vec3 iA;
glm::vec3 iD;
glm::vec3 iS;
GLuint tex;
void TextureApplication::startup()
{
	//Mesh setup
	{
		m_mesh = new Mesh();
		std::vector<glm::vec4> points;
		points.push_back(glm::vec4(0, 0, 0, 1));
		points.push_back(glm::vec4(1, 0, 0, 1));
		points.push_back(glm::vec4(0, 0, 1, 1));
		points.push_back(glm::vec4(1, 0, 1, 1));

		std::vector<glm::vec4> color;
		for (int i = 0; i < 4; i++)
			color.push_back(glm::vec4(1, 1, 1, 1));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(0, 1));
		uv.push_back(glm::vec2(1, 1));

		std::vector<unsigned> indices;
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);
		std::vector<Vertex> verts = genVertices(points, color, uv);
		m_mesh->initialize(verts, indices);
		m_mesh->create_buffers();
	}

	//Shader setup
	{
		m_shader = new Shader();

		m_shader->load("./shaders/fs.frag", GL_FRAGMENT_SHADER);
		m_shader->load("./shaders/vs.vert", GL_VERTEX_SHADER);
		m_shader->attach();

		kA = glm::vec3(1, 1, 1);
		kD = glm::vec3(1, 1, 1);
		kS = glm::vec3(1, 1, 1);

		iA = glm::vec3(.5f, .5f, .5f);
		iD = glm::vec3(1, 1, 1);
		iS = glm::vec3(1, 1, 1);

		/*m_shader->bind();
		unsigned int handle = m_shader->getUniform("iSpecPower");
		glUniform1f(handle, 50);

		handle = m_shader->getUniform("kA");
		glUniform3fv(handle, 1, &kA[0]);
		handle = m_shader->getUniform("kD");
		glUniform3fv(handle, 1, &kD[0]);
		handle = m_shader->getUniform("kS");
		glUniform3fv(handle, 1, &kS[0]);
		handle = m_shader->getUniform("iA");
		glUniform3fv(handle, 1, &iA[0]);
		handle = m_shader->getUniform("iD");
		glUniform3fv(handle, 1, &iD[0]);
		handle = m_shader->getUniform("iS");
		glUniform3fv(handle, 1, &iS[0]);

		m_shader->unbind();*/
	}

	//Camera Setup
	{
		m_camera = new FlyCamera();
		m_camera->setPerspective(1, 4 / 3, 1, 100);
		m_camera->setPosition(glm::vec3(3, 2, 3));
		m_camera->setLookAt(m_camera->getPosition(), glm::vec3(.5f, 0, .5f), glm::vec3(0, 1, 0));
	}

	//Image Setup



	int width, height, format;
	unsigned char* image = stbi_load("crate.png", &width, &height, &format, STBI_rgb);

	glGenTextures(1, &tex);
	
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
}

void TextureApplication::shutdown()
{
}

void TextureApplication::update(float deltaTime)
{
	glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x + m_camera->getWorldTransform()[2].z * deltaTime * 3, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z + m_camera->getWorldTransform()[0].z * deltaTime * 3);
	m_camera->setPosition(move);
	m_camera->setLookAt(m_camera->getPosition(), glm::vec3(.5f, 0, .5f), glm::vec3(0, 1, 0));

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x, m_camera->getWorldTransform()[3].y + 6 * deltaTime, m_camera->getWorldTransform()[3].z);
		m_camera->setPosition(move);
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x, m_camera->getWorldTransform()[3].y - 6 * deltaTime, m_camera->getWorldTransform()[3].z);
		m_camera->setPosition(move);
	}
}

void TextureApplication::draw()
{
	ImGui_ImplGlfwGL3_NewFrame();

	m_shader->bind();

	//unsigned int handle = m_shader->getUniform("camPos");
	//glUniform3fv(handle, 1, glm::value_ptr(m_camera->getPosition()));

	glm::mat4 mvp = m_camera->getProjectionView() * glm::mat4(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	glUniform1i(glGetUniformLocation(m_shader->program(), "tex"), 0);


	m_mesh->draw(m_shader->program(), GL_TRIANGLES, mvp);
	m_shader->unbind();
}