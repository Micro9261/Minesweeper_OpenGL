#pragma once
#include "interface/IGameRenderer.hpp"

class GameRendererStdout final : public IGameRenderer
{
public:
  GameRendererStdout();
  
  void init() override;
  void set_board(const std::weak_ptr<IGameBoard> && board_ptr) override;
  void render() override;
  void render_win() override;
  void render_lose() override;
  void render_difficulty_selection(const DifficultyLevel&) override;
  void render_ask_start(const StartState&) override;
  void clear() override;

private:
  Size _board_size;
  std::weak_ptr<IGameBoard> _board_ptr;

  void change_color(int text_col, int bg_col);
  void render_title();
  
};