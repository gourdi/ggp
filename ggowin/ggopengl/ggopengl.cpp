#include <iostream>
#include <sstream>
#include <vector>
#include <ggo_shapes3d.h>
#include <ggo_interpolation3d.h>
#include <ggo_marching_cubes.h>
#include <ggo_color.h>
#include <ggo_bmp.h>
#include "opengl_helpers.h"
#include "ggo_matrix4x4.h"
#include "ggo_camera.h"
#include "ggo_vertex_buffer.h"

#define WINDOW_TITLE_PREFIX "ggopengl"

#define GGOGL(zzz) { zzz; ThrowOnGLError(#zzz); }

float LightPosition[4] = {10, 10, 10, 1};
float LightColor[4] = {1, 1, 1, 1};

int
  CurrentWidth = 800,
  CurrentHeight = 600,
  WindowHandle = 0;

GLuint
  ProjectionMatrixUniformLocation,
  ViewMatrixUniformLocation,
  LightPositionUniformLocation,
  LightColorUniformLocation,
  buffer_name = 1,
  vertex_array_name = 1,
  shader_main, shader_vertex, shader_fragment;

//Matrix
ggo_camera camera;

ggo_vertex_buffer vertex_buffer;

void CreateShaders()
{
  shader_main = glCreateProgram();
  ThrowOnGLError("could not create the shader program");

  shader_fragment = LoadShader("../ggopengl/SimpleShader.fragment.glsl", GL_FRAGMENT_SHADER);
  shader_vertex = LoadShader("../ggopengl/SimpleShader.vertex.glsl", GL_VERTEX_SHADER);
  glAttachShader(shader_main, shader_fragment);
  glAttachShader(shader_main, shader_vertex);
  glLinkProgram(shader_main);
  ThrowOnGLError("could not link the shader program");

  ViewMatrixUniformLocation = glGetUniformLocation(shader_main, "ViewMatrix");
  ProjectionMatrixUniformLocation = glGetUniformLocation(shader_main, "ProjectionMatrix");
  LightPositionUniformLocation = glGetUniformLocation(shader_main, "LightPosition");
  LightColorUniformLocation = glGetUniformLocation(shader_main, "LightColor");
  ThrowOnGLError("could not get shader uniform locations");
}

void CreateIsosurface()
{
  vertex_buffer.clear();

  std::array<float, 16 * 16 * 16> noise3d;
  for (auto & value : noise3d)
  {
    value = ggo::rand_float(-1.f, 1.f);
  }

  auto func3d = [&](float x, float y, float z)
  {
    return z + ggo::bilinear_interpolation3d(noise3d.data(), 16, 16, 16, 25 * x, 25 * y, 25 * z);
  };

  auto triangle_groups = ggo::marching_cubes(func3d, ggo::point3d_float(-5.5f, -5.5f, -5.5f), 101, 0.1f);

  for (const auto & triangle_group : triangle_groups)
  {
    for (const auto & triangle : triangle_group)
    {
      ggo::vector3d_float normal{ 0.f, 0.f, 1.f };// ggo::cross(triangle.v3() - triangle.v1(), triangle.v2() - triangle.v1());

      vertex_buffer.add_vertex(triangle.v1(), ggo::color::WHITE, normal);
      vertex_buffer.add_vertex(triangle.v2(), ggo::color::WHITE, normal);
      vertex_buffer.add_vertex(triangle.v3(), ggo::color::WHITE, normal);
    }
  }

  // Generate the buffer.
  glGenBuffers(1, &buffer_name);
  ThrowOnGLError("could not generate the buffer objects");

  glBindBuffer(GL_ARRAY_BUFFER, buffer_name);
  glBufferData(GL_ARRAY_BUFFER, vertex_buffer.get_byte_size(), vertex_buffer.get_pointer(), GL_STATIC_DRAW);
  ThrowOnGLError("could not bind the vertex buffer object to the vertex array object");

  // Generate the vertex array.
  glGenVertexArrays(1, &vertex_array_name);
  ThrowOnGLError("could not generate the vertex array object");
  glBindVertexArray(vertex_array_name);
  ThrowOnGLError("could not bind the vertex array object");

  int point_offset = ggo_vertex_buffer::get_point_byte_offset();
  int color_offset = ggo_vertex_buffer::get_color_byte_offset();
  int normal_offset = ggo_vertex_buffer::get_normal_byte_offset();
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, ggo_vertex_buffer::get_stride_byte_size(), (GLvoid*)point_offset);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, ggo_vertex_buffer::get_stride_byte_size(), (GLvoid*)color_offset);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, ggo_vertex_buffer::get_stride_byte_size(), (GLvoid*)normal_offset);
  ThrowOnGLError("could not set vertex array object attributes");

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  ThrowOnGLError("could not enable vertex array object");

  glBindVertexArray(0);
}

