#pragma once
#include "IGameBoard.hpp"
#include <cinttypes>
#include <memory>

enum class DifficultyLevel
{
  easy,
  medium,
  hard,
  custom,
  waiting,
  quit_opt
};

DifficultyLevel& operator++(DifficultyLevel& oryg);
DifficultyLevel& operator--(DifficultyLevel& oryg);

enum class GameStatus
{
  lose,
  win,
  in_progress,
  preparing
};

class IGameLogic
{
public:
  virtual void set_difficulty_level(const DifficultyLevel& diff) = 0;
  virtual void set_custom_params(const Size& size, uint8_t bombs_num) = 0;
  virtual void set_custom_params(uint8_t x, uint8_t y, uint8_t bombs_num) = 0;
  virtual void set_board(IGameBoard * board_ptr, bool armor) = 0;
  virtual void clean() = 0;
  virtual void Start() = 0;
  virtual void armor(const Pos& start_pos) = 0;
  virtual void armor(uint8_t x, uint8_t y) = 0;

  [[nodiscard]] virtual bool move_select(const Pos& pos) = 0;
  [[nodiscard]] virtual bool move_select(uint8_t x, uint8_t y) = 0;
  
  [[nodiscard]] virtual bool check_tile(const Pos& pos) = 0;
  [[nodiscard]] virtual bool check_tile(uint8_t x, uint8_t y) = 0;

  [[nodiscard]] virtual bool flag_tile(const Pos& pos) = 0;
  [[nodiscard]] virtual bool flag_tile(uint8_t x, uint8_t y) = 0;

  [[nodiscard]] virtual GameStatus get_status() const = 0;
  virtual void set_status_lose() = 0;
  [[nodiscard]] virtual const std::weak_ptr<IGameBoard> get_board() const = 0;

  virtual ~IGameLogic() = default;
};