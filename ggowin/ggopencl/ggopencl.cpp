#pragma warning(disable: 4996)

#include <array>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_disc_light.h>
#include "ggo_opencl_array.h"
#include "ggo_opencl_image.h"
#include "ggo_opencl_camera.h"
#include <CL/cl.h>

//////////////////////////////////////////////////////////////
std::vector<cl_platform_id> ggo_get_platform_ids()
{
  std::array<cl_platform_id, 64> platforms;
  cl_uint num_platforms = 0;

  cl_int error = clGetPlatformIDs(platforms.size(), platforms.data(), &num_platforms);
  if (error != CL_SUCCESS)
  {
    throw std::exception("failed getting platform ids");
  }

  return std::vector<cl_platform_id>(platforms.data(), platforms.data() + num_platforms);
}

//////////////////////////////////////////////////////////////
std::string ggo_get_platform_info(cl_platform_id platform_id, int flag)
{
  std::array<char, 1024> info;
  size_t size = 0;

  cl_int error = clGetPlatformInfo(platform_id, flag, info.size(), info.data(), &size);
  if (error != CL_SUCCESS)
  {
    std::ostringstream oss;
    oss << "failed getting info for platform " << platform_id << " (error: " << error << ")";
    throw std::exception(oss.str().c_str());
  }

  return std::string(info.data(), size);
}

//////////////////////////////////////////////////////////////
std::string ggo_get_platform_name(cl_platform_id platform_id)
{
  return ggo_get_platform_info(platform_id, CL_PLATFORM_NAME);
}

//////////////////////////////////////////////////////////////
std::string ggo_get_device_info_str(cl_device_id device_id, int flag)
{
  std::array<char, 1024> info;
  size_t size = 0;

  cl_int error = clGetDeviceInfo(device_id, flag, info.size(), info.data(), &size);
  if (error != CL_SUCCESS)
  {
    std::ostringstream oss;
    oss << "failed getting info for device " << device_id << " (error: " << error << ")";
    throw std::exception(oss.str().c_str());
  }

  return std::string(info.data(), size);
}

//////////////////////////////////////////////////////////////
uint32_t ggo_get_device_info_uint32(cl_device_id device_id, int flag)
{
  std::array<unsigned char, 1024> info;
  size_t size = 0;

  cl_int error = clGetDeviceInfo(device_id, flag, info.size(), info.data(), &size);
  if (error != CL_SUCCESS)
  {
    std::ostringstream oss;
    oss << "failed getting info for device " << device_id << " (error: " << error << ")";
    throw std::exception(oss.str().c_str());
  }

  return info[0] | (info[1] << 8) | (info[2] << 16) | (info[3] << 24);
}

//////////////////////////////////////////////////////////////
std::string ggo_get_device_image_max_size(cl_device_id device_id)
{
  std::ostringstream oss;

  oss << ggo_get_device_info_uint32(device_id, CL_DEVICE_IMAGE2D_MAX_WIDTH) << "x" << ggo_get_device_info_uint32(device_id, CL_DEVICE_IMAGE2D_MAX_HEIGHT);

  return oss.str();
}

//////////////////////////////////////////////////////////////
std::vector<cl_device_id> ggo_get_device_ids(cl_platform_id platform_id, cl_device_type device_type = CL_DEVICE_TYPE_ALL)
{
  std::array<cl_device_id, 1024> devices;
  cl_uint num_devices = 0;

  cl_int error = clGetDeviceIDs(platform_id, device_type, devices.size(), devices.data(), &num_devices);
  if (error != CL_SUCCESS)
  {
    std::ostringstream oss;
    oss << "failed getting device ids for platform " << platform_id << " (error: " << error << ")";
    throw std::exception(oss.str().c_str());
  }

  return std::vector<cl_device_id>(devices.data(), devices.data() + num_devices);
}

//////////////////////////////////////////////////////////////
std::string ggo_get_device_name(cl_device_id device_id)
{
  std::array<char, 1024> name;
  size_t size = 0;

  cl_int error = clGetDeviceInfo(device_id, CL_DEVICE_NAME, name.size(), name.data(), &size);
  if (error != CL_SUCCESS)
  {
    std::ostringstream oss;
    oss << "failed getting name of device " << device_id;
    throw std::exception(oss.str().c_str());
  }

  return std::string(name.data(), size);
}

