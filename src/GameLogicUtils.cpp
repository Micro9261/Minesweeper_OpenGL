#include "interface/IGameLogic.hpp"

DifficultyLevel& operator--(DifficultyLevel& oryg)
{
  switch (oryg)
  {
  case DifficultyLevel::easy:
    oryg = DifficultyLevel::medium;
    break;
  
  case DifficultyLevel::medium:
    oryg = DifficultyLevel::hard;
    break;

  case DifficultyLevel::hard:
    oryg = DifficultyLevel::custom;
    break;

  case DifficultyLevel::custom:
    oryg = DifficultyLevel::quit_opt;
    break;
  
  case DifficultyLevel::quit_opt:
    oryg = DifficultyLevel::easy;
    break;
  
  default:
    oryg = DifficultyLevel::waiting;
    break;
  }

  return oryg;
}

DifficultyLevel& operator++(DifficultyLevel& oryg)
{
  switch (oryg)
  {
  case DifficultyLevel::easy:
    oryg = DifficultyLevel::quit_opt;
    break;
  
  case DifficultyLevel::medium:
    oryg = DifficultyLevel::easy;
    break;

  case DifficultyLevel::hard:
    oryg = DifficultyLevel::medium;
    break;

  case DifficultyLevel::custom:
    oryg = DifficultyLevel::hard;
    break;
  
  case DifficultyLevel::quit_opt:
    oryg = DifficultyLevel::custom;
    break;
  
  default:
    oryg = DifficultyLevel::waiting;
    break;
  }

  return oryg;
}