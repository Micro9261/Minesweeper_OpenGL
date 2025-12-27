#include "impl/InputStdin.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <limits>

InputStdin::InputStdin()
{
  //Nothing
}

void InputStdin::init()
{
  //Nothing
}

void InputStdin::set_board(const std::weak_ptr<IGameBoard> && board_ptr)
{
  _board_ptr = std::move(board_ptr);
}

[[nodiscard]] Move InputStdin::get_input()
{
  if (_board_ptr.expired())
    return Move();
  
  std::shared_ptr<IGameBoard> board = _board_ptr.lock();
  //Print input legend
  std::cout << "Controls:\n";
  std::cout << "Move: Left(A), Up(W), Down(S), Right(d)\n";
  std::cout << "Action: Select(Q), Flag(F), Reset(R), Change difficulty(C)" << std::endl;

  char input = get_input_safe();
  if (std::islower(input))
  {
    input = std::toupper(input);
  }

  const Pos current_pos = board->get_current_pos();
  Move move(Action::no_action, current_pos);
  uint8_t x = current_pos.x;
  uint8_t y = current_pos.y;
  switch (input)
  {
  case 'W':
    y--;
    move.pos = Pos(x,y);
    break;
  
  case 'S':
    y++;
    move.pos = Pos(x,y);
    break;

  case 'A':
    x--;
    move.pos = Pos(x,y);
    break;

  case 'D':
    x++;
    move.pos = Pos(x,y);
    break;

  case 'Q':
    move.action = Action::Check;
    break;

  case 'F':
    move.action = Action::flag;
    break;

  case 'C':
    move.action = Action::change_difficulty;
    break;

  case 'R':
    move.action = Action::reset;
    break;
  
  default:
    break;
  }

  return move;
}

[[nodiscard]] DifficultyLevel InputStdin::get_difficulty()
{
  DifficultyLevel diff_level = DifficultyLevel::custom;
  char input = '4';
  bool bad_input = false;
  do
  {
    bad_input = false;
    // std::cout << "Select difficulty level:\n";
    // std::cout << "1) Easy\n";
    // std::cout << "2) Medium\n";
    // std::cout << "3) hard\n";
    // std::cout << "4) Custom" << std::endl;

    char input = get_input_safe();
    if (std::islower(input))
    {
      input = std::toupper(input);
    }

    switch (input)
    {
    case '1':
      diff_level = DifficultyLevel::easy;
      break;
    
    case '2':
      diff_level = DifficultyLevel::medium;
      break;

    case '3':
      diff_level = DifficultyLevel::hard;
      break;

    case '4':
      diff_level = DifficultyLevel::custom;
      break;
    
    default:
      std::cout << "Bad option!" << std::endl;
      bad_input = true;
      break;
    }

  } while (bad_input);

  return diff_level;
}

[[nodiscard]] bool InputStdin::start()
{
  char input = 'Q';
  do
  {
    input = get_input_safe();
    if (std::islower(input))
    {
      input = std::toupper(input);
    }

    if (input != 'S' && input != 'Q')
    {
      std::cout << "Bad option!" << std::endl;
    }

  } while (input != 'S' && input != 'Q');

  return input == 'S';
}

//Private

char InputStdin::get_input_safe()
{
  char value = std::cin.get();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return value;
}