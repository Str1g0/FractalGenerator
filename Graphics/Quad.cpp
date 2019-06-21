#include "Quad.hpp"

void Graphics::Quad::CreateGeometry()
{
	glGenBuffers(1, &m_geometryBuffer);
	glGenVertexArrays(1, &m_attributeArray);

	glBindVertexArray(m_attributeArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_geometryBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_COORDINATES_NDC), &QUAD_COORDINATES_NDC, GL_STATIC_DRAW);

	glEnableVertexAttribArray(POSITION_INDEX);
	glVertexAttribPointer(POSITION_INDEX, 2, GL_FLOAT, GL_FALSE, STRIDE, (GLvoid*)POSITION_OFFSET);
	
	glEnableVertexAttribArray(TEXCOORD_INDEX);
	glVertexAttribPointer(TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, STRIDE, (GLvoid*)TEXCOORD_OFFSET);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


Graphics::Quad::Quad()
{
	CreateGeometry();
}

Graphics::Quad::~Quad()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &m_geometryBuffer);
	glDeleteVertexArrays(1, &m_attributeArray);
}

Graphics::Quad::Quad(const Quad & other)
{
	CreateGeometry();
}

Graphics::Quad Graphics::Quad::operator=(const Quad & other)
{
	return Quad();
}

void Graphics::Quad::Draw(ShaderProgramPtr renderer)
{
	renderer->Use();
	glBindVertexArray(m_attributeArray);
	glDrawArrays(GL_TRIANGLES, 0, N_VERTICES);
}
