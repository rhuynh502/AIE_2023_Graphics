#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::~Mesh()
{
	// If the array does not equal
	// zero, then delete the data

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void Mesh::InitialiseQuad()
{
	// Check if the mesh is not initialised
	assert(m_vao == 0);
	
	// Generate buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	// Bind the vertex array, this will be our mesh buffer
	glBindVertexArray(m_vao);

	// Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Define the 6 vertices for our two triangles to make a quad,
	// in a counter-clockwise direction.
	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1.f };
	vertices[1].position = { 0.5f, 0, 0.5f, 1.f };
	vertices[2].position = { -0.5f, 0, -0.5f, 1.f };

	vertices[3].position = { -0.5f, 0, -0.5f, 1.f };
	vertices[4].position = { 0.5f, 0, 0.5f, 1.f };
	vertices[5].position = { 0.5f, 0, -0.5f, 1.f };

	// Fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices,
		GL_STATIC_DRAW);

	// Now we will enable the first element as the poisition
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Next we unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// This is a quad made up of two triangles
	m_triCount = 2;

}

void Mesh::Draw()
{
	glBindVertexArray(m_vao);

	// Check if we are using indices or just vertex points
	if (m_ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, 
			3 * m_triCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);
	}
}
