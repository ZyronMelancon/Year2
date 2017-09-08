#include "RenderApplication.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "Mesh.h"

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

Mesh * transarrow;

std::vector<glm::vec4> genHalfCircle(float radius, int np)
{
	std::vector<glm::vec4> vecs;
	if (np < 3 || radius == 0)
	{
		return vecs;
	}

	for (int i = 0; i < np; i++)
	{
		float rad = glm::pi<float>() / (np - 1) * i;
		vecs.push_back(glm::vec4(sin(rad), cos(rad), 0, 1));
	}

	return vecs;
}

std::vector<glm::vec4> rotatePoints(std::vector<glm::vec4> points, int nm)
{
	std::vector<glm::vec4> vecs;
	if (nm < 4 || points.size() == 0)
		return vecs;

	for (int i = 0; i <= nm; i++)
		for (unsigned int o = 0; o < points.size(); o++)
		{
			float rad = 2 * glm::pi<float>() / nm * i;
			float newX = points[o].x * cos(rad);
			float newY = points[o].y;
			float newZ = points[o].x * -sin(rad);
			vecs.push_back(glm::vec4(newX, newY, newZ, 1));
		}

	return vecs;
}

void RenderApplication::generateGrid(unsigned int rows, unsigned int cols)
{
	m_row = rows;
	m_column = cols;
	Vertex * aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
		for (unsigned int c = 0; c < cols; ++c)
		{
			aoVertices[r * cols + c].position = glm::vec4((float)c, 0, (float)r, 1);
			glm::vec3 color = glm::vec3((float)r/rows, (float)c/cols, 1);
			aoVertices[r * cols + c].color = glm::vec4(color, 1);
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

	grid = new Mesh();

	grid->initialize(verts, indices);
	
	grid->create_buffers();

	gridCount = (rows - 1) * (cols - 1) * 6;


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
	std::vector<glm::vec4> thingy;

	thingy.push_back(glm::vec4(.2f, 0, 0, 1));
	thingy.push_back(glm::vec4(.2f, 2, 0, 1));
	thingy.push_back(glm::vec4(.35f, 2, 0, 1));
	thingy.push_back(glm::vec4(0, 3, 0, 1));

	std::vector<glm::vec4> thingy2 = rotatePoints(thingy, 8);



	std::vector<Vertex> transArrowVerts;

	for (int i = 0; i < thingy2.size(); i++)
	{
		Vertex thing;
		thing.position = thingy2[i];
		thing.color = glm::vec4(1, 1, 1, 1);
		transArrowVerts.push_back(thing);
	}



	std::vector<unsigned int> transIndices;

	for (unsigned int i = 0; i < 8; i++)
	{
		for (unsigned int o = 0; o < 4; o++)
		{
			unsigned int botL = i * 4 + o;
			transIndices.push_back(botL);
			transIndices.push_back(botL + 4);
		}
	}

	transarrow = new Mesh();
	transarrow->initialize(transArrowVerts, transIndices);
	transarrow->create_buffers();

	//Sphere generation
	int numPoints = 20;
	int numMeridian = 20;
	float radius = 2;

	std::vector<unsigned int> indices;
	std::vector<glm::vec4> diamondpoints = rotatePoints(genHalfCircle(radius, numPoints), numMeridian);
	std::vector<Vertex> TheSphere;
	
	for (unsigned int i = 0; i < diamondpoints.size(); i++)
	{
		Vertex zoz;
		zoz.position = diamondpoints[i];
		zoz.color = glm::vec4(1,1,1,1);
		TheSphere.push_back(zoz);
	}

	for (unsigned int i = 0; i < numMeridian; i++)
	{
		for (unsigned int o = 0; o < numPoints; o++)
		{
			unsigned int botL = i * numMeridian + o;
			indices.push_back(botL);
			indices.push_back(botL + numMeridian);
		}
	}

	m_mesh = new Mesh();

	m_mesh->initialize(TheSphere, indices);

	m_mesh->create_buffers();
	m_camera->setPerspective(1, 16 / 9, 0, 100);
	m_camera->setPosition(glm::vec3(1, 1, 10));




	//Gen grid
	generateGrid(7, 7);



	//Create shaders
	const char * vsSource = "#version 410\n \
							layout(location=0) in vec4 position; \
							layout(location=1) in vec4 color; \
							out vec4 vColor; \
							uniform mat4 projectionViewWorldMatrix; \
							void main() { vColor = color; gl_Position = projectionViewWorldMatrix * position; }";

	const char * fsSource = "#version 410\n \
							in vec4 vColor; \
							out vec4 fragColor; \
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


static double c_mx, c_my; //Current mouse pos
static double p_mx, p_my; //Previous mouse pos
static double d_mx, d_my; //Delta mouse
void RenderApplication::update(float deltaTime)
{


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
		printf("Mouse speed: %f, %f\n", d_mx, d_my);

		m_camera->setRotationX(d_mx / 800);
	}

	//Camera movement
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x + m_camera->getWorldTransform()[0].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z - m_camera->getWorldTransform()[2].z * deltaTime * 6);
		m_camera->setPosition(move);
		printf("W pressed!\n");
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x - m_camera->getWorldTransform()[0].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z + m_camera->getWorldTransform()[2].z * deltaTime * 6);
		m_camera->setPosition(move);
		printf("S pressed!\n");
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x - m_camera->getWorldTransform()[2].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z - m_camera->getWorldTransform()[0].z * deltaTime * 6);
		m_camera->setPosition(move);
		printf("A pressed!\n");
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x + m_camera->getWorldTransform()[2].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z + m_camera->getWorldTransform()[0].z * deltaTime * 6);
		m_camera->setPosition(move);
		printf("D pressed!\n");
	}
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x, m_camera->getWorldTransform()[3].y + 6 * deltaTime, m_camera->getWorldTransform()[3].z);
		m_camera->setPosition(move);
		printf("Space pressed!\n");
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x, m_camera->getWorldTransform()[3].y - 6 * deltaTime, m_camera->getWorldTransform()[3].z);
		m_camera->setPosition(move);
		printf("LCtrl pressed!\n");
	}
}

