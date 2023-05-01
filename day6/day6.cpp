#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <map>

std::string find_longest_unique_substr(std::string str) {
  short CHAR_RANGE = 128;
  short len = str.length();
  std::vector<int> window(CHAR_RANGE, 0);

  short begin = 0;
  short end = 0;
  short limit = 13;

  for (short low = 0, high = 0; high < len; high++) {
    if (window[str[high] - 'a']) {
      while (str[low] != str[high]) {
        window[str[low++] - 'a'] = false;
      }

      low++;
      continue;
    }
    
    window[str[high] - 'a'] = true;
    if (end - begin < high - low) {
      begin = low;
      end = high;

      if (end - begin >= limit) {
        break;
      }
    }
  }

  return str.substr(begin, end - begin + 1);
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

  try
  {
    while (std::getline(data, line))
    {
      int len = line.length();
      if (len)
      {
        auto marker = find_longest_unique_substr(line);
        std::cout << marker << '\n';
        
        for (int i = 0; i < len - 14; i++) {
          if (line.substr(i, 14) == marker) {
            std::cout << i + 14 << '\n';
            break;
          }
        }
      }
    }
  }
  catch (...)
  {
    std::cout << "Unexpected error occurred." << '\n';
  }

  std::cout << '\n';

  data.close();
  return EXIT_SUCCESS;
}
