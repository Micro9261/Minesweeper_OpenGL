#pragma once
#include "IGameRenderer.hpp"

class GameRendererStdout final : public IGameRenderer
{
public:
  GameRendererStdout();
  
  void init(const Size& board_size) override;
  void set_board(const std::weak_ptr<IGameBoard> && board_ptr) override;
  void render() override;
  void render_win() override;
  void render_lose() override;
  void clear() override;

private:
  Size _board_size;
  std::weak_ptr<IGameBoard> _board_ptr;

  void change_color(int text_col, int bg_col);
};