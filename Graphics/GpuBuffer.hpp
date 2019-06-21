#pragma once

#include <Util.h>
#include "OpenGL_Util.hpp"

namespace Engine
{
	namespace Graphics
	{
		struct GpuBuffer;
		struct VertexBufferObject;
		struct ElementBufferObject;
		struct VertexArrayObject;

		typedef std::shared_ptr<GpuBuffer>				GpuBufferPtr;
		typedef std::shared_ptr<VertexBufferObject>		VertexBufferObjectPtr;
		typedef std::shared_ptr<ElementBufferObject>	ElementBufferObjectPtr;
		typedef std::shared_ptr<VertexArrayObject>		VertexArrayObjectPtr;

		#pragma pack(1)
		struct Vertex
		{
			static constexpr uint8  COORD_PER_VERTEX  = 3U;
			static constexpr uint8  TEXCD_PER_VERTEX  = 2U;
			static constexpr uint8  NORMS_PER_VERTEX  = 3U;
			static constexpr uint8  ATTRIB_PER_VERTEX = 3U;
			static constexpr uint8  VALUES_PER_VERTEX = COORD_PER_VERTEX + TEXCD_PER_VERTEX + NORMS_PER_VERTEX;

			math::vec3f m_position;
			math::vec3f m_normals;
			math::vec2f m_texcoord;
		};

		struct GpuBuffer:
			public Misc::Noncopyable
		{
			virtual ~GpuBuffer() {
				Destroy();
			}

			static constexpr uint32 NULL_BUFFER = 0;

			virtual void Bind()			= 0;
			virtual void Unbind()		= 0;

			virtual void Generate();
			virtual void Destroy();
			virtual bool Check();

			inline const GLuint GetHandle() const {
				return m_handle;
			}

			inline const GLuint operator()() const {
				return m_handle;
			}

		protected:

			constexpr static auto TAG = "GpuBuffer";
			GLuint m_handle;
		};

		struct VertexBufferObject :
			public GpuBuffer
		{
			VertexBufferObject(std::vector<Vertex> & vertices, bool animated = false);
			
			template<size_t size>
			VertexBufferObject(const float (&vertices)[size], bool animated = false);

			void Bind()		override;
			void Unbind()	override;

			void SetPositionAttribute(uint16 attribNo, uint32 offset);
			void SetTexcoordAttribute(uint16 attribNo, uint32 offset);
			void SetNormalAttribute(uint16 attribNo, uint32 offset);

			uint64 inline GetSize() const {
				return m_noVertices;
			}

			private:

				uint64 m_noVertices{ 0u };
		};

		struct ElementBufferObject :
			public GpuBuffer
		{
			ElementBufferObject(std::vector<GLuint> & indices, bool animated = false);

			void Bind()		override;
			void Unbind()	override;

			uint64 inline GetSize() const {
				return m_noElements;
			}

			private:
				uint64 m_noElements{ 0u };
		};

		struct VertexArrayObject :
			public GpuBuffer
		{
			VertexArrayObject(std::vector<Vertex>  vertices,
							  std::vector<GLuint>  elements,
							  bool animated = false);

			void Generate()	override;
			void Bind()		override;
			void Unbind()	override;
			void Destroy()	override;
			bool Check()	override;

			inline uint64 GetNoVertices(){
				return m_vbo->GetSize();
			}

			inline uint64 GetNoIndices() {
				return m_ebo->GetSize();
			}

		private:

			static inline constexpr int8* BUFFER_OFFSET(int8 off) {
				return((int8*)NULL + off);
			}

			void SetVertexBufferAttributes();

			uint16					m_noAttribs	{Vertex::ATTRIB_PER_VERTEX};
			
			VertexBufferObjectPtr	m_vbo{nullptr};
			ElementBufferObjectPtr	m_ebo{nullptr};
		};

		struct Binder
		{
			Binder(GpuBufferPtr ptr) :
				m_buffer(ptr) {
				m_buffer->Bind();
			}

			virtual ~Binder() {
				m_buffer->Unbind();
			}

		private:
			GpuBufferPtr m_buffer;
		};

		template<size_t size>
		inline VertexBufferObject::VertexBufferObject(float const (&vertices)[size], bool animated)
		{
			Generate();

			Bind();
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex),
				vertices, animated ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
			PrintOpenGLErrors();
			Unbind();
		}

}
}