#pragma once
#include "interface/IInput3D.hpp"
#include <SFML2/Window.hpp>
#include <SFML2/Graphics.hpp>

class InputSFML final : public IInput3D
{
public:
  InputSFML(sf::RenderWindow& window);

  void init() override;
  void set_board(const std::weak_ptr<IGameBoard> && board_ptr) override;
  [[nodiscard]] Move get_input() override;
  [[nodiscard]] DifficultyLevel get_difficulty() override;
  [[nodiscard]] bool start() override;

  [[nodiscard]] std::tuple<StartState, bool> Start3D(const StartState& state) override;
  [[nodiscard]] std::tuple<Move, ViewParams> get_input_3D(const ViewParams& params) override;
  [[nodiscard]] std::tuple<DifficultyLevel, ViewParams, bool> get_difficulty_3D(
    const DifficultyLevel& diff_level) override;

  ~InputSFML() = default;
private:
  std::weak_ptr<IGameBoard> _board_ptr;
  sf::RenderWindow& _window;
};