//////////////////////////////////////////////////////////////
cl_context ggo_create_context(cl_device_id device_id)
{
  cl_int error = 0;
  cl_context context = clCreateContext(0, 1, &device_id, nullptr, nullptr, &error);
  if (error != CL_SUCCESS)
  {
    std::ostringstream oss;
    oss << "failed creating context for device " << device_id;
    throw std::exception(oss.str().c_str());
  }

  return context;
}

//////////////////////////////////////////////////////////////
cl_context ggo_create_context(cl_device_type device_type, cl_device_id & device_id)
{
  auto platform_ids = ggo_get_platform_ids();

  for (const auto & platform_id : platform_ids)
  {
    auto device_ids = ggo_get_device_ids(platform_id, device_type);

    if (device_ids.empty() == false)
    {
      device_id = device_ids.front();
      cl_context context = ggo_create_context(device_id);
      std::cout << "Creating context " << context << " for device " << ggo_get_device_name(device_id) << " of platform " << ggo_get_platform_name(platform_id) << std::endl;
      std::cout << "  max image size: " << ggo_get_device_image_max_size(device_id) << std::endl;
      return ggo_create_context(device_ids.front());
    }
  }

  std::ostringstream oss;
  oss << "failed creating context for device type " << device_type;
  throw std::exception(oss.str().c_str());
}

//////////////////////////////////////////////////////////////
cl_command_queue ggo_create_command_queue(cl_context context, cl_device_id & device_id)
{
  cl_int error = 0;
  cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &error);
  if (error != CL_SUCCESS)
  {
    std::ostringstream oss;
    oss << "failed creating command queue for device " << device_id << " and context " << context;
    throw std::exception(oss.str().c_str());
  }

  return queue;
}

//////////////////////////////////////////////////////////////
cl_program ggo_build_program(cl_context context, cl_device_id device_id, std::vector<std::string> & filenames)
{
  std::cout << "Building program:" << std::endl;

  std::vector<std::string> sources;

  for (const auto & filename : filenames)
  {
    std::ifstream ifs(filename);
    std::istreambuf_iterator<char> input_it(ifs);
    std::istreambuf_iterator<char> eos;

    sources.emplace_back(input_it, eos);
  }

  std::vector<const char *> sources_cstr;
  std::vector<size_t> sources_sizes;
  for (const auto & source : sources)
  {
    sources_cstr.push_back(source.c_str());
    sources_sizes.push_back(source.size());
  }

  cl_int build_error = 0;
  cl_program program = clCreateProgramWithSource(context, sources.size(), &sources_cstr[0], &sources_sizes[0], &build_error);
  if (build_error == CL_SUCCESS)
  {
    build_error = clBuildProgram(program, 1, &device_id, "", nullptr, nullptr);
  }

  // Display log.
  size_t log_size = 0;
  cl_int error = 0;
  error = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
  if (error != CL_SUCCESS)
  {
    throw std::exception("failed retrieving log size");
  }
  ggo_array<unsigned char> log(log_size);
  error = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
  if (error != CL_SUCCESS)
  {
    throw std::exception("failed retrieving log");
  }

  std::cout << std::string(log.get_pointer(), log + log_size) << std::endl;

  if (build_error != CL_BUILD_SUCCESS)
  {
    std::ostringstream oss;
    oss << "failed building program (error " << build_error << ")";
    throw std::exception(oss.str().c_str());
  }

  return program;
}

//////////////////////////////////////////////////////////////
template <typename T>
void ggo_set_argument(cl_kernel kernel, int arg, const T & value)
{
  cl_int error = clSetKernelArg(kernel, arg, sizeof(T), (void*)&value);
  if (error != CL_SUCCESS)
  {
    std::ostringstream oss;
    oss << "failed setting argument " << arg << " (error: " << error << ")"; 
    throw std::exception(oss.str().c_str());
  }
}

