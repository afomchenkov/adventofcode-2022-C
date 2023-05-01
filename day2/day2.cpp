#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

enum class Move
{
  Rock = 1,
  Paper = 2,
  Scissors = 3,
  None = -1
};

enum class Result
{
  Loss = 0,
  Draw = 3,
  Win = 6,
  None = -1
};

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

// A for Rock, B for Paper, C for Scissors
// X for Rock, Y for Paper, Z for Scissors
// 1 for Rock, 2 for Paper, 3 for Scissors
Move parse_move(std::string move)
{
  if (move == "A" || move == "X")
  {
    return Move::Rock;
  }

  if (move == "B" || move == "Y")
  {
    return Move::Paper;
  }

  if (move == "C" || move == "Z")
  {
    return Move::Scissors;
  }

  return Move::None;
}

// 0 if you lost, 3 if the round was a draw, 6 if you won
Result calculate_your_result(Move opponent_move, Move your_move)
{
  if (opponent_move == your_move)
  {
    return Result::Draw;
  }

  // Rock defeats Scissors, Scissors defeats Paper, Paper defeats Rock
  bool your_win = (your_move == Move::Rock && opponent_move == Move::Scissors) ||
                  (your_move == Move::Scissors && opponent_move == Move::Paper) ||
                  (your_move == Move::Paper && opponent_move == Move::Rock);

  return your_win ? Result::Win : Result::Loss;
}

// [opponent_score, your_score]
std::vector<int> calculate_score(std::string opponent, std::string you)
{
  auto opponent_move = parse_move(opponent);
  auto your_move = parse_move(you);

  // draw
  if (calculate_your_result(opponent_move, your_move) == Result::Draw)
  {
    return {(int)Result::Draw + (int)opponent_move, (int)Result::Draw + (int)your_move};
  }

  // you loose
  if (calculate_your_result(opponent_move, your_move) == Result::Loss)
  {
    return {(int)Result::Win + (int)opponent_move, (int)Result::Loss + (int)your_move};
  }

  // you win
  if (calculate_your_result(opponent_move, your_move) == Result::Win)
  {
    return {(int)Result::Loss + (int)opponent_move, (int)Result::Win + (int)your_move};
  }

  return {0, 0};
}

Move calculate_loss(Move opponent_move)
{
  switch (opponent_move)
  {
  case Move::Paper:
  {
    return Move::Rock;
    break;
  }
  case Move::Rock:
  {
    return Move::Scissors;
    break;
  }
  case Move::Scissors:
  {
    return Move::Paper;
    break;
  }
  default:
    return Move::None;
  }
}

Move calculate_win(Move opponent_move)
{
  switch (opponent_move)
  {
  case Move::Paper:
  {
    return Move::Scissors;
    break;
  }
  case Move::Rock:
  {
    return Move::Paper;
    break;
  }
  case Move::Scissors:
  {
    return Move::Rock;
    break;
  }
  default:
    return Move::None;
  }
}

int calculate_expectation_score(std::string opponent, std::string you)
{
  auto opponent_move = parse_move(opponent);

  // Y - you need to draw
  if (you == "Y")
  {
    return (int)Result::Draw + (int)opponent_move;
  }

  // X - you need to lose
  if (you == "X")
  {
    return (int)Result::Loss + (int)calculate_loss(opponent_move);
  }

  // Z - you need to win
  if (you == "Z")
  {
    return (int)Result::Win + (int)calculate_win(opponent_move);
  }

  return (int)Result::None;
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

  int total_opponent = 0;
  int total_you = 0;

  try
  {
    while (std::getline(data, line))
    {
      std::vector<std::string> tokens = split_string(line, " ");
      std::string first = tokens[0];  // - opponent move
      std::string second = tokens[1]; // - your move

      // auto result = calculate_score(first, second);
      // total_opponent += result[0];
      // total_you += result[1];

      total_you += calculate_expectation_score(first, second);
    }
  }
  catch (...)
  {
    std::cout << "Unexpected error occurred." << '\n';
  }

  std::cout << total_opponent << ":" << total_you << '\n';

  data.close();
  return EXIT_SUCCESS;
}
