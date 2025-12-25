#include "impl/GameBoard.hpp"

GameBoard::GameBoard()
  : GameBoard({0,0}, 0)
{
  //Nothing
}

GameBoard::GameBoard(const Size& size, uint8_t bombs_num)
  : _bombs_num(bombs_num), _tiles_num(size.x * size.y), _discovered_tiles_num(0),
  _flagged_tiles_num(0), _current_pos(size.x/2, size.y/2), _board_size(size.x, size.y)
{
  for (int i = 0; i < size.x; i ++)
  {
    _board_map.emplace_back(size.y);
  }
}

void GameBoard::resize_and_clear()
{
  for (int i = 0; i < _board_map.size(); i++)
  {
    _board_map[i].clear();
  }
  _board_map.clear();

  for (int i = 0; i < _board_size.x; i++)
  {
    _board_map.emplace_back(_board_size.y);
  }
  _bombs_num = 0;
  _tiles_num = _board_size.x * _board_size.y;
  _discovered_tiles_num = 0;
  _flagged_tiles_num = 0;
  _current_pos = Pos(_board_size.x/2, _board_size.y/2);
}

[[nodiscard]] GameBoard::tile_type GameBoard::get_tile(uint8_t x, uint8_t y) const
{
  if ( x < _board_size.x  && y < _board_size.y)
  {
    return _board_map[x][y];
  }
  else
  {
    return tile_type();
  }
}

[[nodiscard]] GameBoard::tile_type GameBoard::get_tile(const pos_type& pos) const
{
  return get_tile(pos.x, pos.y);
}

[[nodiscard]] bool GameBoard::set_tile(uint8_t x, uint8_t y, const tile_type& tile)
{
  if ( x < _board_size.x  && y < _board_size.y)
  {
    _board_map[x][y] = tile;
    return true;
  }

  return false;
}

[[nodiscard]] bool GameBoard::set_tile(const pos_type& pos, const tile_type& tile)
{
  return set_tile(pos.x, pos.y, tile);
}

[[nodiscard]] uint8_t GameBoard::get_bombs_num() const
{
  return _bombs_num;
}

void GameBoard::set_bombs_num(uint8_t bombs_num)
{
  _bombs_num = bombs_num;
}

[[nodiscard]] uint16_t GameBoard::get_discovered_tiles_num() const
{
  return _discovered_tiles_num;
}

void GameBoard::set_discovered_tiles_num(uint16_t disc_num)
{
  const uint16_t remaining_tiles = _tiles_num - _flagged_tiles_num;
  if ( remaining_tiles >= disc_num)
  {
    _discovered_tiles_num = disc_num;
  }
}

[[nodiscard]] uint16_t GameBoard::get_flagged_tiles_num() const
{
  return _flagged_tiles_num;
}

void GameBoard::set_flagged_tiles_num(uint16_t flag_num)
{
  const uint16_t remaining_tiles = _tiles_num - _discovered_tiles_num;
  if ( remaining_tiles >= flag_num)
  {
    _flagged_tiles_num = flag_num;
  }
}

[[nodiscard]] uint16_t GameBoard::get_tiles_num() const
{
  return _tiles_num;
}

[[nodiscard]] Size GameBoard::get_board_size() const
{
  uint8_t x = _board_map.size();
  uint8_t y = _board_map.empty() ? 0 : _board_map[0].size();

  return Size(x,y);
}

void GameBoard::set_board_size(const Size& board_size)
{
  _board_size = board_size;
}

[[nodiscard]] GameBoard::pos_type GameBoard::get_current_pos() const
{
  return _current_pos;
}

[[nodiscard]] bool GameBoard::set_current_pos(const pos_type& pos)
{
  if (pos.x < _board_size.x && pos.y < _board_size.y)
  {
    _current_pos = pos;
    return true;
  }
  return false;
}