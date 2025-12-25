#pragma once
#include "interface/IGameBoard.hpp"
#include <vector>

class GameBoard : public IGameBoard
{
public:
  using cont_type = std::vector<std::vector<Tile>>;
  using tile_type = Tile;
  using pos_type = Pos;

  GameBoard();
  GameBoard(const Size& size, uint8_t bombs_num);
  GameBoard(const GameBoard&) = default;
  GameBoard& operator=(const GameBoard&) = default;

  void resize_and_clear() override;

  [[nodiscard]] tile_type get_tile(uint8_t x, uint8_t y) const override;
  [[nodiscard]] tile_type get_tile(const pos_type& pos) const override;
  [[nodiscard]] bool set_tile(uint8_t x, uint8_t y, const tile_type& tile) override;
  [[nodiscard]] bool set_tile(const pos_type& pos, const tile_type& tile) override;

  [[nodiscard]] uint8_t get_bombs_num() const override;
  void set_bombs_num(uint8_t bombs_num) override;

  
  [[nodiscard]] uint16_t get_discovered_tiles_num() const override;
  void set_discovered_tiles_num(uint16_t disc_num) override;

  [[nodiscard]] uint16_t get_flagged_tiles_num() const override;
  void set_flagged_tiles_num(uint16_t flag_num) override;

  [[nodiscard]] uint16_t get_tiles_num() const override;

  [[nodiscard]] Size get_board_size() const override;
  void set_board_size(const Size& board_size) override;

  [[nodiscard]] pos_type get_current_pos() const override;
  [[nodiscard]] bool set_current_pos(const pos_type& pos) override;

  ~GameBoard() = default;
private:
  cont_type _board_map;
  Size _board_size;
  uint8_t _bombs_num;
  uint16_t _tiles_num;
  uint16_t _discovered_tiles_num;
  uint16_t _flagged_tiles_num;
  pos_type _current_pos;
};