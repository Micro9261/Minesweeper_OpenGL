#pragma once
#include "IGameLogic.hpp"
#include <memory>

#include "IGameBoard.hpp"

class GameLogic : public IGameLogic
{
public:
  GameLogic();
  GameLogic(IGameBoard * board_ptr);
  explicit GameLogic(const DifficultyLevel& diff, IGameBoard * board_ptr);
  explicit GameLogic(const Size& size, uint8_t bombs_num, IGameBoard * board_ptr);
  explicit GameLogic(uint8_t x, uint8_t y, uint8_t bombs_num, IGameBoard * board_ptr);

  void set_difficulty_level(const DifficultyLevel& diff) override;
  void set_custom_params(const Size& size, uint8_t bombs_num) override;
  void set_custom_params(uint8_t x, uint8_t y, uint8_t bombs_num) override;
  void set_board(IGameBoard * board_ptr, bool armor = false) override;
  void clean() override;
  void Start() override;
  void armor(const Pos& start_pos) override;
  void armor(uint8_t x, uint8_t y) override;
  

  [[nodiscard]] bool move_select(const Pos& pos) override;
  [[nodiscard]] bool move_select(uint8_t x, uint8_t y) override;
  
  [[nodiscard]] bool check_tile(const Pos& pos) override;
  [[nodiscard]] bool check_tile(uint8_t x, uint8_t y) override;

  [[nodiscard]] bool flag_tile(const Pos& pos) override;
  [[nodiscard]] bool flag_tile(uint8_t x, uint8_t y) override;
  
  [[nodiscard]] GameStatus get_status() const override;
  [[nodiscard]] const std::weak_ptr<IGameBoard> get_board() const override;

  ~GameLogic() = default;
private:
  std::shared_ptr<IGameBoard> _board_ptr;
  DifficultyLevel _difficulty;
  GameStatus _status;
  bool _armored;

  void reveal_hidden_tails(const Pos& start_pos);
  void reveal_check_tail(const Pos& check_pos);
  void reveal_bomb_tails();
};