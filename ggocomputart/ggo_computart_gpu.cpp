#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <kernel/ggo_ease.h>
#include <2d/ggo_color.h>

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

static const char* fragment_shader_text =
"#version 330 core\n"
"\n"
"in vec4 gl_FragCoord;\n"
"uniform ivec2 resolution;\n"
"uniform vec3 c1;\n"
"uniform vec3 c2;\n"
"uniform vec3 c3;\n"
"uniform vec3 c4;\n"
"out vec4 glFragColor;\n"
"\n"
"void main()\n"
"{\n"
"  vec3 c5 = gl_FragCoord.x * c1 + (1 - gl_FragCoord.x) * c2;\n"
"  vec3 c6 = gl_FragCoord.x * c3 + (1 - gl_FragCoord.x) * c4;\n"
"  vec3 c7 = gl_FragCoord.y * c5 + (1 - gl_FragCoord.y) * c6;\n"
"  gl_FragColor = vec4(c7, 1);\n"
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

class shader
{
public:

  shader(const std::string & source) : _source(source) {}

  virtual GLenum type() const = 0;

  GLuint compile()
  {
    GLuint shader_id = glCreateShader(type());

    const char * source_ptr = _source.c_str();
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

private:

  const std::string _source;
};

class fragment_shader : public shader
{
public:

  fragment_shader(const std::string & source) : shader(source) {}

  GLenum type() const override { return GL_FRAGMENT_SHADER;  }
};

class vertex_shader : public shader
{
public:

  vertex_shader(const std::string & source) : shader(source) {}

  GLenum type() const override { return GL_VERTEX_SHADER; }
};

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

  vertex_shader vertex_shader(vertex_shader_text);
  GLuint vertex_shader_id = vertex_shader.compile();

  fragment_shader fragment_shader(fragment_shader_text);
  GLuint fragment_shader_id = fragment_shader.compile();

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader_id);
  glAttachShader(program, fragment_shader_id);
  glLinkProgram(program);

  const auto start = std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();




  constexpr float inf = 0.f;
  constexpr float sup = 2000.f;

  const float hue = ggo::rand<float>();

  auto create_color = [&]()
  {
    ggo::rgb_32f c = ggo::from_hsv<ggo::rgb_32f>(hue, ggo::rand<float>(), ggo::rand<float>());

    auto stretch = [&](float v)
    {
      return (sup - inf) * v + inf;
    };

    c.r() = stretch(c.r());
    c.g() = stretch(c.g());
    c.b() = stretch(c.b());

    return c;
  };


  ggo::rgb_32f c1_start = create_color();
  ggo::rgb_32f c2_start = create_color();
  ggo::rgb_32f c3_start = create_color();
  ggo::rgb_32f c4_start = create_color();

  ggo::rgb_32f c1_end = create_color();
  ggo::rgb_32f c2_end = create_color();
  ggo::rgb_32f c3_end = create_color();
  ggo::rgb_32f c4_end = create_color();

  constexpr float duration_ms = 5000;



  while (!glfwWindowShouldClose(window))
  {
    const auto now = std::chrono::steady_clock::now();

    auto elapsed_time_ms = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(now - start).count());

    float weight = ggo::ease_inout(elapsed_time_ms / duration_ms);
    ggo::rgb_32f c1 = ggo::linerp(c1_start, c1_end, weight);
    ggo::rgb_32f c2 = ggo::linerp(c1_start, c1_end, weight);
    ggo::rgb_32f c3 = ggo::linerp(c1_start, c1_end, weight);
    ggo::rgb_32f c4 = ggo::linerp(c1_start, c1_end, weight);



    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLint loc_resolution = glGetUniformLocation(program, "resolution");
    glUniform2i(loc_resolution, width, height);

    GLint loc_c1 = glGetUniformLocation(program, "c1");
    glUniform3f(loc_resolution, c1.r(), c1.g(), c1.b());
    GLint loc_c2 = glGetUniformLocation(program, "c2");
    glUniform3f(loc_resolution, c2.r(), c2.g(), c2.b());
    GLint loc_c3 = glGetUniformLocation(program, "c3");
    glUniform3f(loc_resolution, c3.r(), c3.g(), c3.b());
    GLint loc_c4 = glGetUniformLocation(program, "c4");
    glUniform3f(loc_resolution, c4.r(), c4.g(), c4.b());

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(vertex_buffer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}