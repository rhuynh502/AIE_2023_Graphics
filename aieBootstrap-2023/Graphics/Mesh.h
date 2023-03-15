#pragma once
#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh() : 
		m_triCount(0), m_vao(0), m_vbo(0), m_ibo(0){};
	virtual ~Mesh();

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	// Will be used to make a simple quad
	void InitialiseQuad();

	// Will be used for Post Processing
	void InitialiseFullScreenQuad();

	void InitialiseBox(glm::vec4 center, float extent);

	// Will be used for implementing a primitive
	void Initialise(unsigned int _vertexCount, const Vertex* _vertices,
		unsigned int _indexCount = 0, unsigned int* _indices = nullptr);

	virtual void Draw();

protected:
	unsigned int m_triCount;
	unsigned int m_vao; // the Vertex Array Object
	unsigned int m_vbo; // the Vertex Buffer Object
	unsigned int m_ibo; // the Index Buffer Object
};

