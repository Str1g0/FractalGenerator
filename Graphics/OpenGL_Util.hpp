#pragma once

#include <GL\include\glad\glad.h>
#include <GL\include\glad\glad_wgl.h>
#include <GL\include\KHR\khrplatform.h>

#include <Util.h>
#include <App/Logging.h>
#include <Math/Vector.inl>

#define GL_CALL(func, ...)	\
{							\
	func(...);				\
	PrintOpenGLErrors();	\
} 


namespace Graphics
{
		typedef GLuint GpuHandleID;
		
		std::string ErrorTranslator(GLenum errorCode);
		void PrintOpenGLErrors();

}
