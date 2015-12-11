#include <new>
#include <vector>
#include <CL/cl.h>

//////////////////////////////////////////////////////////////
class ggo_opencl_array
{
public:

  virtual ~ggo_opencl_array()
  {
    clReleaseMemObject(_mem_handle);
  }

  cl_mem get_handle() const { return _mem_handle; }

protected:

  ggo_opencl_array(cl_context context, cl_mem_flags flags, size_t size, void * host_ptr)
  {
    cl_int error = 0;
    _mem_handle = clCreateBuffer(context, flags, size, host_ptr, &error);
    if (error != CL_SUCCESS)
    {
      std::ostringstream oss;
      oss << "failed creating opencl buffer (error: " << error << ")"; 
      throw std::exception(oss.str().c_str());
    }
  }

protected:

  cl_mem  _mem_handle;
};

//////////////////////////////////////////////////////////////
class ggo_opencl_readonly_array_float : public ggo_opencl_array
{
public:

  ggo_opencl_readonly_array_float(cl_context context, size_t size, const float * host_ptr)
    :
    ggo_opencl_array(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size * sizeof(float), (void*)host_ptr)
  {
  }

  ggo_opencl_readonly_array_float(cl_context context, const std::vector<float> & data)
    :
    ggo_opencl_array(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, data.size() * sizeof(float), (void*)&data[0])
  {
  }
};

