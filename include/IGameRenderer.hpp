#pragma once
#include "IGameBoard.hpp"
#include <memory>

class IGameRenderer
{
public:
  virtual void init(const Size& board_size) = 0;
  virtual void set_board(const std::weak_ptr<IGameBoard> && board_ptr) = 0;
  virtual void render() = 0;
  virtual void render_win() = 0;
  virtual void render_lose() = 0;
  virtual void clear() = 0;

  virtual ~IGameRenderer() = default;
};