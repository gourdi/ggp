#include <new>
#include <CL/cl.h>

//////////////////////////////////////////////////////////////
class ggo_opencl_image
{
public:

  virtual ~ggo_opencl_image()
  {
    clReleaseMemObject(_mem_handle);
  }

  cl_mem get_handler() const { return _mem_handle; }

protected:

  ggo_opencl_image(cl_context context, cl_mem_flags flags, cl_channel_type channel_type, size_t width, size_t height, void * host_ptr)
    :
    _width(width), _height(height)
  {
    cl_image_format image_format;
    image_format.image_channel_order = CL_RGBA;
    image_format.image_channel_data_type = channel_type;

    cl_int error = 0;
    _mem_handle = clCreateImage2D(context, flags, &image_format, width, height, 0, host_ptr, &error);
    if (error != CL_SUCCESS)
    {
      std::ostringstream oss;
      oss << "failed creating opencl image (error: " << error << ")"; 
      throw std::exception(oss.str().c_str());
    }
  }

protected:
  cl_mem  _mem_handle;
  size_t  _width;
  size_t  _height;
};

//////////////////////////////////////////////////////////////
class ggo_opencl_writeonly_image_float : public ggo_opencl_image
{
public:
  ggo_opencl_writeonly_image_float(cl_context context, size_t width, size_t height)
    :
    ggo_opencl_image(context, CL_MEM_WRITE_ONLY, CL_FLOAT, width, height, nullptr)
  {
  }

  void enqueue(cl_command_queue queue, float * buffer)
  {
    const size_t origin[3] = {0, 0, 0};
    const size_t region[3] = {_width, _height, 1};

    cl_int error = clEnqueueReadImage(queue, _mem_handle, CL_TRUE, origin, region, 0, 0, buffer, 0, nullptr, nullptr);
    if (error != CL_SUCCESS)
    {
      std::ostringstream oss;
      oss << "failed enqueuing read image (error: " << error << ")"; 
      throw std::exception(oss.str().c_str()); 
    }
  }
};

