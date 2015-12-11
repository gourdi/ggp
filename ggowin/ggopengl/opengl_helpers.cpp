#include "opengl_helpers.h"
#include <fstream>
#include <sstream>

void ThrowOnGLError(const char* error_message)
{
  const GLenum ErrorValue = glGetError();

  if (ErrorValue != GL_NO_ERROR)
  {
    std::ostringstream oss;
    oss << error_message << ": " << gluErrorString(ErrorValue);
    throw shader_error(oss.str());
  }
}

GLuint LoadShader(const char* filename, GLenum shader_type)
{
  std::ifstream ifs(filename);
  std::istreambuf_iterator<char> input_it(ifs);
  std::istreambuf_iterator<char> eos;
  std::string content(input_it, eos);

  if (content.empty())
  {
    throw std::invalid_argument("failed opening file");
  }
  
  GLuint shader_id = glCreateShader(shader_type);
  if (shader_id == 0)
  {
    throw shader_error("failed creating shader");
  }

  const char * glsl_sources[1] = { content.c_str() };

  glShaderSource(shader_id, 1, glsl_sources, NULL);
  glCompileShader(shader_id);

  const GLenum error_value = glGetError();
  if (error_value != GL_NO_ERROR)
  {
    const char * error_msg = reinterpret_cast<const char*>(gluErrorString(error_value));
    throw shader_error(error_msg);
  }

  return shader_id;
}