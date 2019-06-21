#include "Shader.hpp"

bool Graphics::ShaderProgram::HasUniform(cstring name, ValueType type)
{
	hash_map<Uniform>::iterator uniform = m_uniforms.find(name);

	if (uniform == m_uniforms.end())
	{
		LOG_WARN(TAG, "Unknown uniform!");
		return false;
	}

	if (uniform->second.type != type)
	{
		LOG_WARN(TAG, "Invalid uniform type");
		return false;
	}

	return true;
}

Graphics::ShaderProgram::ShaderProgram()
{
}

Graphics::ShaderProgram::~ShaderProgram()
{
	DetachAllStages();
	glDeleteProgram(m_program);
}

bool Graphics::ShaderProgram::RegisterUniform(cstring name, ValueType type, uint32_t lenght)
{
	if (m_uniforms.find(name) != m_uniforms.end())
	{
		LOG_WARN(TAG, "An uniform with the name \"%s\" already exists!", name);
		return false;
	}

	Uniform newUniform;
	newUniform.name = name;
	newUniform.type = type;
	newUniform.lenght = lenght;
	newUniform.handle = glGetUniformLocation(m_program, name);
	m_uniforms[name] = newUniform;

	PrintOpenGLErrors();
	return true;
}

bool Graphics::ShaderProgram::AddStage(ShaderStagePtr stage, ShaderType type)
{
	if (!m_hasLinked and stage->HasCompiled())
	{
		m_stages[ShaderStagePosition(type)] = stage;
		return true;
	}

	return false;
}

bool Graphics::ShaderProgram::IsCurrent()
{
	GLint m_currentProgram = INVALID;
	glGetIntegerv(GL_CURRENT_PROGRAM, &m_currentProgram);

	return m_program == m_currentProgram;
}

bool Graphics::ShaderProgram::IsValid()
{
	return glIsProgram(m_program) == GL_TRUE;
}

void Graphics::ShaderProgram::LinkProgram(bool detachStages)
{
	m_program = glCreateProgram();

	for (auto& stage : m_stages)
	{
		if (stage)
			glAttachShader(m_program, stage->GetID());
	}

	glLinkProgram(m_program);

	PrintOpenGLErrors();

	GLint hasLinked;

	glGetProgramiv(m_program, GL_LINK_STATUS, &hasLinked);

	m_hasLinked = hasLinked;

	if (!m_hasLinked)
	{
		char buffer[LOG_SIZE] = "";
		glGetProgramInfoLog(m_program, LOG_SIZE, NULL, buffer);
		LOG_ERR(TAG, "Failed to link shader program: %s", buffer);
	}

	if (detachStages)
	{
		for (auto & stage : m_stages)
		{
			if (stage)
				glDetachShader(m_program, stage->GetID());
		}

		m_hasDetached = true;
	}
}

bool Graphics::ShaderProgram::CheckShaderStatus()
{
	return IsValid() and m_hasLinked;
}

void Graphics::ShaderProgram::DetachAllStages()
{
	for (auto & stage : m_stages)
	{
		if (stage and stage->IsValid())
			glDetachShader(m_program, stage->GetID());
	}
}

void Graphics::ShaderProgram::DetachStage(ShaderType stage)
{
	glDetachShader(m_program,
		m_stages[ShaderStagePosition(stage)]->GetID());
}

void Graphics::ShaderProgram::Use()
{
	if (IsValid())
	{
		glUseProgram(m_program);
	}
}

void Graphics::ShaderProgram::operator()()
{
	if (IsValid())
	{
		glUseProgram(m_program);;
	}
}

void Graphics::ShaderProgram::SetUniformBool(cstring name, bool value)
{
	if (HasUniform(name, ValueType::BOOL))
	 glUniform1i(m_uniforms[name].handle, value);
}

void Graphics::ShaderProgram::SetUniformInt(cstring name, int32 value)
{
	if (HasUniform(name, ValueType::INT))
	 glUniform1i(m_uniforms[name].handle, value);
}

void Graphics::ShaderProgram::SetUniformUint(cstring name, uint32 value)
{
	if (HasUniform(name, ValueType::UINT))
	 glUniform1ui(m_uniforms[name].handle, value);
}

void Graphics::ShaderProgram::SetUniformFloat(cstring name, float value)
{
	if (HasUniform(name, ValueType::FLOAT))
	 glUniform1f(m_uniforms[name].handle, value);
}

void Graphics::ShaderProgram::SetUniformDouble(cstring name, double value)
{
	if (HasUniform(name, ValueType::DOUBLE)) return;
	else glUniform1d(m_uniforms[name].handle, value);
}

void Graphics::ShaderProgram::SetUniform2i(cstring name, math::vec2i value)
{
	if (HasUniform(name, ValueType::VEC2I))
	glUniform2i(m_uniforms[name].handle, value.x, value.y);
}