void RenderApplication::draw()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::Begin("menu");
	ImGui::Text("t12");

	ImGui::End();

	if (ImGui::Button("hello world"))
		printf("hello guvanaana\n");
	

	glUseProgram(m_programID);
	int mvpUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glm::mat4 pv = m_camera->getProjectionView();
	glm::mat4 model = glm::mat4(1);
	glm::mat4 mvp = pv * model;
	
	
	glm::mat4 move = glm::translate(glm::vec3(3, 1, 3));
	glm::mat4 rotate90Z = glm::rotate(glm::mat4(1),-glm::pi<float>()/2, glm::vec3(0, 0, 1));
	glm::mat4 rotate90X = glm::rotate(glm::mat4(1), glm::pi<float>() / 2, glm::vec3(1, 0, 0));
	glUniformMatrix4fv(mvpUniform, 1, false, glm::value_ptr(mvp * move));
	m_mesh->bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLE_STRIP, m_mesh->index_count, GL_UNSIGNED_INT, 0);
	//glUniformMatrix4fv(mvpUniform, 1, false, glm::value_ptr(mvp));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDrawElements(GL_TRIANGLE_STRIP, m_mesh->index_count, GL_UNSIGNED_INT, 0);
	m_mesh->unbind();

	transarrow->bind();
	glUniformMatrix4fv(mvpUniform, 1, false, glm::value_ptr(mvp));
	glDrawElements(GL_TRIANGLE_STRIP, transarrow->index_count, GL_UNSIGNED_INT, 0);
	glUniformMatrix4fv(mvpUniform, 1, false, glm::value_ptr(mvp * rotate90Z));
	glDrawElements(GL_TRIANGLE_STRIP, transarrow->index_count, GL_UNSIGNED_INT, 0);
	glUniformMatrix4fv(mvpUniform, 1, false, glm::value_ptr(mvp * rotate90X));
	glDrawElements(GL_TRIANGLE_STRIP, transarrow->index_count, GL_UNSIGNED_INT, 0);
	transarrow->unbind();

	glUniformMatrix4fv(mvpUniform, 1, false, glm::value_ptr(mvp));
	grid->bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, grid->index_count, GL_UNSIGNED_INT, 0);
	grid->unbind();



	glBindVertexArray(0);
	glUseProgram(0);
}