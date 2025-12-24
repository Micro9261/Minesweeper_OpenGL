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

enum class TileStatus
{
  hidden,
  discovered,
  flagged
};

struct Tile
{
  TileType tile_type;
  TileStatus tile_status;

  Tile()
    : tile_type(TileType::undefined), tile_status(TileStatus::hidden)
  {}

  Tile(const Tile&) = default;
  Tile& operator=(const Tile&) = default;
  ~Tile() = default;
};

struct Pos
{
  uint8_t x;
  uint8_t y;

  Pos()
    : x(0), y(0)
  {}

  Pos(const Pos&) = default;
  Pos& operator=(const Pos&) = default;
  ~Pos() = default;
};

struct Size
{
  uint8_t x;
  uint8_t y;

  Size()
    : x(0), y(0)
  {}

  Size(const Size&) = default;
  Size& operator=(const Size&) = default;
  ~Size() = default;
};

class IGameBoard
{
public:
  [[nodiscard]] virtual Tile get_tile(uint8_t x, uint8_t y) const = 0;
  virtual void set_tile(uint8_t x, uint8_t y, const Tile& tile) = 0;

  [[nodiscard]] virtual uint8_t get_bombs_num() const = 0;
  virtual void set_bombs_num(uint8_t bombs_num) = 0;

  [[nodiscard]] virtual uint16_t get_tiles_num() const = 0;
  [[nodiscard]] virtual uint16_t get_discovered_tiles_num() const = 0;
  [[nodiscard]] virtual uint16_t get_flagged_tiles_num() const = 0;
  [[nodiscard]] virtual Size get_board_size() const = 0;

  [[nodiscard]] virtual Pos get_current_pos() const = 0;
  virtual void set_current_pos(const Pos& pos) = 0;

  virtual ~IGameBoard() = default;
};