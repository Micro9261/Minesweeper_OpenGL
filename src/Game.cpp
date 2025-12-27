#include <memory>
#include <thread>
#include <chrono>

// #include <SFML2/Window.hpp>
// #include <SFML2/Graphics.hpp>
// #include <SFML2/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <GL/glu.h>
#include "Game.hpp"

#include "interface/IGameRenderer.hpp"
#include "interface/IGameRenderer3D.hpp"
#include "interface/IGameBoard.hpp"
#include "interface/IGameLogic.hpp"
#include "interface/IInput.hpp"
#include "interface/IInput3D.hpp"

#include "impl/GameRendererStdout.hpp"
#include "impl/GameBoard.hpp"
#include "impl/GameLogic.hpp"
#include "impl/InputStdin.hpp"
#include "impl/GameRendererSFML.hpp"
#include "impl/InputSFML.hpp"

enum WindowState
{
  difficulty_selection,
  start_waiting,
  game_in_progress,
  result_display
};


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
    render_ptr->render_difficulty_selection(DifficultyLevel::waiting);
    DifficultyLevel diff_level = input_ptr->get_difficulty();
    render_ptr->clear();
    logic_ptr->set_difficulty_level(diff_level);
    logic_ptr->clean();

    render_ptr->render_ask_start(StartState::waiting);
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
        render_ptr->render_ask_start(StartState::waiting);
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

void Game::start_3D()
{
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.majorVersion = 2;
  settings.minorVersion = 1;

  sf::RenderWindow window(
    sf::VideoMode(800, 600),
    "Minesweeper 3D",
    sf::Style::Default,
    settings
  );
  window.setVerticalSyncEnabled(true);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glClearDepth(1.f);

  std::unique_ptr<IGameBoard> board_ptr(std::make_unique<GameBoard>());
  std::unique_ptr<IGameLogic> logic_ptr(std::make_unique<GameLogic>());
  logic_ptr->set_board(board_ptr.release(), false);
  std::unique_ptr<IGameRenderer3D> render_ptr(std::make_unique<GameRendererSFML>(
    window
  ));
  render_ptr->init();
  render_ptr->set_board(logic_ptr->get_board());
  std::unique_ptr<IInput3D> input_ptr(std::make_unique<InputSFML>(window));
  input_ptr->init();
  input_ptr->set_board(logic_ptr->get_board());


  float angle_x = 0.f;
  float angle_y = 0.f;

  WindowState win_state = WindowState::difficulty_selection;
  bool animation_finished = true;
  bool animation_started = false;
  ViewParams view_param;
  DifficultyLevel difficulty_level = DifficultyLevel::easy;
  StartState start_state = StartState::start;
  while (window.isOpen())
  {
    if (input_ptr->reshape())
    {
      render_ptr->init();
    }

    render_ptr->clear();
    render_ptr->render_view(view_param);
    switch (win_state)
    {
      case WindowState::difficulty_selection:
      {
        render_ptr->render_difficulty_selection(difficulty_level);
        auto [diff_level, view, enter] = input_ptr->get_difficulty_3D(difficulty_level);
        view_param = view;
        difficulty_level = diff_level;
        if (enter)
        {
          if (DifficultyLevel::quit_opt != difficulty_level)
          {
            logic_ptr->set_difficulty_level(difficulty_level);
            logic_ptr->clean();
            win_state = WindowState::start_waiting;
          }
          else
          {
            window.close();
          }
        }
        break;
      }

      case WindowState::start_waiting:
      {
        render_ptr->render_ask_start(start_state);
        auto [state, enter] = input_ptr->start_3D(start_state);
        start_state = state;
        if (enter)
        {
          switch (start_state)
          {
            case StartState::start:
              logic_ptr->Start();
              win_state = WindowState::game_in_progress;
              break;

            case StartState::selection_screen:
              win_state = WindowState::difficulty_selection;
              start_state = StartState::start;
              break;

            case StartState::quit:
              window.close();
              break;
            
            default:
              break;
          }
        }
        
        break;
      }

      case WindowState::game_in_progress:
      {
        render_ptr->render();
        auto [move, view] = input_ptr->get_input_3D(view_param);
        view_param = view;

        if (GameStatus::in_progress != logic_ptr->get_status())
        {
          win_state = WindowState::result_display;
          break;
        }

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
          win_state =  WindowState::difficulty_selection;
          break;

        case Action::reset :
          window.close();
          break;
        
        default:

          break;
        }
        break;
      }

      case WindowState::result_display:
      {
        if (animation_finished && !animation_started)
        {
          animation_finished = false;
          animation_started = true;

          //Animation time
          std::thread anim_time(
            [&animation_finished]()
            {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            animation_finished = true;
            }
          );
          anim_time.detach();
        }

        if (GameStatus::lose == logic_ptr->get_status())
        {
          render_ptr->render_lose();
        }
        else if (GameStatus::win == logic_ptr->get_status())
        {
          render_ptr->render_win();
        }

        if (animation_finished)
        {
          win_state = WindowState::difficulty_selection;
          animation_started = false;
        }
        break;
      }
        
      default:

        break;
    }

    window.display();
  }
}

Game::~Game()
{
  std::cout << "Game end!" << std::endl;
}