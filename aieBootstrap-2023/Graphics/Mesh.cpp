#include "Mesh.h"
#include <gl_core_4_4.h>
#include "Gizmos.h"

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
	// Define the 6 vertices for our two triangles to make a quad,
	// in a counter-clockwise direction.
	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1.f };
	vertices[1].position = { 0.5f, 0, 0.5f, 1.f };
	vertices[2].position = { -0.5f, 0, -0.5f, 1.f };

	vertices[3].position = { -0.5f, 0, -0.5f, 1.f };
	vertices[4].position = { 0.5f, 0, 0.5f, 1.f };
	vertices[5].position = { 0.5f, 0, -0.5f, 1.f };

	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };
	vertices[4].normal = { 0, 1, 0, 0 };
	vertices[5].normal = { 0, 1, 0, 0 };

	// Fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices,
		GL_STATIC_DRAW);

	// Now we will enable the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Enable second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	// Next we unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// This is a quad made up of two triangles
	m_triCount = 2;

}

void Mesh::InitialiseBox(glm::vec4 center, float extent)
{
	Vertex vertices[36];

	float distFromCenter = sqrt(extent * extent);

	glm::vec3 vert1 = { center.x + distFromCenter, center.y + distFromCenter, center.z + distFromCenter};
	glm::vec3 vert2 = { center.x + distFromCenter, center.y - distFromCenter, center.z + distFromCenter};
	glm::vec3 vert3 = { center.x - distFromCenter, center.y - distFromCenter, center.z + distFromCenter};
	glm::vec3 vert4 = { center.x - distFromCenter, center.y + distFromCenter, center.z + distFromCenter};
	glm::vec3 vert5 = { center.x + distFromCenter, center.y + distFromCenter, center.z - distFromCenter};
	glm::vec3 vert6 = { center.x + distFromCenter, center.y - distFromCenter, center.z - distFromCenter};
	glm::vec3 vert7 = { center.x - distFromCenter, center.y - distFromCenter, center.z - distFromCenter};
	glm::vec3 vert8 = { center.x - distFromCenter, center.y + distFromCenter, center.z - distFromCenter};

	vertices[0].position = { vert1, center.w };
	vertices[1].position = { vert2, center.w };
	vertices[2].position = { vert3, center.w };

	vertices[3].position = { vert1, center.w };
	vertices[4].position = { vert3, center.w };
	vertices[5].position = { vert4, center.w };

	vertices[6].position = { vert5, center.w };
	vertices[7].position = { vert8, center.w };
	vertices[8].position = { vert2, center.w };
	
	vertices[9].position = { vert5, center.w };
	vertices[10].position = { vert2, center.w };
	vertices[11].position = { vert1, center.w };
		
	vertices[12].position = { vert6, center.w };
	vertices[13].position = { vert7, center.w };
	vertices[14].position = { vert8, center.w };
		
	vertices[15].position = { vert5, center.w };
	vertices[16].position = { vert7, center.w };
	vertices[17].position = { vert6, center.w };
		
	vertices[18].position = { vert4, center.w };
	vertices[19].position = { vert3, center.w };
	vertices[20].position = { vert7, center.w };
		
	vertices[21].position = { vert4, center.w };
	vertices[22].position = { vert7, center.w };
	vertices[23].position = { vert6, center.w };
		
	vertices[24].position = { vert5, center.w };
	vertices[25].position = { vert1, center.w };
	vertices[26].position = { vert4, center.w };
		
	vertices[27].position = { vert5, center.w };
	vertices[28].position = { vert4, center.w };
	vertices[29].position = { vert6, center.w };
		
	vertices[30].position = { vert2, center.w };
	vertices[31].position = { vert8, center.w };
	vertices[32].position = { vert7, center.w };
		
	vertices[33].position = { vert2, center.w };
	vertices[34].position = { vert7, center.w };
	vertices[35].position = { vert3, center.w };

	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_triCount = 12;
}

void Mesh::Initialise(unsigned int _vertexCount, const Vertex* _vertices, unsigned int _indexCount, unsigned int* _indices)
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

	// Fill the vertex Buffer
	glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(Vertex),
		_vertices, GL_STATIC_DRAW);

	// Enable the first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Enable second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	// Bind the indices if there are any defined
	if (_indexCount != 0)
	{
		glGenBuffers(1, &m_ibo);

		// Bind the vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		// Fill the vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount *
			sizeof(unsigned int), _indices, GL_STATIC_DRAW);

		m_triCount = _indexCount / 3;
	}
	else
	{
		m_triCount = _vertexCount / 3;
	}

	// Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
