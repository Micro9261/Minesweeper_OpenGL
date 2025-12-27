#include "impl/InputSFML.hpp"

InputSFML::InputSFML(sf::RenderWindow& window)
  : _window(window), _window_reshaped(false)
{
  //Nothing
}

void InputSFML::init()
{
  _view = _window.getDefaultView();
}
void InputSFML::set_board(const std::weak_ptr<IGameBoard> && board_ptr)
{
  _board_ptr = std::move(board_ptr);
}

[[nodiscard]] Move InputSFML::get_input()
{
  Move move;
  if (_board_ptr.expired())
    return Move();
  
  std::shared_ptr<IGameBoard> board = _board_ptr.lock();
  move.pos = board->get_current_pos();
  const Pos current_pos = move.pos;

  sf::Event event;
  while (_window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      _window.close();
    if (event.type == sf::Event::Resized)
      _reshape_window(event);

    if (event.type == sf::Event::KeyPressed)
    {
      uint8_t x = current_pos.x;
      uint8_t y = current_pos.y;
      switch (event.key.code)
      {
        case sf::Keyboard::A :
          x--;
          move.pos = Pos(x,y);
          break;
        
        case sf::Keyboard::S :
          y++;
          move.pos = Pos(x,y);
          break;
        
        case sf::Keyboard::D :
          x++;
          move.pos = Pos(x,y);
          break;

        case sf::Keyboard::W :
          y--;
          move.pos = Pos(x,y);
          break;

        case sf::Keyboard::Q :
          move.action = Action::Check;
          break;
        
        case sf::Keyboard::F :
          move.action = Action::flag;
          break;
        
        case sf::Keyboard::R :
          move.action = Action::reset;
          break;
        
        case sf::Keyboard::C :
          move.action = Action::change_difficulty;
          break;

        case sf::Keyboard::Escape:
          _window.close();
        
        default:
          break;
      }
    }
  }
  
  return move;
}

[[nodiscard]] DifficultyLevel InputSFML::get_difficulty()
{
  DifficultyLevel diff_value = DifficultyLevel::waiting;
  sf::Event event;
  while (_window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      _window.close();
    if (event.type == sf::Event::Resized)
      _reshape_window(event);
    
    if (event.type == sf::Event::KeyPressed)
    {
      if (event.key.code == sf::Keyboard::Escape)
        _window.close();
      else if (event.key.code == sf::Keyboard::Num1)
        diff_value = DifficultyLevel::easy;
      else if (event.key.code == sf::Keyboard::Num2)
        diff_value = DifficultyLevel::medium;
      else if (event.key.code == sf::Keyboard::Num3)
        diff_value = DifficultyLevel::hard;
      else if (event.key.code == sf::Keyboard::Num4)
        diff_value = DifficultyLevel::custom;
    }
  }
  return diff_value;
}

[[nodiscard]] std::tuple<DifficultyLevel, ViewParams, bool> InputSFML::get_difficulty_3D(
  const DifficultyLevel& diff_level)
{
  DifficultyLevel diff_value = diff_level;
  ViewParams view;
  bool enter = false;
  sf::Event event;
  while (_window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      _window.close();
    if (event.type == sf::Event::Resized)
      _reshape_window(event);
    
    if (event.type == sf::Event::KeyPressed)
    {
      switch (event.key.code)
      {
        case sf::Keyboard::W:
          ++diff_value;
          break;

        case sf::Keyboard::S:
          --diff_value;
          break;

        case sf::Keyboard::Escape:
          _window.close();
          break;

        case sf::Keyboard::Enter:
          enter = true;
          break;

        default:
          break;
      }
    }
  }

  switch (diff_value)
  {
  case DifficultyLevel::easy:
    view = ViewParams({3.f, 90.f, 90.f});
    break;

  case DifficultyLevel::medium:
    view = ViewParams({5.f, 90.f, 90.f});
    break;

  case DifficultyLevel::hard:
    diff_value = DifficultyLevel::hard;
    view = ViewParams({7.f, 90.f, 90.f});
    break;

  case DifficultyLevel::custom:
    view = ViewParams({7.f, 90.f, 90.f});
    break;
  
  default:
    break;
  }
  return {diff_value, view, enter};
}

