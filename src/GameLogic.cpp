#include "GameLogic.hpp"
#include <random>
#include <vector>

GameLogic::GameLogic()
  : _difficulty(DifficultyLevel::custom), _status(GameStatus::preparing),
  _armored(false)
{

}

GameLogic::GameLogic(IGameBoard * board_ptr)
  : GameLogic(DifficultyLevel::easy, board_ptr)
{
  //Nothing
}

GameLogic::GameLogic(const DifficultyLevel& diff, IGameBoard * board_ptr)
  : _difficulty(diff), _status(GameStatus::preparing), _armored(false), _board_ptr(board_ptr)
{
  uint8_t bombs_num = 0;
  Size size;

  switch (_difficulty)
  {
  case DifficultyLevel::easy :
    bombs_num = 10;
    size = Size(9, 9);
    break;
  
  case DifficultyLevel::medium :
    bombs_num = 40;
    size = Size(16, 16);
    break;
  
  case DifficultyLevel::hard :
    bombs_num = 99;
    size = Size(24, 24);
    break;
  
  default:
    //Params will be set in set_custom_params()
    break;
  }

  _board_ptr->set_board_size(size);
  _board_ptr->resize_and_clear();
  _board_ptr->set_bombs_num(bombs_num);
}

GameLogic::GameLogic(const Size& size, uint8_t bombs_num, IGameBoard * board_ptr)
  : _difficulty(DifficultyLevel::custom), _armored(false), _status(GameStatus::preparing),
  _board_ptr(board_ptr)
{
  _board_ptr->set_board_size(size);
  _board_ptr->resize_and_clear();
  _board_ptr->set_bombs_num(bombs_num);
}

GameLogic::GameLogic(uint8_t x, uint8_t y, uint8_t bombs_num, IGameBoard * board_ptr)
  : GameLogic(Size(x,y), bombs_num, board_ptr)
{
  //Nothing
}

void GameLogic::set_difficulty_level(const DifficultyLevel& diff)
{
  _difficulty = diff;
  _status = GameStatus::preparing;
}

void GameLogic::set_custom_params(const Size& size, uint8_t bombs_num)
{
  _difficulty = DifficultyLevel::custom;
  _status = GameStatus::preparing;
  _board_ptr->set_board_size(size);
  _board_ptr->resize_and_clear();
  _board_ptr->set_bombs_num(bombs_num);
}

void GameLogic::set_custom_params(uint8_t x, uint8_t y, uint8_t bombs_num)
{
  _difficulty = DifficultyLevel::custom;
  _status = GameStatus::preparing;
  _board_ptr->set_board_size(Size(x,y));
  _board_ptr->resize_and_clear();
  _board_ptr->set_bombs_num(bombs_num);
}

void GameLogic::set_board(IGameBoard * board_ptr, bool armor)
{
  _board_ptr.reset(board_ptr);
  _difficulty = DifficultyLevel::custom;
  _status = GameStatus::in_progress;
  _armored = armor;
}

void GameLogic::clean()
{
  const Size board_size = _board_ptr->get_board_size();
  const uint8_t bombs_num = _board_ptr->get_bombs_num();

  _status = GameStatus::preparing;
  _board_ptr->set_board_size(board_size);
  _board_ptr->resize_and_clear();
  _board_ptr->set_bombs_num(bombs_num);

  _armored = false;
}

void GameLogic::Start()
{
  _status = GameStatus::in_progress;
}

