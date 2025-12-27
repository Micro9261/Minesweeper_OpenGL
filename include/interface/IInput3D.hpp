#pragma once
#include "interface/IInput.hpp"
#include "interface/IGameRenderer3D.hpp"
#include <tuple>

enum class StartState
{
  start,
  selection_screen,
  quit,
  waiting,
};

StartState& operator--(StartState& oryg);
StartState& operator++(StartState& oryg);

class IInput3D : public IInput
{
public:
  [[nodiscard]] virtual std::tuple<StartState, bool> Start3D(const StartState& state) = 0;
  [[nodiscard]] virtual std::tuple<Move, ViewParams> get_input_3D(const ViewParams& params) = 0;
  [[nodiscard]] virtual std::tuple<DifficultyLevel, ViewParams, bool> get_difficulty_3D(
    const DifficultyLevel& diff_level) = 0;

  virtual ~IInput3D() = default;
};