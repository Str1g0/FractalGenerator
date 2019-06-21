#include "GpuBuffer.hpp"

Engine::Graphics::VertexBufferObject::VertexBufferObject(std::vector<Vertex>& vertices, bool animated):
	m_noVertices(vertices.size())
{
	Generate();

	Bind();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
				 vertices.data(), animated ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	PrintOpenGLErrors();
	Unbind();
}

void Engine::Graphics::VertexBufferObject::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	PrintOpenGLErrors();
}

void Engine::Graphics::VertexBufferObject::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, NULL_BUFFER);
}

#pragma warning(disable : 4312)

void Engine::Graphics::VertexBufferObject::SetPositionAttribute(uint16 attribNo, uint32 offset)
{
	Bind();
	glVertexAttribPointer(attribNo, Vertex::COORD_PER_VERTEX,
						  GL_FLOAT, GL_FALSE,
						  sizeof(Vertex), 
						  (GLvoid*)(offset));
	PrintOpenGLErrors();
	Unbind();
}

void Engine::Graphics::VertexBufferObject::SetTexcoordAttribute(uint16 attribNo, uint32 offset)
{
	Bind();
	glVertexAttribPointer(attribNo, Vertex::TEXCD_PER_VERTEX,
						  GL_FLOAT, GL_FALSE,
						  sizeof(Vertex),
						  (GLvoid*)(offset));
	PrintOpenGLErrors();
	Unbind();
}

void Engine::Graphics::VertexBufferObject::SetNormalAttribute(uint16 attribNo, uint32 offset)
{
	Bind();
	glVertexAttribPointer(attribNo, Vertex::NORMS_PER_VERTEX,
						  GL_FLOAT, GL_FALSE,
						  sizeof(Vertex),
						  (GLvoid*)(offset));
	PrintOpenGLErrors();
	Unbind();
}

#pragma warning(default : 4312)

Engine::Graphics::ElementBufferObject::ElementBufferObject(std::vector<GLuint>& indices, bool animated):
	m_noElements(indices.size())
{
	Generate();
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), 
				animated ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	PrintOpenGLErrors();
}

void Engine::Graphics::ElementBufferObject::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
	PrintOpenGLErrors();
}

void Engine::Graphics::ElementBufferObject::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL_BUFFER);
}

Engine::Graphics::VertexArrayObject::VertexArrayObject(std::vector<Vertex> vertices, 
													   std::vector<GLuint> elements, 
													   bool animated)
{
	
	Generate();
	Bind();

	m_ebo = std::make_shared<ElementBufferObject>(elements, animated);
	m_vbo = std::make_shared<VertexBufferObject>(vertices, animated);
	SetVertexBufferAttributes();

	Unbind();
}

void Engine::Graphics::VertexArrayObject::Generate()
{
	glGenVertexArrays(1, &m_handle);
	PrintOpenGLErrors();

	Bind();
	if (glIsVertexArray(m_handle) != GL_TRUE)
		LOG_ERR(TAG, "Vertex array (id %u) is not valid!", m_handle);
	else
		LOG_DBG(TAG, "Vertex array (id %u) has been generated successfully", m_handle);
	Unbind();
}

void Engine::Graphics::VertexArrayObject::Bind()
{
	glBindVertexArray(m_handle);
	PrintOpenGLErrors();
}

void Engine::Graphics::VertexArrayObject::Unbind()
{
	glBindVertexArray(NULL_BUFFER);
}

void Engine::Graphics::VertexArrayObject::Destroy()
{
	Bind();
	for (auto i = 0u; i < Vertex::ATTRIB_PER_VERTEX; ++i)
		glDisableVertexAttribArray(i);
	Unbind();

	if(m_vbo)
		m_vbo->Destroy();
	
	if (m_ebo)
		m_ebo->Destroy();

	glDeleteVertexArrays(1, &m_handle);
	PrintOpenGLErrors();
}

bool Engine::Graphics::VertexArrayObject::Check()
{
	Bind();
	if (glIsVertexArray(m_handle) != GL_TRUE)
	{
		LOG_ERR(TAG, "Vertex array (id %u) is not valid!", m_handle);
		return false;
	}
	else
	{
		LOG_DBG(TAG, "Vertex array (id %u) is a valid vertex array", m_handle);
	}

	bool check;
	m_vbo->Bind();
	m_ebo->Bind();
	
	if (!m_vbo->Check())
	{
		check = false;
		LOG_ERR(TAG, "No vertex buffer attached to the array buffer");
	}

	if (!m_ebo->Check())
	{
		check = false;
		LOG_ERR(TAG, "No element buffer attached to the array buffer");
	}
	
	m_ebo->Unbind();
	m_vbo->Unbind();

	Unbind();

	return check;
}

void Engine::Graphics::VertexArrayObject::SetVertexBufferAttributes()
{
	uint8	attribIndex		= 0u;
	uint16	bufferOffset	= 0u;

	Bind();
	m_vbo->Bind();

	glEnableVertexAttribArray(attribIndex);
	PrintOpenGLErrors();
	m_vbo->SetPositionAttribute(attribIndex, bufferOffset);

	++attribIndex;
	bufferOffset += Vertex::COORD_PER_VERTEX * sizeof(GLfloat);

	glEnableVertexAttribArray(attribIndex);
	PrintOpenGLErrors();
	m_vbo->SetNormalAttribute(attribIndex, bufferOffset);

	++attribIndex;
	bufferOffset += Vertex::NORMS_PER_VERTEX * sizeof(GLfloat);

	glEnableVertexAttribArray(attribIndex);
	PrintOpenGLErrors();
	m_vbo->SetTexcoordAttribute(attribIndex, bufferOffset);

	m_vbo->Unbind();
	Unbind();
}

void Engine::Graphics::GpuBuffer::Generate()
{
	glGenBuffers(1, &m_handle);
	PrintOpenGLErrors();

	Bind();
	if (glIsBuffer(m_handle) != GL_TRUE)
		LOG_ERR(TAG, "Failed to generate gpu buffer!");
	Unbind();

	Check();
}

void Engine::Graphics::GpuBuffer::Destroy()
{
	glDeleteBuffers(1, &m_handle);
	PrintOpenGLErrors();
}

inline bool Engine::Graphics::GpuBuffer::Check()
{
	Bind();
	if (glIsBuffer(m_handle) != GL_TRUE)
	{
		PrintOpenGLErrors();
		LOG_ERR(TAG, "The buffer with id %u is not a valid gpu buffer!", m_handle);
		return false;
	}
	else {
		LOG_DBG(TAG, "The buffer with id %u is a valid gpu buffer.", m_handle);
		return true;
	}

	Unbind();
}