void GameLogic::armor(const Pos& start_pos)
{
  const Size board_size = _board_ptr->get_board_size();

  std::random_device rand_device;
  std::mt19937 gen(rand_device());
  std::uniform_int_distribution<> distrib_x(0, board_size.x - 1);
  std::uniform_int_distribution<> distrib_y(0, board_size.y - 1);

  //Fill bombs
  const uint8_t bombs_num = _board_ptr->get_bombs_num();
  for (uint8_t i = 0; i < bombs_num; i++)
  {

    bool retry = true;
    while ( retry )
    {
      uint8_t x = static_cast<uint8_t>(distrib_x(gen));
      uint8_t y = static_cast<uint8_t>(distrib_y(gen));

      const Pos pos(x,y);
      if (pos == start_pos)
      {
        continue;
      }
      Tile chosen_tile = _board_ptr->get_tile(pos);
      if (TileType::undefined == chosen_tile.type)
      {
        chosen_tile.type = TileType::bomb;
        _board_ptr->set_tile(pos, chosen_tile);
        retry = false;
      }
    }
    
  }

  //Fill rest of the map
  for (uint8_t x = 0; x < board_size.x; x++)
  {
    for (uint8_t y = 0; y < board_size.y; y++)
    {
      uint8_t bombs = 0;
      const Pos pos(x,y);
      Tile chosen_tile = _board_ptr->get_tile(pos);
      if (TileType::undefined == chosen_tile.type)
      {

        for (int16_t dx = -1; dx <= 1; dx++)
        {
          for (int16_t dy = -1; dy <= 1; dy++)
          {
            int16_t temp_x = x + dx;
            int16_t temp_y = y + dy;
            const Pos temp_pos(temp_x, temp_y);

            Tile temp_tile = _board_ptr->get_tile(temp_pos);
            if (TileType::bomb == temp_tile.type)
            {
              bombs++;
            }
          }
        }

        chosen_tile.type = GameBoardUtil::bombs_num_to_tileType(bombs);
        _board_ptr->set_tile(pos, chosen_tile);
      }
    }
  }

  //Reveal start position
  Tile start_tile = _board_ptr->get_tile(start_pos);
  start_tile.status = TileStatus::discovered;
  _board_ptr->set_tile(start_pos, start_tile);
  _board_ptr->set_discovered_tiles_num(1);

  //Reveal known start_pos neighbors
  if (TileType::blank == start_tile.type)
  {
    reveal_hidden_tails(start_pos);
  }
  else
  {
    reveal_check_tail(start_pos);
  }

  _armored = true;
}

void GameLogic::armor(uint8_t x, uint8_t y)
{
  armor(Pos(x,y));
}

[[nodiscard]] bool GameLogic::move_select(const Pos& pos)
{
  if (GameStatus::in_progress != _status)
  {
    return false;
  }

  return _board_ptr->set_current_pos(pos);
}

[[nodiscard]] bool GameLogic::move_select(uint8_t x, uint8_t y)
{
  return move_select(Pos(x,y));
}

[[nodiscard]] bool GameLogic::check_tile(const Pos& pos)
{
  bool result = false;
  if (_armored && GameStatus::in_progress == _status)
  {
    Tile tile = _board_ptr->get_tile(pos);
    if (TileStatus::hidden == tile.status)
    {
      if (TileType::bomb == tile.type)
      {
        _status = GameStatus::lose;
      }
      else if (TileType::blank == tile.type)
      {
        reveal_hidden_tails(pos);
      }
      else
      {
        reveal_check_tail(pos);
      }
      tile.status = TileStatus::discovered;
      _board_ptr->set_tile(pos, tile);
      _board_ptr->set_discovered_tiles_num(_board_ptr->get_discovered_tiles_num() + 1);
      
      const uint16_t known_tiles_num = _board_ptr->get_discovered_tiles_num()
       + _board_ptr->get_bombs_num();
      if (known_tiles_num >= _board_ptr->get_tiles_num())
      {
        _status = GameStatus::win;
      }
    }
  }
  else if (GameStatus::in_progress == _status)
  {
    armor(pos);
    result = true;
  }
  return result;
}

[[nodiscard]] bool GameLogic::check_tile(uint8_t x, uint8_t y)
{
  return check_tile(Pos(x,y));
}

[[nodiscard]] bool GameLogic::flag_tile(const Pos& pos)
{
  Tile check_tile = _board_ptr->get_tile(pos);
  bool state_changed = false;
  if (TileStatus::hidden == check_tile.status && GameStatus::in_progress == _status)
  {
    check_tile.status = TileStatus::flagged;
    _board_ptr->set_tile(pos, check_tile);
    _board_ptr->set_flagged_tiles_num(_board_ptr->get_flagged_tiles_num() + 1);
    state_changed = true;
  }
  else if (TileStatus::flagged == check_tile.status && GameStatus::in_progress == _status)
  {
    check_tile.status = TileStatus::hidden;
    _board_ptr->set_tile(pos, check_tile);
    _board_ptr->set_flagged_tiles_num(_board_ptr->get_flagged_tiles_num() - 1);
    state_changed = true;
  }
  return state_changed;
}

