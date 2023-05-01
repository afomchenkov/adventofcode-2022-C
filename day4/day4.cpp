#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>

std::vector<std::string> split_string(std::string str, std::string pattern)
{
  std::vector<std::string> result;

  while (str.find(pattern) != std::string::npos)
  {
    auto first_occur = str.find(pattern);
    result.push_back(str.substr(0, first_occur));
    str = str.substr(first_occur + 1);
  }

  if ((int)str.length() > 0)
  {
    result.push_back(str);
  }

  return result;
}

bool is_ranges_overlap(std::vector<int> &first_range, std::vector<int> &second_range)
{
  auto a_from = first_range[0];
  auto a_to = first_range[1];

  auto b_from = second_range[0];
  auto b_to = second_range[1];

  // a contains b
  // |--- a ---|
  //  |- b -|
  if (a_from <= b_from && a_to >= b_to)
  {
    return true;
  }
  // b contains a
  // |--- b ---|
  //     |- a -|
  if (b_from <= a_from && b_to >= a_to)
  {
    return true;
  }

  // a and b have intersection
  // |--- b ---|         |--- b ---|
  //        |- a -|    |- a -|
  // OR
  //  |--- a ---|      |--- a ---|
  // |- b -|             |---- b ----|
  if (
      (a_from <= b_from && a_to >= b_from) ||
      (b_from <= a_from && b_to >= a_from))
  {
    return true;
  }

  return false;
}

int main()
{
  std::string line;
  std::ifstream data("../data-2.txt");

  std::cout << "Start counting." << '\n';

  if (!data.is_open())
  {
    std::cout << "File cannot be opened." << '\n';
    return 0;
  }

  int overlaps_count = 0;
  try
  {

    while (std::getline(data, line))
    {
      // assignment range: FirstElf[from, to], SecondElf[from, to]
      auto split = split_string(line, ",");

      auto a = split_string(split[0], "-");
      std::vector<int> first_range(a.size());
      std::transform(a.begin(), a.end(), first_range.begin(), [](const auto val)
                     { return stoi(val); });

      auto b = split_string(split[1], "-");
      std::vector<int> second_range(b.size());
      std::transform(b.begin(), b.end(), second_range.begin(), [](const auto val)
                     { return stoi(val); });

      if (is_ranges_overlap(first_range, second_range))
      {
        overlaps_count++;
      }
    }
  }
  catch (...)
  {
    std::cout << "Unexpected error occurred." << '\n';
  }

  std::cout << overlaps_count << '\n';

  data.close();
  return EXIT_SUCCESS;
}
