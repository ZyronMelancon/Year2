#include "Mesh.h"
#include <gl_core_4_4.h>
#include <ext.hpp>

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::create_buffers()
{
	//Generate VAO
	glGenVertexArrays(1, &m_vao);

	
	//Generate VBO
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindVertexArray(m_vao);
 	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indices)
{
	for (auto v : verts)
		m_vertices.push_back(v);
	vertex_count = m_vertices.size();
	
	for (auto i : indices)
		m_indices.push_back(i);
	index_count = m_indices.size();

}

void Mesh::bind()
{
	glBindVertexArray(m_vao);
	
}

void Mesh::unbind()
{
	glBindVertexArray(0);
	
}

void Mesh::draw(unsigned int program, unsigned int renderType, glm::mat4 mod)
{
	glBindVertexArray(m_vao);

	glUniformMatrix4fv(glGetUniformLocation(program, "projectionViewWorldMatrix"), 1, false, glm::value_ptr(mod));
	glDrawElements(renderType, this->index_count, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

 