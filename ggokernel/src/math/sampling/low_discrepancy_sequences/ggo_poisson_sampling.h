#ifndef __GGO_POISSON_SAMPLING__
#define __GGO_POISSON_SAMPLING__

#include <functional>

namespace ggo
{
  template <typename SAMPLE_TYPE, typename DIST_TYPE>
  std::vector<SAMPLE_TYPE> poisson_sampling(std::function<SAMPLE_TYPE()> generate_sample,
                                            std::function<DIST_TYPE(const SAMPLE_TYPE &, const SAMPLE_TYPE &)> compute_samples_dist,
                                            DIST_TYPE radius,
                                            int tries_count)
  {
    std::vector<SAMPLE_TYPE> samples;
    
    while (true)
    {
      // Try to create a new sample.
      int i = 0;
      for (; i < tries_count; ++i)
      {
        SAMPLE_TYPE sample = generate_sample();
        
        // Check if the new sample is not too close from previous samples.
        bool accept_sample = true;
        for (const auto & sample2 : samples)
        {
          if (compute_samples_dist(sample, sample2) < radius)
          {
            accept_sample = false;
            break;
          }
        }
        
        // The sample is valid.
        if (accept_sample == true)
        {
          samples.push_back(sample);
          break;
        }
      }
      
      if (i == tries_count)
      {
        break;
      }
    }
    
    return samples;
  }
}

#endif
