#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

struct Elf
{
  std::vector<int> calories;
  int total_amount = 0;

  Elf() {}
  ~Elf() {}

  int get_total_amount()
  {
    if (!total_amount)
    {
      for (auto next : calories)
      {
        total_amount += next;
      }
    }

    return total_amount;
  }
};

int main()
{
  std::string line;
  std::ifstream data("../data-2.txt");
  std::vector<Elf> elves;

  std::cout << "Start counting." << '\n';

  if (!data.is_open())
  {
    std::cout << "File cannot be opened." << '\n';
    return 0;
  }

  try
  {
    Elf elf;

    while (std::getline(data, line))
    {
      if (line == "")
      {
        auto total = elf.get_total_amount();
        elves.push_back(elf);

        Elf next_elf;
        elf = next_elf;
        continue;
      }

      elf.calories.push_back(stoi(line));
    }
    auto total = elf.get_total_amount();
    elves.push_back(elf);
  }
  catch (...)
  {
    std::cout << "Unexpected error occurred." << '\n';
  }

  std::sort(elves.begin(), elves.end(), [](const auto a, const auto b)
            { return a.total_amount > b.total_amount; });

  int total_three = 0;
  for (int i = 0; i < 3; ++i)
  {
    auto elf = elves[i];
    total_three += elf.total_amount;
  }

  std::cout << total_three << '\n';

  data.close();
  return EXIT_SUCCESS;
}
