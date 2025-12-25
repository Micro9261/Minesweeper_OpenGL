#include <memory>
#include <thread>
#include <chrono>
#include "Game.hpp"

#include "interface/IGameRenderer.hpp"
#include "interface/IGameBoard.hpp"
#include "interface/IGameLogic.hpp"
#include "interface/IInput.hpp"

#include "impl/GameRendererStdout.hpp"
#include "impl/GameBoard.hpp"
#include "impl/GameLogic.hpp"
#include "impl/InputStdin.hpp"

Game::Game()
{
  //Nothing
}

void Game::Start()
{
  std::unique_ptr<IGameBoard> board_ptr(std::make_unique<GameBoard>());
  std::unique_ptr<IGameLogic> logic_ptr(std::make_unique<GameLogic>());
  logic_ptr->set_board(board_ptr.release(), false);
  std::unique_ptr<IGameRenderer> render_ptr(std::make_unique<GameRendererStdout>());
  render_ptr->init();
  render_ptr->set_board(logic_ptr->get_board());
  std::unique_ptr<IInput> input_ptr(std::make_unique<InputStdin>());
  input_ptr->init();
  input_ptr->set_board(logic_ptr->get_board());

  bool close_game = false;
  while ( !close_game )
  {
    render_ptr->clear();
    render_ptr->render_difficulty_selection();
    DifficultyLevel diff_level = input_ptr->get_difficulty();
    render_ptr->clear();
    logic_ptr->set_difficulty_level(diff_level);
    logic_ptr->clean();

    render_ptr->render_ask_start();
    bool start = input_ptr->start();
    if (!start)
    {
      close_game = true;
      continue;
    }
    logic_ptr->Start();

    while ( GameStatus::in_progress == logic_ptr->get_status() && !close_game)
    {
      render_ptr->clear();
      render_ptr->render();
      const Move move = input_ptr->get_input();
      
      switch (move.action)
      {
      case Action::no_action :
        logic_ptr->move_select(move.pos);
        break;

      case Action::Check :
        logic_ptr->check_tile(move.pos);
        break;

      case Action::flag :
        logic_ptr->flag_tile(move.pos);
        break;

      case Action::change_difficulty :
        logic_ptr->set_status_lose();
        break;

      case Action::reset :
        logic_ptr->clean();
        render_ptr->render_ask_start();
        start = input_ptr->start();
        if (start)
        {
          logic_ptr->Start();
        }
        else
        {
          close_game = true;
        }
        break;
      
      default:
        break;
      }

    }
    render_ptr->clear();
    render_ptr->render();

    GameStatus status = logic_ptr->get_status();
    if ( GameStatus::lose == status )
    {
      render_ptr->render_lose();
    }
    else if ( GameStatus::win == status )
    {
      render_ptr->render_win();
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

}

Game::~Game()
{
  std::cout << "Game end!" << std::endl;
}