[[nodiscard]] bool GameLogic::flag_tile(uint8_t x, uint8_t y)
{
  return flag_tile(Pos(x,y));
}

[[nodiscard]] GameStatus GameLogic::get_status() const
{
  return _status;
}

[[nodiscard]] const std::weak_ptr<IGameBoard> GameLogic::get_board() const
{
  return _board_ptr;
}

//Private

void GameLogic::reveal_hidden_tails(const Pos& start_pos)
{
  const Size board_size = _board_ptr->get_board_size();
  std::vector<Pos> pos_to_check;
  pos_to_check.push_back(start_pos);

  uint16_t discovered_tiles = 0;
  while ( !pos_to_check.empty() )
  {
    const Pos check_pos = pos_to_check.back();
    pos_to_check.pop_back();

    for (int16_t dx = -1; dx <= 1; dx++)
    {
      for (int16_t dy = -1; dy <= 1; dy++)
      {
        int16_t temp_x = check_pos.x + dx;
        int16_t temp_y = check_pos.y + dy;
        const Pos temp_pos(temp_x, temp_y);

        Tile temp_tile = _board_ptr->get_tile(temp_pos);
        if (TileStatus::hidden == temp_tile.status && TileType::undefined != temp_tile.type)
        {
          if (TileType::blank == temp_tile.type)
          {
            pos_to_check.push_back(temp_pos);
          }
          temp_tile.status = TileStatus::discovered;
          discovered_tiles++;
          _board_ptr->set_tile(temp_pos, temp_tile);
        } 
      }
    }
  }
  _board_ptr->set_discovered_tiles_num(_board_ptr->get_discovered_tiles_num() + discovered_tiles);
}

void GameLogic::reveal_check_tail(const Pos& check_pos)
{
  const Size board_size = _board_ptr->get_board_size();
  const Tile check_tile = _board_ptr->get_tile(check_pos);
  const uint8_t bombs_expected = GameBoardUtil::tileType_to_bomb_num(check_tile.type);

  uint8_t flag_count = 0;
  std::vector<Pos> hidden_tiles;
  for (int16_t dx = -1; dx <= 1; dx++)
  {
    for (int16_t dy = -1; dy <= 1; dy++)
    {
      int16_t temp_x = check_pos.x + dx;
      int16_t temp_y = check_pos.y + dy;
      const Pos temp_pos(temp_x, temp_y);

      Tile temp_tile = _board_ptr->get_tile(temp_pos);
      if (TileStatus::flagged == temp_tile.status)
      {
        flag_count++;
      } 
      else if (TileStatus::hidden == temp_tile.status && TileType::undefined != temp_tile.type)
      {
        hidden_tiles.push_back(temp_pos);
      }
    }
  }

  if (bombs_expected == flag_count)
  {
    _board_ptr->set_discovered_tiles_num(_board_ptr->get_discovered_tiles_num()
      + hidden_tiles.size());
    
    while ( !hidden_tiles.empty() )
    {
      const Pos pos = hidden_tiles.back();
      hidden_tiles.pop_back();

      Tile reveal_tile = _board_ptr->get_tile(pos);
      reveal_tile.status = TileStatus::discovered;
      _board_ptr->set_tile(pos, reveal_tile);
      if (TileType::blank == reveal_tile.type)
      {
        reveal_hidden_tails(pos);
      }
    }
  }
}

void GameLogic::reveal_bomb_tails()
{
  const Size board_size = _board_ptr->get_board_size();

  for (uint8_t x = 0; x < board_size.x; x++)
  {
    for (uint8_t y = 0; y < board_size.y; y++)
    {
      const Pos pos(x,y);
      Tile chosen_tile = _board_ptr->get_tile(pos);
      if (TileType::bomb == chosen_tile.type)
      {
        chosen_tile.status = TileStatus::discovered;
        _board_ptr->set_tile(pos, chosen_tile);
      }
    }
  }
}