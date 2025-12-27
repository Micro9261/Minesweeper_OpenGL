#include "interface/IInput3D.hpp"

StartState& operator--(StartState& oryg)
{
  switch (oryg)
  {
  case StartState::start:
    oryg = StartState::selection_screen;
    break;
  
  case StartState::selection_screen:
    oryg = StartState::quit;
    break;

  case StartState::quit:
    oryg = StartState::start;
    break;

  default:
    oryg = StartState::waiting;
    break;
  }

  return oryg;
}

StartState& operator++(StartState& oryg)
{
  switch (oryg)
  {
  case StartState::start:
    oryg = StartState::quit;
    break;
  
  case StartState::selection_screen:
    oryg = StartState::start;
    break;

  case StartState::quit:
    oryg = StartState::selection_screen;
    break;
      
  default:
    oryg = StartState::waiting;
    break;
  }

  return oryg;
}