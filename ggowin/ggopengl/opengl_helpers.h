#ifndef __GGO_OPENGL_HELPERS__
#define __GGO_OPENGL_HELPERS__

#include <stdexcept>
#include <GL/glew.h>
#include <GL/freeglut.h>

class shader_error : public std::runtime_error
{
public:
  shader_error(const std::string & what) : runtime_error(what) {}
};

void ThrowOnGLError(const char* error_message);
GLuint LoadShader(const char* filename, GLenum shader_type);

#endif