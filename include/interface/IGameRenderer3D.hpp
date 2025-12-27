#pragma once
#include "interface/IGameRenderer.hpp"
#include <cmath>
#include <array>
#include <span>

struct ViewParams
{
  std::array<float, 3> params;
  //[0] distance
  //[1] theta
  //[2] phi

  ViewParams()
    : ViewParams({0.f, 0.f, 0.f})
  {}
  
  ViewParams(std::initializer_list<float> ang)
  {
    
    
    std::initializer_list<float>::iterator iter = ang.begin();
    uint8_t index = 0;
    while (index < 3)
    {
      if (iter != ang.end())
      {
        if (index == 0)
        {
          params[index] = *iter;
        }
        else
        {
          params[index] = deg_to_rad(*iter);
        }
        iter++;
      }
      else
      {
        if (index == 0)
        {
          params[index] = *iter;
        }
        else
        {
          params[index] = deg_to_rad(*iter);
        }
      }
      index++;
    }
  }

  [[nodiscard]] float get_x() const
  {
    float distance = params[0];
    float theta = params[1];
    float phi = params[2];
    return distance * std::cos(phi) * std::cos(theta);
  }

  [[nodiscard]] float get_y() const
  {
    float distance = params[0];
    float theta = params[1];
    float phi = params[2];
    return distance * std::sin(phi);
  }

  [[nodiscard]] float get_z() const
  {
    float distance = params[0];
    float theta = params[1];
    float phi = params[2];
    return distance * std::cos(phi) * std::sin(theta);
  }

  void set_distance(float dist)
  {
    params[0] = dist;
  }

  void set_theta(float theta)
  {
    params[1] = deg_to_rad(theta);
  }

  void set_phi(float phi)
  {
    params[2] = deg_to_rad(phi);
  }

  [[nodiscard]] float get_distance() const
  {
    return params[0];
  }

  [[nodiscard]] float get_theta() const
  {
    return rad_to_deg(params[1]);
  }

  [[nodiscard]] float get_phi() const
  {
    return rad_to_deg(params[2]);
  }

  static float deg_to_rad(float deg)
  {
    const float pi = std::acos(-1);
    return deg * (pi / 180.f);
  }

  static float rad_to_deg(float rad)
  {
    const float pi = std::acos(-1);
    return rad * (180.f / pi);
  }

  ViewParams(const ViewParams&) = default;
  ViewParams& operator=(const ViewParams&) = default;

  ~ViewParams() = default;
};

class IGameRenderer3D : public IGameRenderer
{
public:
  virtual void render_view(const ViewParams& view) = 0;

  virtual ~IGameRenderer3D() = default;
};