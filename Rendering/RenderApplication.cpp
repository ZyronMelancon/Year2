#include "RenderApplication.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "Mesh.h"

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include "gl_core_4_4.h"

Mesh * transarrow;
Mesh * cube;

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
		vecs.push_back(glm::vec4(sin(rad) * radius, cos(rad) * radius, 0, 1));
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
			//Todo:: should breakout this rad variable to be something meaningful  and maybe not do decimal mult 
			float rad = glm::two_pi<float>() / nm * i;
			float newX = points[o].x * cos(rad);
			float newY = points[o].y;
			float newZ = points[o].x * -sin(rad);
			vecs.push_back(glm::vec4(newX, newY, newZ, 1));
		}

	return vecs;
}

std::vector<unsigned int> genStripIndices(int np, int nm)
{
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < nm; i++)
	{
		for (unsigned int o = 0; o < np; o++)
		{
			unsigned int botL = i * np + o;
			indices.push_back(botL);
			indices.push_back(botL + np);
		}
	}

	return indices;
}

std::vector<Vertex> genVertices(std::vector<glm::vec4> vecs, glm::vec4 color)
{
	std::vector<Vertex> verts;
	for (int i = 0; i < vecs.size(); i++)
	{
		Vertex vert;
		vert.position = vecs[i];
		vert.color = color;
		verts.push_back(vert);
	}
	return verts;
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
	//Making transform arrow
	std::vector<glm::vec4> arrowShape;
	arrowShape.push_back(glm::vec4(0, 0, 0, 1));
	arrowShape.push_back(glm::vec4(.2f, 0, 0, 1));
	arrowShape.push_back(glm::vec4(.2f, 2, 0, 1));
	arrowShape.push_back(glm::vec4(.35f, 2, 0, 1));
	arrowShape.push_back(glm::vec4(0, 3, 0, 1));

	transarrow = new Mesh();
	transarrow->initialize(genVertices(rotatePoints(arrowShape, 16), glm::vec4(1,1,1,1)), genStripIndices(5,16));
	transarrow->create_buffers();


	//Manually making cube
	std::vector<glm::vec4> cubeVecs;
	{
		cubeVecs.push_back(glm::vec4(0, 0, 0, 1)); //0
		cubeVecs.push_back(glm::vec4(1, 0, 0, 1)); //1
		cubeVecs.push_back(glm::vec4(0, 1, 0, 1)); //2
		cubeVecs.push_back(glm::vec4(1, 1, 0, 1)); //3
		cubeVecs.push_back(glm::vec4(0, 0, 1, 1)); //4
		cubeVecs.push_back(glm::vec4(1, 0, 1, 1)); //5
		cubeVecs.push_back(glm::vec4(0, 1, 1, 1)); //6
		cubeVecs.push_back(glm::vec4(1, 1, 1, 1)); //7
	}
	std::vector<unsigned int> cubeInds;
	{
		//Front
		cubeInds.push_back(0);
		cubeInds.push_back(1);
		cubeInds.push_back(2);

		cubeInds.push_back(1);
		cubeInds.push_back(3);
		cubeInds.push_back(2);

		//Right
		cubeInds.push_back(1);
		cubeInds.push_back(5);
		cubeInds.push_back(3);

		cubeInds.push_back(5);
		cubeInds.push_back(7);
		cubeInds.push_back(3);

		//Left
		cubeInds.push_back(5);
		cubeInds.push_back(4);
		cubeInds.push_back(7);

		cubeInds.push_back(4);
		cubeInds.push_back(6);
		cubeInds.push_back(7);

		//Back
		cubeInds.push_back(4);
		cubeInds.push_back(0);
		cubeInds.push_back(6);

		cubeInds.push_back(0);
		cubeInds.push_back(2);
		cubeInds.push_back(6);

		//Top
		cubeInds.push_back(2);
		cubeInds.push_back(3);
		cubeInds.push_back(6);

		cubeInds.push_back(3);
		cubeInds.push_back(7);
		cubeInds.push_back(6);

		//Bottom
		cubeInds.push_back(4);
		cubeInds.push_back(5);
		cubeInds.push_back(0);

		cubeInds.push_back(5);
		cubeInds.push_back(1);
		cubeInds.push_back(0);
	}

	cube = new Mesh();
	cube->initialize(genVertices(cubeVecs, glm::vec4(1,1,1,1)), cubeInds);
	cube->create_buffers();


	//Sphere generation
	int numPoints = 50;
	int numMeridian = 50;
	float radius = 0.5f;

	m_mesh = new Mesh();

	m_mesh->initialize(genVertices(rotatePoints(genHalfCircle(radius,numPoints), numMeridian), glm::vec4(1,1,1,0.7f)), genStripIndices(numPoints, numMeridian));
	//I think Mr. Matthew will yell at me for this but it works...

	m_mesh->create_buffers();


	m_camera->setPerspective(1, 1, 0.1f, 100);
	m_camera->setPosition(glm::vec3(1, 1, 10));


	//Gen grid
	generateGrid(7, 7);

	m_shader = new Shader();

	//m_shader->defaultLoad();
	m_shader->load("./shaders/vs.vert", GL_VERTEX_SHADER);
	m_shader->load("./shaders/fsphong.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

}

void RenderApplication::shutdown()
{
}


static double c_mx, c_my; //Current mouse pos
static double p_mx, p_my; //Previous mouse pos
static double d_mx, d_my; //Delta mouse
float pastTime = 0;
void RenderApplication::update(float deltaTime)
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

		m_camera->setRotationX(d_mx / 800);
	}

	//Camera movement
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x + m_camera->getWorldTransform()[0].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z - m_camera->getWorldTransform()[2].z * deltaTime * 6);
		m_camera->setPosition(move);
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x - m_camera->getWorldTransform()[0].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z + m_camera->getWorldTransform()[2].z * deltaTime * 6);
		m_camera->setPosition(move);
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x - m_camera->getWorldTransform()[2].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z - m_camera->getWorldTransform()[0].z * deltaTime * 6);
		m_camera->setPosition(move);
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x + m_camera->getWorldTransform()[2].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z + m_camera->getWorldTransform()[0].z * deltaTime * 6);
		m_camera->setPosition(move);
	}
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
	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		m_camera->setLookAt(glm::vec3(m_camera->getWorldTransform()[3][0], m_camera->getWorldTransform()[3][1], m_camera->getWorldTransform()[3][2]), glm::vec3(0, 0, 0), glm::vec3(0,1,0));
	}


}

