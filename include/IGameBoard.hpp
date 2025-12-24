#pragma once
#include <cinttypes>

enum class TileType
{
  blank,
  undefined,
  bomb,
  bombs_1,
  bombs_2,
  bombs_3,
  bombs_4,
  bombs_5,
  bombs_6,
  bombs_7,
  bombs_8
};

namespace GameBoardUtil {
  TileType bombs_num_to_tileType(uint8_t bombs_num);

  uint8_t tileType_to_bomb_num(const TileType& type);
};
enum class TileStatus
{
  hidden,
  discovered,
  flagged
};

struct Tile
{
  TileType type;
  TileStatus status;

  Tile()
    : Tile(TileType::undefined, TileStatus::hidden)
  {}

  Tile(const TileType& type, const TileStatus& status)
    : type(type), status(status)
  {}

  bool operator==(const Tile& tile) const
  {
    return (tile.type == type && tile.status == status);
  }

  bool operator!=(const Tile& tile) const
  {
    return !(tile == * this);
  }

  Tile(const Tile&) = default;
  Tile& operator=(const Tile&) = default;
  ~Tile() = default;
};

struct Pos
{
  uint8_t x;
  uint8_t y;

  Pos()
    : Pos(0,0)
  {}

  Pos(uint8_t pos_x, uint8_t pos_y)
    : x(pos_x), y(pos_y)
  {}

  bool operator==(const Pos& pos) const
  {
    return (pos.x == x && pos.y == y);
  }

  bool operator!=(const Pos& pos) const
  {
    return !(pos == *this);
  }

  Pos(const Pos&) = default;
  Pos& operator=(const Pos&) = default;
  ~Pos() = default;
};

struct Size
{
  uint8_t x;
  uint8_t y;

  Size()
    : Size(0,0)
  {}

  Size(uint8_t size_x, uint8_t size_y)
    : x(size_x), y(size_y)
  {}

  bool operator==(const Size& size) const
  {
    return (size.x == x && size.y == y);
  }

  bool operator!=(const Size& size) const
  {
    return !(size == *this);
  }

  Size(const Size&) = default;
  Size& operator=(const Size&) = default;
  ~Size() = default;
};

class IGameBoard
{
public:
  virtual void resize_and_clear() = 0;

  [[nodiscard]] virtual Tile get_tile(uint8_t x, uint8_t y) const = 0;
  [[nodiscard]] virtual Tile get_tile(const Pos& pos) const = 0;
  [[nodiscard]] virtual bool set_tile(uint8_t x, uint8_t y, const Tile& tile) = 0;
  [[nodiscard]] virtual bool set_tile(const Pos& pos, const Tile& tile) = 0;

  [[nodiscard]] virtual uint8_t get_bombs_num() const = 0;
  virtual void set_bombs_num(uint8_t bombs_num) = 0;

  [[nodiscard]] virtual uint16_t get_discovered_tiles_num() const = 0;
  virtual void set_discovered_tiles_num(uint16_t disc_num) = 0;

  [[nodiscard]] virtual uint16_t get_flagged_tiles_num() const = 0;
  virtual void set_flagged_tiles_num(uint16_t flag_num) = 0;
  
  [[nodiscard]] virtual uint16_t get_tiles_num() const = 0;

  [[nodiscard]] virtual Size get_board_size() const = 0;
  virtual void set_board_size(const Size& board_size) = 0;

  [[nodiscard]] virtual Pos get_current_pos() const = 0;
  [[nodiscard]] virtual bool set_current_pos(const Pos& pos) = 0;

  virtual ~IGameBoard() = default;
};