#pragma once

#include "OpenGL_Util.hpp"
#include "../Math/Vector.inl"

namespace Graphics
{
	template<typename T>
	struct mat2x2;

	template<typename T>
	struct mat3x3;

	template<typename T>
	struct mat4x4;

	enum class ShaderType :
		byte
	{
		INVALID,
		VERTEX		= ENUM(0),
		TESS_CTRL	= ENUM(1),
		TESS_EVAL	= ENUM(2),
		GEOMETRY	= ENUM(3),
		FRAGMENT	= ENUM(4),
		COMPUTING	= ENUM(5)
	};

	enum class ValueType
	{
		INT, UINT, BOOL,
		FLOAT, DOUBLE,
		VEC2I, VEC3I, VEC4I,
		VEC2U, VEC3U, VEC4U,
		VEC2F, VEC3F, VEC4F,
		MAT2X2, MAT3X3, MAT4X4,
	};

	struct Uniform
	{
		GpuHandleID		handle;
		ValueType		type;
		uint32			lenght;
		cstring			name;
	};

	enum class SamplerType
	{
		SAMPLER_1D,
		SAMPLER_2D,
		SAMPLER_3D,
		SAMPLER_CUBE
	};

	struct Sampler
	{
		uint32		id;
		SamplerType type;
		cstring		name;
	};

	class ShaderProgram;
	class ShaderStage;
	class ShaderCode;

	typedef std::shared_ptr<ShaderProgram>	ShaderProgramPtr;
	typedef std::shared_ptr<ShaderStage>	ShaderStagePtr;

	constexpr auto INVALID = 0;

	constexpr int8 ShaderStagePosition(ShaderType type);

	struct Attribute
	{
		uint16		position;
		ValueType	type;
	};

	class ShaderCode
	{
		static constexpr cstring TOKENS = "{}; \n";

		std::string			  m_source;
		hash_map<Uniform>	  m_uniforms;

		std::string::iterator m_position;

	public:

		void LoadFromFile(std::string filename);
		const std::string GetCode() const;
	};

	class ShaderStage :
		Misc::Noncopyable
	{
		static constexpr auto  TAG = "OpenGL";

		ShaderType		m_type;
		GpuHandleID		m_handle;

		bool m_hasCompiled{ false };

	public:

		ShaderStage(ShaderType type, ShaderCode source);

		bool IsValid();
		bool HasCompiled();

		void CompileStage(cstring content);
		void DeleteStage();

		static GLenum ShaderTypeToGLEnum(ShaderType type);

		GpuHandleID GetID() const;
	};

	class ShaderProgram :
		Misc::Noncopyable
	{
		static constexpr uint8 MAX_STAGES = 6u;
		static constexpr auto  TAG = "OpenGL";

		ShaderType m_contents;
		GpuHandleID  m_program;

		hash_map<Uniform>		m_uniforms;
		std::vector<Sampler>	m_samplers;

		std::array<ShaderStagePtr, MAX_STAGES>	m_stages;

		bool m_hasLinked{ false };
		bool m_hasDetached{ false };

		bool HasUniform(cstring name, ValueType type);

	public:

		ShaderProgram();
		~ShaderProgram();

		bool RegisterUniform(cstring name, ValueType type, uint32_t lenght = 1u);
		bool AddStage(ShaderStagePtr stage, ShaderType type);

		bool IsCurrent();
		bool IsValid();

		void LinkProgram(bool detachStages = true);
		bool CheckShaderStatus();

		void DetachAllStages();
		void DetachStage(ShaderType stage);

		void Use();
		void operator()();

		void SetUniformBool		(cstring name,   bool	value);
		void SetUniformInt		(cstring name,	int32	value);
		void SetUniformUint		(cstring name,	uint32	value);
		void SetUniformFloat	(cstring name,	float	value);
		void SetUniformDouble	(cstring name,  double	value);

		void SetUniform2i(cstring name, math::vec2i value);
		void SetUniform3i(cstring name, math::vec3i value);
		void SetUniform4i(cstring name, math::vec4i value);

		void SetUniform2u(cstring name, math::vec2u value);
		void SetUniform3u(cstring name, math::vec3u value);
		void SetUniform4u(cstring name, math::vec4u value);

		void SetUniform2f(cstring name, math::vec2f value);
		void SetUniform3f(cstring name, math::vec3f value);
		void SetUniform4f(cstring name, math::vec4f value);
	};
}
