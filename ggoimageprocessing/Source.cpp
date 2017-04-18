#include <dng_>

void foo(const char * filename)
{
  try
  {

    dng_file_stream stream(filename);

    dng_host host;

    host.SetPreferredSize(gPreferredSize);
    host.SetMinimumSize(gMinimumSize);
    host.SetMaximumSize(gMaximumSize);

    host.ValidateSizes();

    if (host.MinimumSize())
    {

      host.SetForPreview(true);

      gDumpDNG.Clear();

    }

    if (gDumpDNG.NotEmpty())
    {

      host.SetSaveDNGVersion(dngVersion_SaveDefault);

      host.SetSaveLinearDNG(false);

      host.SetKeepOriginalFile(false);

    }

    // Read into the negative.

    AutoPtr<dng_negative> negative;

    {

      dng_info info;

      info.Parse(host, stream);

      info.PostParse(host);

      if (!info.IsValidDNG())
      {
        return dng_error_bad_format;
      }

      negative.Reset(host.Make_dng_negative());

      negative->Parse(host, stream, info);
    }