void Graphics::ShaderProgram::SetUniform3i(cstring name, math::vec3i value)
{
	if (HasUniform(name, ValueType::VEC3I))
	glUniform3i(m_uniforms[name].handle, value.x, value.y, value.z);
}

void Graphics::ShaderProgram::SetUniform4i(cstring name, math::vec4i value)
{
	if (HasUniform(name, ValueType::VEC4I))
	 glUniform4i(m_uniforms[name].handle, value.x, value.y, value.z, value.w);
}

void Graphics::ShaderProgram::SetUniform2u(cstring name, math::vec2u value)
{
	if (HasUniform(name, ValueType::VEC2U))
	 glUniform2ui(m_uniforms[name].handle, value.x, value.y);
}

void Graphics::ShaderProgram::SetUniform3u(cstring name, math::vec3u value)
{
	if (HasUniform(name, ValueType::VEC3U))
	 glUniform3ui(m_uniforms[name].handle, value.x, value.y, value.z);
}

void Graphics::ShaderProgram::SetUniform4u(cstring name, math::vec4u value)
{
	if (HasUniform(name, ValueType::VEC4U))
	 glUniform4ui(m_uniforms[name].handle, value.x, value.y, value.z, value.w);
}

void Graphics::ShaderProgram::SetUniform2f(cstring name, math::vec2f value)
{
	if (HasUniform(name, ValueType::VEC2F))
		glUniform2f(m_uniforms[name].handle, value.x, value.y);
}

void Graphics::ShaderProgram::SetUniform3f(cstring name, math::vec3f value)
{
	if (HasUniform(name, ValueType::VEC3F))
		glUniform3f(m_uniforms[name].handle, value.x, value.y, value.z);
}

void Graphics::ShaderProgram::SetUniform4f(cstring name, math::vec4f value)
{
	if (HasUniform(name, ValueType::VEC4F))
		glUniform4f(m_uniforms[name].handle, value.x, value.y, value.z, value.w);
}


Graphics::ShaderStage::ShaderStage(ShaderType type, ShaderCode source) :
	m_type(type)
{
	CompileStage(source.GetCode().c_str());
}

bool Graphics::ShaderStage::IsValid()
{
	return glIsShader(m_handle) == GL_TRUE;
}

bool Graphics::ShaderStage::HasCompiled()
{
	return m_hasCompiled;
}

void Graphics::ShaderStage::CompileStage(cstring content)
{
	m_handle = glCreateShader(ShaderTypeToGLEnum(m_type));

	if (!IsValid())
	{
		LOG_ERR(TAG, "Failed to create shader!");
		return;
	}

	glShaderSource(m_handle, 1, &content, NULL);
	glCompileShader(m_handle);

	PrintOpenGLErrors();

	GLint check;
	glGetShaderiv(m_handle, GL_COMPILE_STATUS, &check);

	m_hasCompiled = check;

	if (!check)
	{
		char buffer[LOG_SIZE] = "";
		glGetShaderInfoLog(m_handle, LOG_SIZE, NULL, buffer);
		LOG_ERR(TAG, "Shader compilation failed: %s", buffer);

		glDeleteShader(m_handle);
	}
}

void Graphics::ShaderStage::DeleteStage()
{
	if (IsValid())
	{
		glDeleteShader(m_handle);
	}
}

GLenum Graphics::ShaderStage::ShaderTypeToGLEnum(ShaderType type)
{
	switch (type)
	{
	case ShaderType::VERTEX:	return GL_VERTEX_SHADER;
	case ShaderType::TESS_EVAL: return GL_TESS_EVALUATION_SHADER;
	case ShaderType::TESS_CTRL: return GL_TESS_CONTROL_SHADER;
	case ShaderType::GEOMETRY:	return GL_GEOMETRY_SHADER;
	case ShaderType::FRAGMENT:  return GL_FRAGMENT_SHADER;
	case ShaderType::COMPUTING: return GL_COMPUTE_SHADER;
	default:					return NULL;
	}
}

Graphics::GpuHandleID Graphics::ShaderStage::GetID() const
{
	return m_handle;
}

constexpr int8 Graphics::ShaderStagePosition(ShaderType type)
{
	int8 position = 0;

	switch (type)
	{
	case ShaderType::COMPUTING:
		position++;
	case ShaderType::FRAGMENT:
		position++;
	case ShaderType::GEOMETRY:
		position++;
	case ShaderType::TESS_CTRL:
		position++;
	case ShaderType::TESS_EVAL:
		position++;
	case ShaderType::VERTEX:
		return position;
	default:
		return -1;
	}
}

void Graphics::ShaderCode::LoadFromFile(std::string filename)
{
	std::ifstream shaderSource(filename);
	std::stringstream convertor;

	convertor << shaderSource.rdbuf();
	m_source = convertor.str();
}

const std::string Graphics::ShaderCode::GetCode() const
{
	return m_source;
}