void DrawScene()
{
  GGOGL(glUseProgram(shader_main));

  GGOGL(glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, camera.get_view_matrix().coefs()));

  // Update color.
  glUniform4fv(LightColorUniformLocation, 1, LightColor);
  glUniform4fv(LightPositionUniformLocation, 1, LightPosition);
  ThrowOnGLError("could not set the light shader uniforms");

  // Draw model.
  glBindVertexArray(vertex_array_name);
  ThrowOnGLError("could not bind the VAO for drawing purposes");

  glDrawArrays(GL_TRIANGLES, 0, vertex_buffer.get_vertex_count());
  ThrowOnGLError("could not draw the model");

  glBindVertexArray(0);

  glUseProgram(0);
}

void DestroyCube()
{
  GGOGL(glDetachShader(shader_main, shader_vertex));
  GGOGL(glDetachShader(shader_main, shader_fragment));
  GGOGL(glDeleteShader(shader_vertex));
  GGOGL(glDeleteShader(shader_fragment));
  GGOGL(glDeleteProgram(shader_main));

  GGOGL(glDeleteBuffers(1, &buffer_name));
  GGOGL(glDeleteVertexArrays(1, &vertex_array_name));
}

void InitWindow(int argc, char* argv[])
{
  glutInit(&argc, argv);

  glutInitContextVersion(4, 0);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  glutInitWindowSize(CurrentWidth, CurrentHeight);

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

  if(WindowHandle < 1) {
    fprintf(
      stderr,
      "ERROR: Could not create a new rendering window.\n"
      );
    exit(EXIT_FAILURE);
  }

  //glutReshapeFunc(ResizeFunction);
  //glutDisplayFunc(RenderFunction);
  //glutIdleFunc(IdleFunction);
  //glutTimerFunc(0, TimerFunction, 0);
  //glutCloseFunc(DestroyCube);
}

int main(int argc, char* argv[])
{
  try
  {
    const int WIDTH = 600;
    const int HEIGHT = 600;

    InitWindow(argc, argv);

    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
      std::cerr << "ERROR: " << glewGetErrorString(GlewInitResult) << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;


    glGetError(); // flush error

    GGOGL(glEnable(GL_DEPTH_TEST));
    GGOGL(glDepthFunc(GL_LESS));
    GGOGL(glDisable(GL_CULL_FACE));
    /*GGOGL(glEnable(GL_CULL_FACE));
    GGOGL(glCullFace(GL_BACK));
    GGOGL(glFrontFace(GL_CCW));
*/
    CreateShaders();
    CreateIsosurface();

    camera.set_projection(60 * 3.15159f / 180, (float)WIDTH / HEIGHT, 1.0f, 100.0f);
    camera.set_position(0, 0, 10);
    camera.rotate({ 1.f, 0.f, 0.f }, -0.5f);
    

    //Somewhere at initialization
    GLuint frame_buffer_object, render_buffer;
    GGOGL(glGenFramebuffers(1, &frame_buffer_object));
    GGOGL(glGenRenderbuffers(1, &render_buffer));
    GGOGL(glBindRenderbuffer(GL_RENDERBUFFER, render_buffer));
    GGOGL(glRenderbufferStorage(GL_RENDERBUFFER, GL_BGRA, WIDTH, HEIGHT));
    GGOGL(glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_object));
    GGOGL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, render_buffer));



    GGOGL(glUseProgram(shader_main));
    GGOGL(glViewport(0, 0, WIDTH, HEIGHT));
    GGOGL(glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, camera.get_projection_matrix().coefs()));
    

    GGOGL(glClearColor(0.0f, 1.0f, 0.0f, 0.5f));
    GGOGL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    DrawScene();

    // After drawing
    std::vector<std::uint8_t> dataBGRA(WIDTH * HEIGHT * 4);
    GGOGL(glReadBuffer(GL_COLOR_ATTACHMENT0));
    GGOGL(glReadPixels(0, 0, WIDTH, HEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, &dataBGRA[0]));

    DestroyCube();

    std::vector<uint8_t> dataRGB(3 * WIDTH * HEIGHT);

    for (int y = 0; y < HEIGHT; ++y)
    {
      for (int x = 0; x < WIDTH; ++x)
      {
        dataRGB[3 * (y * WIDTH + x) + 0] = dataBGRA[4 * (y * WIDTH + x) + 2];
        dataRGB[3 * (y * WIDTH + x) + 1] = dataBGRA[4 * (y * WIDTH + x) + 1];
        dataRGB[3 * (y * WIDTH + x) + 2] = dataBGRA[4 * (y * WIDTH + x) + 0];
      }
    }

    ggo::save_bmp("d:/test.bmp", dataRGB.data(), WIDTH, HEIGHT);

    // Cleanup.
    glDeleteFramebuffers(1, &frame_buffer_object);
    glDeleteRenderbuffers(1, &render_buffer);
  }
  catch (std::exception & e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
