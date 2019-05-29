#include "ggo_bitmap_artist_abc.h"
#include <kernel/ggo_kernel.h>
#include <kernel/time/ggo_chronometer.h>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <cstring>

namespace
{
  struct ggo_params
  {
    ggo::bitmap_artist_id	_artist_id;
    std::string				    _output_directory;
    int						        _from;
    int						        _to;
    int						        _width;
    int						        _height;
    int                   _threads_count;
  };

  std::mutex  mutex;
  int         image_index(0);
}

//////////////////////////////////////////////////////////////
class ggo_logger
{
public:

  ggo_logger(int thread_id, const std::string & filename = "")
  {
    mutex.lock();
    
    time_t currtime;                                              
    char buffer[200];                                             

    time(&currtime);                                                     
    strftime(buffer, sizeof(buffer), "%c", localtime(&currtime)); 

    std::cout << buffer;
    std::cout << ", thread #" << thread_id;
    if (filename.empty() == false)
    {
	    std::cout << ", " << filename;
    }
    std::cout << ": ";
  };
  
  ~ggo_logger()
  { 
    std::cout << std::endl;
    mutex.unlock();
  };
  
  template <typename T>
  ggo_logger & operator<<(const T & v)
  {
    std::cout << v;
    return *this;
  };
};

//////////////////////////////////////////////////////////////
bool parse_args(int argc, char ** argv, ggo_params & params)
{
	params._from = 1;
	params._to = 1;
	params._width = 4000;
	params._height = 4000;
  params._threads_count = 1;
	
	if (argc == 1)
	{
		std::cout << "ggocomputartbitmap ARTIST_ID ";
    std::cout << "[-d OUTPUT_DIRECTORY] ";
    std::cout << "[-s OUTPUT_WIDTH OUTPUT_HEIGHT] ";
    std::cout << "[-r RANGE_MIN RANGE_MAX] ";
    std::cout << "[-t THREADS_COUNT]" << std::endl;
		return false;
	}

	for (int i = 1; i < argc; ++i)
	{
		std::string arg(argv[i]);
		
		if (i == 1)
		{
			params._artist_id = static_cast<ggo::bitmap_artist_id>(ggo::to<int>(arg));
		}
		else
		if (arg == "-d")
		{
			++i;
			if (i >= argc)
			{
				std::cerr << "Error : missing directory parameter" << std::endl;
				return false;
			}
			
			params._output_directory = argv[i];
		}
		else
		if (arg == "-r")
		{
			++i;
			if (i >= argc)
			{
				std::cerr << "Error : missing range parameters" << std::endl;
				return false;
			}
      params._from = ggo::to<int>(argv[i]);
			
			++i;
			if (i >= argc)
			{
				std::cerr << "Error : missing range parameters" << std::endl;
				return false;
			}
      params._to = ggo::to<int>(argv[i]);
		}
		else
		if (arg == "-s")
		{
			++i;
			if (i >= argc)
			{
				std::cerr << "Error : missing width parameter" << std::endl;
				return false;
			}
      params._width = ggo::to<int>(argv[i]);

			++i;
			if (i >= argc)
			{
				std::cerr << "Error : missing height parameter" << std::endl;
				return false;
			}
      params._height = ggo::to<int>(argv[i]);
		}
    else
		if (arg == "-t")
		{
			++i;
			if (i >= argc)
			{
				std::cerr << "Error : missing threads count parameter" << std::endl;
				return false;
			}
      params._threads_count = ggo::to<int>(argv[i]);
		}
		else
		{
			std::cerr << "Invalid command line parameter: " << arg << std::endl;
			return false;
		}
	}
	
	return true;
}

//////////////////////////////////////////////////////////////
void render_images(const ggo_params & params, int thread_id)
{
  std::unique_ptr<ggo::bitmap_artist_abc> artist(ggo::bitmap_artist_abc::create(params._artist_id, params._width, params._height, 3 * params._width, ggo::rgb_8u_yu));
  ggo::array_8u buffer(3 * params._width * params._height);
  
  while (true)
  {
    mutex.lock();
    int index = image_index;
    ++image_index;
    mutex.unlock();
    
    if (index > params._to)
    {
      ggo_logger(thread_id) << "Finished";
      break;
    }
    
    std::ostringstream filename;
		if (params._output_directory.length() > 0)
		{
			filename << params._output_directory << '/';
		}
		filename.width(8);
		filename.fill('0');
		filename << index << ".bmp";
    ggo_logger(thread_id, filename.str()) << "Starting rendering image";

		ggo::chronometer chronometer;
    buffer.fill(0);
		artist->render_bitmap(buffer.data());
    ggo_logger(thread_id, filename.str()) << "Image rendered in " << chronometer.get_display_time();

		if (ggo::save_bmp(filename.str(), buffer.data(), ggo::rgb_8u_yu, params._width, params._height, 3 * params._width) == false)
    {
      ggo_logger(thread_id, filename.str()) << "Failed saving file";
    }
  }
}

//////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
	ggo_params params;
	if (parse_args(argc, argv, params) == false)
	{
		return 1;
	}

  std::cout << "Artist ID: " << static_cast<int>(params._artist_id) << std::endl;
  std::cout << "Output resolution: " << params._width << 'x' << params._height << std::endl;
  std::cout << "Thread(s): " << params._threads_count << std::endl;
    
  image_index = params._from;

  if (params._threads_count > 1)
  {
    std::vector<std::thread> threads;
    for (int i = 0; i < params._threads_count; ++i)
    {
      threads.push_back(std::thread(render_images, params, i));
    }
    for (auto & thread : threads)
    {
      thread.join();
    }
  }
  else
  {
    render_images(params, 0);
  }
}

