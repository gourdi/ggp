#include "ggo_blur.h"
#include <kernel/math/signal_proc/ggo_gaussian_blur.h>
#include <kernel/math/signal_proc/ggo_convolution2d.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>
#include <vivelle/ggo_threadpool.h>

namespace ggo
{
  template <typename color_t>
  struct gaussian_blur_helper { };

  template <>
  struct gaussian_blur_helper<ggo::rgb_8u>
  {
    using color_t = rgb_16u;

    static std::vector<uint16_t> build_kernel(const float stddev)
    {
      return ggo::build_fixed_point_gaussian_kernel<uint16_t, float>(stddev, 0.001f, 8);
    }

    static ggo::rgb_16u convert(const ggo::rgb_8u & c)
    {
      return ggo::rgb_16u(uint16_t(c.r()), uint16_t(c.g()), uint16_t(c.b()));
    }

    static ggo::rgb_8u convert(const ggo::rgb_16u & c)
    {
      return ggo::rgb_8u(
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c.r())),
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c.g())),
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c.b())));
    }
  };

  struct horz_blur
  {
    template <ggo::image_format format>
    static void call(const void * input, void * output, ggo::size size, int input_line_step, int output_line_step, int y_start, int y_end, float stddev)
    {
      using format_traits = ggo::image_format_traits<format>;

      if constexpr(std::is_same<format_traits::color_t, ggo::rgb_8u>::value == true)
      {
        using helper = gaussian_blur_helper<typename format_traits::color_t>;
        using color_aux_t = typename helper::color_t;

        auto kernel = helper::build_kernel(stddev);
        auto clipping = ggo::rect_int::from_left_right_bottom_top(0, size.width() - 1, y_start, y_end - 1);
        auto read = [](const void * ptr) -> color_aux_t { return helper::convert(format_traits::read(ptr)); };
        auto write = [](void * ptr, const color_aux_t & pixel) { format_traits::write(ptr, helper::convert(pixel)); };

        apply_symetric_kernel_2d_horz<ggo::border_mode::duplicate_edge>(
          input, format_traits::pixel_byte_size, input_line_step, read,
          output, format_traits::pixel_byte_size, output_line_step, write,
          size, clipping,
          kernel.data(), kernel.size());
      }
      else
      {
        throw std::runtime_error("unsupported format for blur");
      }
    }
  };

  struct vert_blur
  {
    template <ggo::image_format format>
    static void call(const void * input, void * output, ggo::size size, int input_line_step, int output_line_step, int x_start, int x_end, float stddev)
    {
      using format_traits = ggo::image_format_traits<format>;

      if constexpr(std::is_same<format_traits::color_t, ggo::color_8u>::value == true)
      {
        using helper = gaussian_blur_helper<typename format_traits::color_t>;
        using color_aux_t = typename helper::color_t;

        auto kernel = helper::build_kernel(stddev);
        auto clipping = ggo::rect_int::from_left_right_bottom_top(x_start, x_end - 1, 0, size.height() - 1);
        auto read = [](const void * ptr) -> color_aux_t { return helper::convert(format_traits::read(ptr)); };
        auto write = [](void * ptr, const color_aux_t & pixel) { format_traits::write(ptr, helper::convert(pixel)); };

        apply_symetric_kernel_2d_vert<ggo::border_mode::duplicate_edge>(
          input, format_traits::pixel_byte_size, input_line_step, read,
          output, format_traits::pixel_byte_size, output_line_step, write,
          size, clipping,
          kernel.data(), kernel.size());
      }
      else
      {
        throw std::runtime_error("unsupported format for blur");
      }
    }
  };

  void horz_blur_job(const ggo::image * input, ggo::image * output, int y_start, int y_end, float stddev)
  {
    dispatch_image_format<horz_blur>(input->format(),
      input->data(), output->data(), input->size(), input->line_byte_step(), output->line_byte_step(),
      y_start, y_end, stddev);
  }

  void vert_blur_job(const ggo::image * input, ggo::image * output, int x_start, int x_end, float stddev)
  {
    dispatch_image_format<vert_blur>(input->format(),
      input->data(), output->data(), input->size(), input->line_byte_step(), output->line_byte_step(),
      x_start, x_end, stddev);
  }

  void blur(ggo::image & image, const parameters & params)
  {
    auto stddev = params.get<float>({ "stddev" });
    if (!stddev)
    {
      throw std::runtime_error("missing stddev parameter");
    }

    ggo::image temp(image.size(), image.format());

    int threads_count = static_cast<int>(global_threadpool().threads_count());

    // Horizontal blur.
    std::vector<std::future<void>> horz_jobs;
    for (int i = 0; i < threads_count; ++i)
    {
      int y_start = i * image.height() / threads_count;
      int y_end = (i + 1) * image.height() / threads_count;

      horz_jobs.emplace_back(global_threadpool().enqueue(horz_blur_job, &image, &temp, y_start, y_end, *stddev));
    }

    for (auto & job : horz_jobs)
    {
      job.wait();
    }

    // Vertical blur.
    std::vector<std::future<void>> vert_jobs;
    for (int i = 0; i < threads_count; ++i)
    {
      int x_start = i * image.width() / threads_count;
      int x_end = (i + 1) * image.width() / threads_count;

      vert_jobs.emplace_back(global_threadpool().enqueue(vert_blur_job, &temp, &image, x_start, x_end, *stddev));
    }

    for (auto & job : vert_jobs)
    {
      job.wait();
    }
  }
}