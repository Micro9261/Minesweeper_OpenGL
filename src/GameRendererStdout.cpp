#include "GameRendererStdout.hpp"
#include <iostream>
#include <string>
#include <windows.h>
#include <memory>

GameRendererStdout::GameRendererStdout()
  : _board_size(Size(0,0))
{
  //Nothing
}

void GameRendererStdout::init(const Size& board_size)
{
  _board_size = board_size;
  change_color(7,0); //default
}

void GameRendererStdout::set_board(const std::weak_ptr<IGameBoard> && board_ptr)
{
  _board_ptr = std::move(board_ptr);
}

void GameRendererStdout::render()
{
  if (_board_ptr.expired())
    return;
  
  std::shared_ptr<IGameBoard> board = _board_ptr.lock();
  std::cout << "*** Minesweeper STDOUT ***\n";
  std::cout << std::string(_board_size.x + 2, '#') << std::endl;
  
  for (uint8_t x = 0; x < _board_size.x; x++)
  {
    std::cout << "|";
    for (uint8_t y = 0; y < _board_size.y; y++)
    {
      const Pos pos(y,x);
      const Tile tile = board->get_tile(pos);

      if (pos == board->get_current_pos())
      {
        change_color(15,1);
      }

      if (TileStatus::hidden == tile.status)
      {
        std::cout << '-';
      }
      else if (TileStatus::discovered == tile.status)
      {
        if (TileType::blank == tile.type)
        {
          std::cout << " ";
        }
        else if (TileType::bomb == tile.type)
        {
          std::cout << "B";
        }
        else 
        {
          std::cout << static_cast<int>(GameBoardUtil::tileType_to_bomb_num(tile.type));
        }
      }
      else
      {
        std::cout << "U";
      }

      if (pos == board->get_current_pos())
      {
        change_color(7,0);
      }
    }
    std::cout << "|\n";
  }
  
  std::cout << std::string(_board_size.x + 2, '#') << std::endl;
}

void GameRendererStdout::render_win()
{
  std::cout << "*** Minesweeper STDOUT ***\n";
  std::cout << "\tYou WIN!\n";
  std::cout << std::string(_board_size.x + 2, '#') << std::endl;
}

void GameRendererStdout::render_lose()
{
  std::cout << "*** Minesweeper STDOUT ***\n";
  std::cout << "\tYou LOSE!\n";
  std::cout << std::string(_board_size.x + 2, '#') << std::endl;
}

void GameRendererStdout::clear()
{
  std::string clear_str(_board_size.y + 10, '\n');
  std::cout << clear_str << std::endl;
}

//Private

void GameRendererStdout::change_color(int text_col, int bg_col)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, (bg_col << 4) | text_col);
}