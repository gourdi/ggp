#include <ggo_best_candidate_sequence.h>
#include <fstream>

int main(int argc, const char ** argv)
{
  const int GENERATED_COUNT = 1000;
  
  auto points = ggo::best_candidate_sequence_2d(GENERATED_COUNT);
  
  std::ofstream ofs("best_candidate.txt");
  for (const auto & point : points)
  {
    ofs << "    {" << point.get<0>() << "f, " << point.get<1>() << "f}," << std::endl;
  }
}

