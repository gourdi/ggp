#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <kernel/ggo_ease.h>
#include <2d/ggo_color.h>
#include "ggo_gpu_artist.h"

static const struct
{
  float x, y, z;
} vertices[] =
{
  {  1, -1, 0 },
  {  1,  1, 0 },
  { -1,  1, 0 },
  { -1, -1, 0 }
};

const GLubyte indices[] = {
    0, 1, 2,
    2, 3, 0
};

static const char* vertex_shader_text =
"#version 330 core\n"
"\n"
"layout(location = 0) in vec3 position; \n"
"\n"
"void main() {\n"
"  gl_Position.xyz = position; \n"
"}\n";


static void error_callback(int error, const char* description)
{
  std::cerr << "Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLuint compile_shader(GLenum type, const std::string & source)
{
  GLuint shader_id = glCreateShader(type);

  const char * source_ptr = source.c_str();
  glShaderSource(shader_id, 1, &source_ptr, NULL);
  glCompileShader(shader_id);

  GLint compile_status = 0;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
  if (!compile_status)
  {
    std::cerr << "shader compilation failed";

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0)
    {
      std::vector<char> log(log_length);

      glGetShaderInfoLog(shader_id, log_length, &log_length, log.data());

      std::cerr << std::string(log.data());
    }

    throw std::runtime_error("shader compilation failed");
  }

  return shader_id;
}

int main(void)
{
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  GLFWwindow*window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
  {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return -1;
  }
  glfwSwapInterval(1);

  GLuint vertex_buffer = 0;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);

  GLuint index_buffer = 0;
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);

  std::unique_ptr<ggo::gpu_artist> artist;
  GLuint program = 0;

  while (!glfwWindowShouldClose(window))
  {
    if (!artist)
    {
      const std::vector<ggo::gpu_artist_id> ids{
        ggo::gpu_artist_id::poupette,
        ggo::gpu_artist_id::plastic,
        ggo::gpu_artist_id::distorsion,
        ggo::gpu_artist_id::sonson,

      };
      int index = 3; // ggo::rand<int>(0, int(ids.size() - 1));

      artist.reset(ggo::gpu_artist::create(ids[index]));

      GLuint vertex_shader_id = compile_shader(GL_VERTEX_SHADER, vertex_shader_text);
      GLuint fragment_shader_id = compile_shader(GL_FRAGMENT_SHADER, artist->get_fragment_shader());

      program = glCreateProgram();
      glAttachShader(program, vertex_shader_id);
      glAttachShader(program, fragment_shader_id);
      glLinkProgram(program);
    }

    glUseProgram(program);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    bool finished = true;
    auto uniforms = artist->update(finished, { width, height });
    if (finished == true)
    {
      artist.reset();
    }
    else
    {
      GLint loc_resolution = glGetUniformLocation(program, "resolution");
      glUniform2i(loc_resolution, width, height);

      for (const auto & uniform : uniforms)
      {
        GLint loc = glGetUniformLocation(program, uniform.first.c_str());

        uniform.second.visit(
          [&](const int * ptr, size_t size, int n) {
          switch (n)
          {
          case 1: glUniform1iv(loc, GLsizei(size), ptr); break;
          case 2: glUniform2iv(loc, GLsizei(size), ptr); break;
          case 3: glUniform3iv(loc, GLsizei(size), ptr); break;
          }
        },
          [&](const float * ptr, size_t size, int n) {
          switch (n)
          {
          case 1: glUniform1fv(loc, GLsizei(size), ptr); break;
          case 2: glUniform2fv(loc, GLsizei(size), ptr); break;
          case 3: glUniform3fv(loc, GLsizei(size), ptr); break;
          }
        });
      }

      glClear(GL_COLOR_BUFFER_BIT);
      glBindVertexArray(vertex_buffer);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}