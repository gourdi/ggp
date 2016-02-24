#include "ggo_color_stream.h"
#include <iostream>
#include <Windows.h>

#ifdef _WIN32

namespace ggo
{
  color_stream::color_stream(console_color c)
  {
    ::CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
    ::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &console_screen_buffer_info);
    _old_color = console_screen_buffer_info.wAttributes;

    int txt_color = 0;
    switch (c)
    {
    case console_color::GREEN:
      txt_color = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
    case console_color::YELLOW:
      txt_color = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY; break;
    case console_color::RED:
      txt_color = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
    }
    ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), txt_color);
  }

  color_stream::~color_stream()
  {
    ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), _old_color);
  }
}

#else

namespace ggo
{
  color_stream::color_stream(console_color c)
  {
    switch (c)
    {
    case console_color::GREEN:
      std::cout << "\033[1; 32m"; break;
    case console_color::YELLOW:
      std::cout << "\033[1;33m"; break;
    case console_color::RED:
      std::cout << "\033[31m"; break;
    }
  }

  color_stream::~color_stream()
  {
    std::cout << "\033[0m";
  }
}

#endif