#include "impl/GameRendererStdout.hpp"
#include <iostream>
#include <string>
#include <windows.h>
#include <memory>

GameRendererStdout::GameRendererStdout()
  : _board_size(Size(0,0))
{
  //Nothing
}

void GameRendererStdout::init()
{
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
  _board_size = board->get_board_size();
  render_title();
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
          if (pos == board->get_current_pos())
          {
            change_color(7,0);
          }
          else
          
          std::cout << "B";
          change_color(7,0);
        }
        else 
        {
          std::cout << static_cast<int>(GameBoardUtil::tileType_to_bomb_num(tile.type));
        }
      }
      else
      {
        if (pos == board->get_current_pos())
        {
          change_color(15,6);
        }
        else
        {
          change_color(4,4);
        }
        std::cout << "F";
        change_color(7,0);
      }

      if (pos == board->get_current_pos())
      {
        change_color(7,0);
      }
    }
    std::cout << "|\n";
  }
  
  std::cout << std::string(_board_size.x + 2, '#') << std::endl;
  std::cout << "Tiles discovered:" << board->get_discovered_tiles_num() << std::endl;
}

void GameRendererStdout::render_win()
{
  render_title();
  std::cout << "\tYou WIN!\n";
  std::cout << std::string(_board_size.x + 2, '#') << std::endl;
}

void GameRendererStdout::render_lose()
{
  render_title();
  std::cout << "\tYou LOSE!\n";
  std::cout << std::string(_board_size.x + 2, '#') << std::endl;
}

void GameRendererStdout::render_difficulty_selection(const DifficultyLevel& /* diff_level */)
{
  render_title();
  std::cout << "Choose difficulty level:\n";
  std::cout << "1) easy\n";
  std::cout << "2) medium\n";
  std::cout << "3) hard\n";
  std::cout << "4) custom\n\n" << std::endl;
}

void GameRendererStdout::render_ask_start(const StartState& /* state */)
{
  render_title();
  std::cout << "\nType S to start, Q to quit\n" << std::endl;
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

void GameRendererStdout::render_title()
{
  std::cout << "*** Minesweeper STDOUT ***\n";
}