[[nodiscard]] bool InputSFML::start()
{
  bool start = true;
  sf::Event event;
  while (_window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      _window.close();
    if (event.type == sf::Event::Resized)
      _reshape_window(event);

    if (event.type == sf::Event::KeyPressed)
    {
      if (event.key.code == sf::Keyboard::Escape)
        _window.close();
      else if (event.key.code == sf::Keyboard::S)
        start = true;
      else if (event.key.code == sf::Keyboard::Q)
        start = false;
    }
  }
  return start;
}

[[nodiscard]] std::tuple<StartState, bool> InputSFML::start_3D(const StartState& state)
{
  StartState state_value = state;
  bool enter = false;
  sf::Event event;
  while (_window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      _window.close();
    if (event.type == sf::Event::Resized)
      _reshape_window(event);

    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
      {
        case sf::Keyboard::W:
          ++state_value;
          break;

        case sf::Keyboard::S:
          --state_value;
          break;

        case sf::Keyboard::Escape:
          _window.close();
          break;

        case sf::Keyboard::Enter:
          enter = true;
          break;

        default:
          break;
      }
    }
  }
  return {state_value, enter};
}

[[nodiscard]] std::tuple<Move, ViewParams> InputSFML::get_input_3D(const ViewParams& params)
{
  if (_board_ptr.expired())
    return {Move(), ViewParams()};
  
  ViewParams new_params(params);
  Move move;
  
  std::shared_ptr<IGameBoard> board = _board_ptr.lock();
  move.pos = board->get_current_pos();

  const Pos current_pos = move.pos;
  const float angle_step = 1.f;
  const float trans_step = 0.1f;

  sf::Event event;
  while (_window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      _window.close();
    if (event.type == sf::Event::Resized)
      _reshape_window(event);

    if (event.type == sf::Event::KeyPressed)
    {
      uint8_t x = current_pos.x;
      uint8_t y = current_pos.y;
      switch (event.key.code)
      {
        case sf::Keyboard::Escape :
          _window.close();
          break;

        case sf::Keyboard::Up:
          new_params.set_phi(params.get_phi() - angle_step);
          break;

        case sf::Keyboard::Down:
          new_params.set_phi(params.get_phi() + angle_step);
          break;

        case sf::Keyboard::Left:
          new_params.set_theta(params.get_theta() + angle_step);
          break;

        case sf::Keyboard::Right:
          new_params.set_theta(params.get_theta() - angle_step);
          break;

        case sf::Keyboard::Numpad8:
          new_params.set_distance(params.get_distance() + trans_step);
          break;

        case sf::Keyboard::Numpad2:
          new_params.set_distance(params.get_distance() - trans_step);
          break;

        case sf::Keyboard::A :
          x--;
          move.pos = Pos(x,y);
          break;
        
        case sf::Keyboard::S :
          y++;
          move.pos = Pos(x,y);
          break;
        
        case sf::Keyboard::D :
          x++;
          move.pos = Pos(x,y);
          break;

        case sf::Keyboard::W :
          y--;
          move.pos = Pos(x,y);
          break;

        case sf::Keyboard::Q :
          move.action = Action::Check;
          break;
        
        case sf::Keyboard::F :
          move.action = Action::flag;
          break;
        
        case sf::Keyboard::R :
          move.action = Action::reset;
          break;
        
        case sf::Keyboard::C :
          move.action = Action::change_difficulty;
          break;

        default:
          break;
      }
    }
  }
  return {move, new_params};
}

[[nodiscard]] bool InputSFML::reshape()
{
  bool value = _window_reshaped;
  _window_reshaped = false;
  return value;
}

// Private

void InputSFML::_reshape_window(const sf::Event& event)
{
  const float new_width = static_cast<float>(event.size.width);
  const float new_height = static_cast<float>(event.size.height);
  _view.setSize({new_width, new_height});
  _view.setCenter({new_width / 2.f, new_height / 2.f});
  _window.setView(_view);
  _window_reshaped = true;
}