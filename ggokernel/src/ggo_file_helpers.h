#ifndef __GGO_FILE_HELPERS__
#define __GGO_FILE_HELPERS__

#include <string>
#include <algorithm>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  inline std::string get_file_extension(const std::string & filename)
  {
    auto pos = filename.find_last_of(".");

    if (pos == std::string::npos)
    {
      return "";
    }

    std::string extension = filename.substr(pos + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    return extension;
  }
}

#endif
