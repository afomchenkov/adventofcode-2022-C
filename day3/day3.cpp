#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

std::string calculate_missing_item(std::string str)
{
  int len = str.length();
  int half = len / 2;

  std::string first_part = str.substr(0, half);
  std::string second_part = str.substr(half);

  for (auto letter : first_part)
  {
    if (second_part.find(letter) != std::string::npos)
    {
      return std::string{letter};
    }
  }

  return "";
}

// Lowercase item types a through z have priorities 1 through 26.
// Uppercase item types A through Z have priorities 27 through 52.
//
// the literal 'a' has the value of 97 in the ASCII table
// the literal 'A' has the value of 65 in the ASCII table
int calculate_missing_item_score(std::string item)
{
  char ch = item[0];

  if (std::islower(ch))
  {
    return ((item[0]) - 'a') + 1; // calculate lower case
  }

  if (std::isupper(ch))
  {
    return ((item[0]) - 'A') + 27; // calculate upper case
  }

  return -1;
}

int calculate_group_score(const std::vector<std::string>& group)
{
  std::unordered_map<char, std::vector<int>> check;
  int len = group.size();

  for (int i = 0; i < len; ++i) {
    std::string str = group[i];

    for (char ch : str) {
      if (check.count(ch)) {
        check[ch][i] = 1;
      } else {
        std::vector<int> bit_map(len, 0);
        bit_map[i] = 1;
        check[ch] = bit_map;
      }
    }
  }

  for (auto bit_map : check) {
    if (bit_map.second[0] && bit_map.second[1] && bit_map.second[2]) {
      return calculate_missing_item_score(std::string{ bit_map.first });
    }
  }

  return 0;
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

  std::vector<std::string> group;
  int total_score = 0;
  try
  {
    while (std::getline(data, line))
    {
      int len = group.size();

      if (len == 3) {
        total_score += calculate_group_score(group);
        group = { line };
        continue;
      }
      
      group.push_back(line);
      // auto missing_item = calculate_missing_item(line);
      // int score = calculate_missing_item_score(missing_item);
      // total_score += score;
    }

    // calculate last group
    total_score += calculate_group_score(group);
  }
  catch (...)
  {
    std::cout << "Unexpected error occurred." << '\n';
  }

  std::cout << total_score << '\n';

  data.close();
  return EXIT_SUCCESS;
}
