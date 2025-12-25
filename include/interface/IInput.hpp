#pragma once
#include "IGameBoard.hpp"
#include "IGameLogic.hpp"
#include <memory>

enum class Action
{
  Check,
  flag,
  no_action,
  reset,
  change_difficulty
};

struct Move
{
  Action action;
  Pos pos;

  Move()
    : Move(Action::no_action, Pos(0,0))
  {}

  Move(const Action& act, const Pos& pos)
    : action(act), pos(pos)
  {}

  Move(const Move&) = default;
  Move& operator=(const Move&) = default;
  ~Move() = default;
};

class IInput
{
public:
  virtual void init() = 0;
  virtual void set_board(const std::weak_ptr<IGameBoard> && board_ptr) = 0;
  [[nodiscard]] virtual Move get_input() = 0;
  [[nodiscard]] virtual DifficultyLevel get_difficulty() = 0;
  [[nodiscard]] virtual bool start() = 0;

  virtual ~IInput() = default;
};