//////////////////////////////////////////////////////////////
std::vector<float> setup_scene()
{
  std::vector<float> scene_data;

  // First white plane.
  scene_data.push_back(0); // a
  scene_data.push_back(0); // b
  scene_data.push_back(1); // c
  scene_data.push_back(1); // d
  scene_data.push_back(1); // r
  scene_data.push_back(1); // g
  scene_data.push_back(1); // b
  scene_data.push_back(0); // reflexion

  for (int i = 0; i < 5; ++i)
  {
    scene_data.push_back(-1.5f); // x
    scene_data.push_back(float(2 * i)); // y
    scene_data.push_back(0); // z
    scene_data.push_back(1); // radius
    scene_data.push_back(1); // r
    scene_data.push_back(0); // g
    scene_data.push_back(0); // b
    scene_data.push_back(0.5); // reflexion

    scene_data.push_back(1.5f); // x
    scene_data.push_back(float(2 * i)); // y
    scene_data.push_back(0); // z
    scene_data.push_back(1); // radius
    scene_data.push_back(0); // r
    scene_data.push_back(1); // g
    scene_data.push_back(0); // b
    scene_data.push_back(0.5); // reflexion
  }

  return scene_data;
}

//////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  try
  {
    cl_device_id device_id = 0;
    auto context = ggo_create_context(CL_DEVICE_TYPE_GPU, device_id);
    auto queue = ggo_create_command_queue(context, device_id);

    const int WIDTH = 640;
    const int HEIGHT = 480;
    const int SAMPLES_COUNT = 256;

    // Create and build program.
    std::vector<std::string> filenames;
    filenames.emplace_back("c:/Cygwin/home/ggourdin/ggp/ggowin/ggopencl/kernel.cl");
    auto program = ggo_build_program(context, device_id, filenames);

    // Kernel.
    cl_int error = 0;
    cl_kernel ggo_kernel = clCreateKernel(program, "ggo_kernel", &error);
    if (error != CL_SUCCESS)
    {
      throw std::exception("failed creating kernel"); 
    }

    // Camera.
    ggo_point_camera camera(WIDTH, HEIGHT);
    camera.set_samples_count(SAMPLES_COUNT);
    camera.set_depth_of_field(15);
    camera.set_depth_of_field_factor(0.5f);
    camera.basis().move(0, 0, 15);
    camera.basis().rotate(ggo_ray3d_float::O_X(), 1.2f);

    camera.on_start_rendering_frame();

    auto center_focus_points = camera.get_center_focus_points();
    auto eye_points = camera.get_eye_points();
    std::shuffle(center_focus_points.begin(), center_focus_points.end(), ggo_get_random_generator());
    std::shuffle(eye_points.begin(), eye_points.end(), ggo_get_random_generator());

    // Lights.
    std::vector<std::shared_ptr<ggo_disc_light>> lights;

    ggo_point3d_float pos1(100, 0, 100);
    ggo_ray3d_float ray1(pos1, -pos1);
    lights.push_back(std::make_shared<ggo_disc_light>(ggo_color(0.75, 0.75, 0.75), ray1, 10.f));
    lights.back()->set_samples_count(SAMPLES_COUNT);

    ggo_point3d_float pos2(-100, 0, 100);
    ggo_ray3d_float ray2(pos2, -pos2);
    lights.push_back(std::make_shared<ggo_disc_light>(ggo_color(0.75, 0.75, 0.75), ray2, 10.f));
    lights.back()->set_samples_count(SAMPLES_COUNT);

    std::vector<std::vector<ggo_point3d_float>> lights_samples;
    for (auto & light : lights)
    {
      auto light_samples = light->sample();
      std::shuffle(light_samples.begin(), light_samples.end(), ggo_get_random_generator());
      lights_samples.emplace_back(light_samples);
    }

    // Arguments.
    std::cout << "Passing arguments" << std::endl;

    ggo_array_float accumulation_buffer(3 * WIDTH * HEIGHT, 0);

    // Scene.
    auto scene_params = setup_scene();

    ggo_opencl_readonly_array_float scene_buffer(context, scene_params);
    ggo_set_argument(ggo_kernel, 1, scene_buffer.get_handle());

    cl_uint scene_data_count = scene_params.size();
    ggo_set_argument(ggo_kernel, 2, scene_data_count);

    // Generate images.
    for (int sample = 0; sample < SAMPLES_COUNT; ++sample)
    {
      // Image argument.
      ggo_opencl_writeonly_image_float opencl_image(context, WIDTH, HEIGHT);
      ggo_set_argument(ggo_kernel, 0, opencl_image.get_handler());

      // Camera.

      // Position.
      std::vector<float> camera_params;
      camera_params.push_back(eye_points[sample].x());
      camera_params.push_back(eye_points[sample].y());
      camera_params.push_back(eye_points[sample].z());

      // Basis x-axis.
      camera_params.push_back(camera.basis().x().x());
      camera_params.push_back(camera.basis().x().y());
      camera_params.push_back(camera.basis().x().z());

      // Basis y-axis.
      camera_params.push_back(camera.basis().y().x());
      camera_params.push_back(camera.basis().y().y());
      camera_params.push_back(camera.basis().y().z());

      // Basis z-axis.
      camera_params.push_back(center_focus_points[sample].x());
      camera_params.push_back(center_focus_points[sample].y());
      camera_params.push_back(center_focus_points[sample].z());

      // Opti.
      camera_params.push_back(camera.get_depth_of_field() * camera.get_aperture() / (0.5f * camera.get_height()));

      ggo_opencl_readonly_array_float camera_buffer(context, camera_params);
      ggo_set_argument(ggo_kernel, 3, camera_buffer.get_handle());

      // Lights.
      std::vector<float> lights_params;
      for (int i = 0; i < int(lights.size()); ++i)
      {
        lights_params.push_back(lights_samples[i][sample].x());
        lights_params.push_back(lights_samples[i][sample].y());
        lights_params.push_back(lights_samples[i][sample].z());
        lights_params.push_back(lights[i]->get_color().r());
        lights_params.push_back(lights[i]->get_color().g());
        lights_params.push_back(lights[i]->get_color().b());
      }

      ggo_opencl_readonly_array_float lights_buffer(context, lights_params);
      ggo_set_argument(ggo_kernel, 4, lights_buffer.get_handle());

      cl_uint lights_count = lights.size();
      ggo_set_argument(ggo_kernel, 5, lights_count);

      // Queue.
      std::cout << "Starting OpenCL (sample " << sample << ")" << std::endl;
      const size_t global_ws[2] = { WIDTH, HEIGHT };
      error = clEnqueueNDRangeKernel(queue, ggo_kernel, 2, nullptr, global_ws, nullptr, 0, nullptr, nullptr);
      if (error != CL_SUCCESS)
      {
        std::ostringstream oss;
        oss << "failed enqueuing kernel (error: " << error << ")"; 
        throw std::exception(oss.str().c_str()); 
      }

      // Reading back and accumulate.
      ggo_array_float res(4 * WIDTH * HEIGHT, 0);
      opencl_image.enqueue(queue, res.get_pointer());

      for (int i = 0; i < WIDTH * HEIGHT; ++i)
      {
        accumulation_buffer[3 * i + 0] += res[4 * i + 0];
        accumulation_buffer[3 * i + 1] += res[4 * i + 1];
        accumulation_buffer[3 * i + 2] += res[4 * i + 2];
      }
    }

    // Save to disk.
    ggo_array_uint8 bmp(3 * WIDTH * HEIGHT);
    for (int y = 0; y < HEIGHT; ++y)
    {
      uint8_t * ptr = bmp.get_pointer() + 3 * (HEIGHT - y - 1) * WIDTH;

      for (int x = 0; x < WIDTH; ++x)
      {
        float r = 255.f * accumulation_buffer[3 * (y * WIDTH + x) + 0] / SAMPLES_COUNT;
        float g = 255.f * accumulation_buffer[3 * (y * WIDTH + x) + 1] / SAMPLES_COUNT;
        float b = 255.f * accumulation_buffer[3 * (y * WIDTH + x) + 2] / SAMPLES_COUNT;

        ptr[3 * x + 0] = ggo_clamp(ggo_round(r), 0, 255);
        ptr[3 * x + 1] = ggo_clamp(ggo_round(g), 0, 255);
        ptr[3 * x + 2] = ggo_clamp(ggo_round(b), 0, 255);
      }
    }

    ggo_save_bmp("d:/res.bmp", bmp, WIDTH, HEIGHT);

    clReleaseKernel(ggo_kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
  }
  catch (std::exception & e)
  {
    std::cerr << e.what() << std::endl;
  }
}
