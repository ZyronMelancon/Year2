#pragma once
#include <vector>
#include <glm.hpp>
struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
};

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();
	void create_buffers();
	void initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indices);
	void bind();
	void unbind();
	unsigned int index_count;
	unsigned int vertex_count;
	std::vector<unsigned int> m_indices;
	std::vector<Vertex> m_vertices;
private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
	
};