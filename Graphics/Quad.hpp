#pragma once

#include "Shader.hpp"

namespace Graphics
{
	class Quad;
	typedef std::shared_ptr<Quad> QuadPtr;

	class Quad
	{
		protected:

		static constexpr float QUAD_COORDINATES_NDC[] =
		{
			-1.0f,  1.0f,  0.0f, 1.0f, // 0 
			-1.0f, -1.0f,  0.0f, 0.0f, // 1
			 1.0f, -1.0f,  1.0f, 0.0f, // 2

			-1.0f,  1.0f,  0.0f, 1.0f, // 0
			 1.0f, -1.0f,  1.0f, 0.0f, // 2
			 1.0f,  1.0f,  1.0f, 1.0f  // 3
		};

		static constexpr auto N_VERTICES	= 6;
		static constexpr auto STRIDE		= 4 * sizeof(GLfloat);

		static constexpr auto POSITION_INDEX = 0;
		static constexpr auto TEXCOORD_INDEX = 1;

		static constexpr uint16 POSITION_OFFSET = 0u;
		static constexpr uint16 TEXCOORD_OFFSET = (2u * sizeof(GLfloat));

		GpuHandleID m_geometryBuffer;
		GpuHandleID m_attributeArray;

		void CreateGeometry();

		public:

			Quad();
		    virtual ~Quad();

			Quad(const Quad & other);
			Quad operator=(const Quad & other);

			void virtual Draw(ShaderProgramPtr renderer);
	};
}

