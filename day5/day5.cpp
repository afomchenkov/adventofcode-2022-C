#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

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

std::vector<int> parse_move(std::string line)
{
  auto tokens = split_string(line, " ");
  return {stoi(tokens[1]), stoi(tokens[3]), stoi(tokens[5])};
}

std::vector<std::stack<char>> compose_stacks(std::vector<std::string> data)
{
  data.pop_back();
  int len = data.size();
  int num_of_stacks = (data[0].length() / 4) + 1;
  std::vector<std::stack<char>> stacks(num_of_stacks);

  for (int i = len - 1; i >= 0; i--)
  {
    auto line = data[i];
    std::replace(line.begin(), line.end(), ' ', '.');

    int token_num = 0;
    for (int j = 0; j < line.length(); j += 4)
    {
      auto token = line.substr(j, 3);
      char val = token[1];

      if (val != '.')
      {
        stacks[token_num].push(val);
      }

      token_num++;
    }
  }

  return stacks;
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

  bool is_moving_started = false;
  std::queue<std::vector<int>> moves;
  std::vector<std::string> stacks_data;
  std::vector<std::stack<char>> stacks;

  try
  {
    while (std::getline(data, line))
    {
      if (!line.length())
      {
        is_moving_started = true;
        continue;
      }

      if (!is_moving_started)
      {
        stacks_data.push_back(line);
      }

      if (is_moving_started)
      {
        if (stacks_data.size())
        {
          stacks = compose_stacks(stacks_data);
          stacks_data = {};
        }

        moves.push(parse_move(line));
      }
    }
  }
  catch (...)
  {
    std::cout << "Unexpected error occurred." << '\n';
  }

  while (!moves.empty())
  {
    auto next_move = moves.front();
    moves.pop();

    int quantity = next_move[0];
    int stack_from = next_move[1];
    int stack_to = next_move[2];

    // for (int k = 0; k < quantity; ++k)
    // {
    //   auto item = stacks[stack_from - 1].top();
    //   stacks[stack_from - 1].pop();
    //   stacks[stack_to - 1].push(item);
    // }

    std::vector<char> to_move;
    for (int k = 0; k < quantity; ++k)
    {
      auto item = stacks[stack_from - 1].top();
      stacks[stack_from - 1].pop();
      to_move.push_back(item);
    }

    for (int i = to_move.size() - 1; i >= 0; i--)
    {
      stacks[stack_to - 1].push(to_move[i]);
    }
  }

  for (auto st : stacks)
  {
    std::cout << st.top();
  }

  std::cout << '\n';

  data.close();
  return EXIT_SUCCESS;
}