void RenderApplication::draw()
{
	//Hella RGB fade
	glm::vec4 colorfade = glm::vec4((sin(pastTime) / 2) + 0.5f, (sin(pastTime - glm::two_pi<float>() / 3) / 2) + 0.5f, (sin(pastTime - glm::two_pi<float>() / 3 * 2) / 2) + 0.5f, 1);

	//Depth stuff

	//GUI stuff
	ImGui_ImplGlfwGL3_NewFrame();
	{
	//ImGui::Begin("menu");
	//ImGui::Text("This is a test.");
	//ImGui::BulletText("This is also a test.");
	//ImGui::MenuItem("This too.", 0, false, true);
	//ImGui::End();

	//if (ImGui::Button("hello world"))
	//	printf("hello guvanaana\n");
	}
	ImGui::Begin("Fade Color");
	ImGui::SliderFloat3("color zColor", &colorfade[0], 0, 1);
	
	ImGui::SliderFloat("specular power", &specPower, 0, 256);
	ImGui::TextColored(ImVec4(colorfade.x, colorfade.y, colorfade.z, colorfade.w), glm::to_string(colorfade).c_str());
	ImGui::End();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	m_shader->bind();

	//Set MVP matrix
	int mvpUniform = m_shader->getUniform("projectionViewWorldMatrix");
	glm::mat4 pv = m_camera->getProjectionView();
	glm::mat4 mvp = pv * glm::mat4(1);

	//Set cam pos
	glm::vec3 campos = glm::vec3(m_camera->getWorldTransform()[3].x, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z);
	unsigned int hand = m_shader->getUniform("camPos");
	unsigned int specPowerHandle = m_shader->getUniform("iSpecPower");
	glUniform3fv(hand, 1, value_ptr(campos));
	ImGui::Begin("Camera Info");
	ImGui::SliderFloat3("pos", &campos[0], -9000, 9000);
	ImGui::End();
	glUniform1f(specPowerHandle, specPower);


	//Some extra matrices
	glm::mat4 move = glm::translate(glm::vec3(3, 1, 3));
	glm::mat4 move2 = glm::translate(glm::vec3(5,0,0));
	glm::mat4 rotate90Z = glm::rotate(glm::mat4(1),-glm::pi<float>()/2, glm::vec3(0, 0, 1));
	glm::mat4 rotate90X = glm::rotate(glm::mat4(1), glm::pi<float>() / 2, glm::vec3(1, 0, 0));
	
	//Apply hella RGB fade to fragment shader
	unsigned int handle = m_shader->getUniform("zColor");
	glUniform4fv(handle, 1, glm::value_ptr(colorfade));

	//handle = glGetUniformLocation(m_programID, "time");
	//glUniform1f(handle, pastTime);

	//Draw sphere
	m_mesh->draw(m_shader->program(), GL_TRIANGLE_STRIP, mvp);

	////Draw cube
	//cube->draw(m_shader->program(), GL_TRIANGLES, mvp*move2);

	////Draw transforms with each color
	glm::vec4 tmp = glm::vec4(0, 1, 0, 1);
	//glUniform4fv(handle, 1, &tmp[0]);
	//transarrow->draw(m_shader->program(), GL_TRIANGLE_STRIP, mvp);

	//tmp = glm::vec4(1, 0, 0, 1);
	//glUniform4fv(handle, 1, &tmp[0]);
	//transarrow->draw(m_shader->program(), GL_TRIANGLE_STRIP, mvp * rotate90Z);

	//tmp = glm::vec4(0, 0, 1, 1);
	//glUniform4fv(handle, 1, &tmp[0]);
	//transarrow->draw(m_shader->program(), GL_TRIANGLE_STRIP, mvp * rotate90X);

	//Set the color back to normal
	tmp = glm::vec4(1, 1, 1, 1);
	glUniform4fv(handle, 1, &tmp[0]);

	//Draw grid plane
	//grid->draw(m_shader->program(), GL_TRIANGLES, mvp);


	m_shader->unbind();
}

//ToDo:: shader to read from file 

//ToDo:: b/c writing code inside a really long string is not cool at all :(

//ToDo:: shader class to bind uniforms or at least get uniforms...
