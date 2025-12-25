#pragma once
#include "interface/IInput.hpp"

class InputStdin final : public IInput
{
public:
  InputStdin();

  void init() override;
  void set_board(const std::weak_ptr<IGameBoard> && board_ptr) override;
  [[nodiscard]] Move get_input() override;
  [[nodiscard]] DifficultyLevel get_difficulty() override;
  [[nodiscard]] bool start() override;

  ~InputStdin() = default;
private:
  std::weak_ptr<IGameBoard> _board_ptr;

  char get_input_safe();
};