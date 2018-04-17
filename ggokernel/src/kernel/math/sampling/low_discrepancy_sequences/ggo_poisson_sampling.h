#ifndef __GGO_POISSON_SAMPLING__
#define __GGO_POISSON_SAMPLING__

#include <functional>

namespace ggo
{
  template <typename sample_type, typename dist_type>
  std::vector<sample_type> poisson_sampling(std::function<sample_type()> generate_sample,
                                            std::function<dist_type(const sample_type &, const sample_type &)> compute_samples_dist,
                                            dist_type radius,
                                            int tries_count)
  {
    std::vector<sample_type> samples;
    
    while (true)
    {
      // Try to create a new sample.
      int i = 0;
      for (; i < tries_count; ++i)
      {
        sample_type sample = generate_sample();
        
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
