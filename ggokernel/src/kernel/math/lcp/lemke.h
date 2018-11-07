#include <kernel/memory/ggo_array.h>
#include <iostream>
#include <algorithm>

namespace ggo
{
  namespace lemke
  {
    template <typename scalar_t>
    struct equation
    {
      static_assert(std::is_floating_point<scalar_t>::value);

      int _key_index;
      ggo::array<scalar_t, 1> _coefs; // Assuming the following order: q, w1, w2, ..., wN, z0, z1, z2, ..., zN

      scalar_t q() const { return _coefs[0]; }
    };

    template <typename scalar_t>
    bool is_terminal(const std::vector<equation<scalar_t>> & dictionary)
    {
      for (const auto & equation : dictionary)
      {
        // z0 is the key?
        if (equation._key_index == dictionary.size() + 1)
        {
          return false;
        }

        // constant is negative?
        if (equation.q() < 0)
        {
          return false;
        }
      }

      return true;
    }

    std::string key2str(int key_index, int z_offset)
    {
      if (key_index < z_offset)
      {
        return "w" + std::to_string(key_index);
      }
      else
      {
        return "z" + std::to_string(key_index - z_offset);
      }
    }

    template <typename scalar_t>
    void dump(const equation<scalar_t> & equation)
    {
      const int z_offset = equation._coefs.size() / 2;

      std::cout << key2str(equation._key_index, z_offset) << " = " << equation.q();

      for (int i = 1; i < z_offset; ++i)
      {
        if (equation._coefs[i] > 0)
        {
          std::cout << " + " << equation._coefs[i] << ".w" << i;
        }
        else if (equation._coefs[i] < 0)
        {
          std::cout << " - " << -equation._coefs[i] << ".w" << i;
        }
      }

      for (int i = z_offset; i < equation._coefs.size(); ++i)
      {
        if (equation._coefs[i] > 0)
        {
          std::cout << " + " << equation._coefs[i] << ".z" << i - z_offset;
        }
        else if (equation._coefs[i] < 0)
        {
          std::cout << " - " << -equation._coefs[i] << ".z" << i - z_offset;
        }
      }
    }

    template <typename scalar_t>
    void dump(const std::vector<equation<scalar_t>> & dictionary)
    {
      for (const auto & eq : dictionary)
      {
        dump(eq);
        std::cout << std::endl;
      }
    }

    template <typename scalar_t>
    std::vector<equation<scalar_t>> build_dictionary(const ggo::array<scalar_t, 2> & M, const ggo::array<scalar_t, 2> & q)
    {
      std::vector<equation<scalar_t>> dictionary;

      for (int y = 0; y < M.height(); ++y)
      {
        dictionary.emplace_back();

        dictionary.back()._key_index = y + 1; // Keys are 'w' variables.

        dictionary.back()._coefs.resize(2 * M.width() + 2);

        dictionary.back()._coefs[0] = q(0, y); // Constants 'q'
        dictionary.back()._coefs[M.width() + 1] = 1; // Auxiliary variables 'z0'

        for (int x = 0; x < M.width(); ++x)
        {
          dictionary.back()._coefs(x + 1) = 0.f; // 'w' variables are null when statring the algo.
          dictionary.back()._coefs(M.width() + x + 2) = M(x, y);
        }
      }

      return dictionary;
    }

