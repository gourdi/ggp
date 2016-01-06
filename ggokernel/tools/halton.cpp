#include <ggo_halton.h>
#include <fstream>

int main(int argc, const char ** argv)
{
  const int GENERATED_COUNT = 2000;
  const int DUMPED_COUNT = 1000;
  
  auto base1 = ggo::halton_1d(2, GENERATED_COUNT);
  auto base2 = ggo::halton_1d(3, GENERATED_COUNT);
  
  std::ofstream ofs_rect("halton_rect.txt");
  for (int i = 0; i < DUMPED_COUNT; ++i)
  {
    ofs_rect << "    {" << base1[i].first << ".f/" << base1[i].second << ".f - 0.5f,  " << base2[i].first << ".f/" << base2[i].second << ".f - 0.5f}," << std::endl;
  }
  
  int count = 0;
  std::ofstream ofs_disc("halton_disc.txt");
  for (int i = 0; i < GENERATED_COUNT; ++i)
  {
    float x = -0.5f + static_cast<float>(base1[i].first) / base1[i].second;
    float y = -0.5f + static_cast<float>(base2[i].first) / base2[i].second;
    
    float d = std::sqrt(x * x + y * y);
    if (d <= 0.5f)
    {
      ofs_disc << "    {" << base1[i].first << ".f/" << base1[i].second << ".f - 0.5f,  " << base2[i].first << ".f/" << base2[i].second << ".f - 0.5f}," << std::endl;
      ++count;
    }
    
    if (count >= DUMPED_COUNT)
    {
      break;
    }
  }
}

