#pragma once
#include "interface/IInput.hpp"
#include "interface/IGameRenderer3D.hpp"
#include <tuple>
class IInput3D : public IInput
{
public:
  [[nodiscard]] virtual std::tuple<StartState, bool> start_3D(const StartState& state) = 0;
  [[nodiscard]] virtual std::tuple<Move, ViewParams> get_input_3D(const ViewParams& params) = 0;
  [[nodiscard]] virtual std::tuple<DifficultyLevel, ViewParams, bool> get_difficulty_3D(
    const DifficultyLevel& diff_level) = 0;
  [[nodiscard]] virtual bool reshape() = 0;

  virtual ~IInput3D() = default;
};