    template <typename scalar_t>
    int insert_z(std::vector<equation<scalar_t>> & dictionary, int in_index)
    {
      const int z_offset = static_cast<int>(dictionary.size() + 1);
      GGO_ASSERT_EQ(z_offset, dictionary[0]._coefs.size() / 2);

      GGO_ASSERT(in_index >= z_offset); // A 'z' variable is expted to enter the dictionary.

      // Seek for the equation to substitute. It is the one with the lowest ratio: -q / z[index].
      scalar_t ratio_min = std::numeric_limits<scalar_t>::max();
      auto selected_equation = dictionary.end();
      if (in_index == z_offset) // z0
      {
        selected_equation = std::min_element(dictionary.begin(), dictionary.end(), [](const auto & eq1, const auto & eq2) { return eq1.q() < eq2.q(); });
        if (selected_equation->q() >= 0)
        {
          throw std::runtime_error("could not move z0 to dictionary");
        }
      }
      else
      {
        for (auto equation = dictionary.begin(); equation != dictionary.end(); ++equation)
        {
          if (equation->_coefs[in_index] != 0)
          {
            scalar_t ratio_cur = -equation->q() / equation->_coefs[in_index];
            if (ratio_cur < ratio_min)
            {
              ratio_min = ratio_cur;
              selected_equation = equation;
            }
          }
        }
      }

      if (selected_equation == dictionary.end())
      {
        throw std::runtime_error("failed finding equation to substitute");
      }

      std::cout << "selected the following equation to make z" << in_index - z_offset << " enter the dictionary:" << std::endl;
      dump(*selected_equation);
      std::cout << std::endl << std::endl;

      // Get the index of the key (which should be a w variable), for its compementary z variable to enter the dictionary later.
      int out_index = selected_equation->_key_index;

      // Update the selected equation.
      selected_equation->_key_index = in_index;

      scalar_t scale = -1.f / selected_equation->_coefs[in_index];

      selected_equation->_coefs[out_index] = -1;
      selected_equation->_coefs[in_index] = 0;
      for (auto & c : selected_equation->_coefs)
      {
        c *= scale;
      }

      std::cout << "made z" << in_index - z_offset << " enter the dictionary by modifying the selected equation (" << key2str(out_index, z_offset) << " leaves the dictionary):" << std::endl;
      dump(dictionary);
      std::cout << std::endl;

      // Update the other equations.
      for (auto & equation = dictionary.begin(); equation != dictionary.end(); ++equation)
      {
        if (equation != selected_equation)
        {
          scalar_t scale = equation->_coefs[in_index];
          for (int i = 0; i < equation->_coefs.size(); ++i)
          {
            equation->_coefs[i] += scale * selected_equation->_coefs[i];
          }
          equation->_coefs[in_index] = 0;
        }
      }
      std::cout << "substituing z" << in_index - z_offset << " in the other equations:" << std::endl;
      dump(dictionary);
      std::cout << std::endl;

      return out_index;
    }

    // Solve an LCP. Given a matrix M, and a vector q, construct vectors w and z such that:
    // - w = q + Mz
    // - wz = 0
    // - w >= 0
    // - z >= 0
    template <typename scalar_t>
    void solve(const ggo::array<scalar_t, 2> & M, const ggo::array<scalar_t, 2> & q, ggo::array<scalar_t, 2> & w, ggo::array<scalar_t, 2> & z)
    {
      // Setup the dictionary.
      auto dictionary = build_dictionary(M, q);
      std::cout << "initial dictionary:" << std::endl;
      dump(dictionary);
      std::cout << std::endl;

      const int z_offset = static_cast<int>(dictionary.size()) + 1;
      int z_index = z_offset; // We first want ot move z0 to the dictionary.
      while (is_terminal(dictionary) == false)
      {
        // We made a w variable leave the dictionary, we have to make its complementary z variable enter the dictionary.
        int w_index = insert_z(dictionary, z_index);
        z_index = w_index + z_offset; // Convert from the w index that went out of the dictionary, to a z index that must enter the dictionary.
      }

      // Store solution.
      w.resize(1, static_cast<int>(dictionary.size()));
      z.resize(1, static_cast<int>(dictionary.size()));

      w.fill(0.f);
      z.fill(0.f);
      for (int i = 0; i < dictionary.size(); ++i)
      {
        if (dictionary[i]._key_index < z_offset)
        {
          w(0, dictionary[i]._key_index - 1) = dictionary[i].q();
        }
        else
        {
          z(0, dictionary[i]._key_index - z_offset - 1) = dictionary[i].q();
        }
      }

      auto dump = [](const ggo::array<scalar_t, 2> & v)
      {
        std::cout << v(0, 0);
        for (int i = 1; i < v.height(); ++i)
        {
          std::cout << ", " << v(0, i);
        }
      };

      std::cout << "solutions are: " << std::endl << "  w = (";
      dump(w);
      std::cout << ")" << std::endl << "  z = (";
      dump(z);
      std::cout << ")" << std::endl;
    }
  }
}
