#pragma once
#include "interface/IGameRenderer3D.hpp"
#include <map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <map>

// #include <SFML2/Window.hpp>
// #include <SFML2/Graphics.hpp>
// #include <SFML2/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <GL/glu.h>

struct Glyph {
    GLuint texture;
    int width;
    int height;
    int bearingX;
    int bearingY;
    int advance;
};

class GameRendererSFML final : public IGameRenderer3D
{
public:
  GameRendererSFML(sf::RenderWindow& window);
  GameRendererSFML(sf::RenderWindow& window, float tile_edge, float board_edge, float board_height);
  
  void init() override;
  void set_board(const std::weak_ptr<IGameBoard> && board_ptr) override;
  void render() override;
  void render_win() override;
  void render_lose() override;
  void render_difficulty_selection(const DifficultyLevel& diff_level) override;
  void render_ask_start(const StartState& state) override;
  void clear() override;

  void render_view(const ViewParams& view) override;

  ~GameRendererSFML() = default;
private:
  Size _board_size;
  std::weak_ptr<IGameBoard> _board_ptr;
  sf::RenderWindow& _window;

  std::map<char, Glyph> _glyphs;
  FT_Library _ft_lib;
  FT_Face _ft_face;
  sf::Font _font;
  sf::Clock _clock;

  //Board params
  float _font_width; //0.10f
  float _font_height; //0.14f
  float _board_start_x;
  float _board_start_z;
  float _board_edge;  //0.05f

  //Tile params
  float _tile_edge; //0.2f
  float _tile_slope; //0.03f
  float _tile_top_edge;
  float _tile_top_edge_offset; //0.01f
  float _tile_edge_offset; //0.01f

  void _draw_background(sf::RenderWindow& window, float timeSec);
  void _draw_text3D(char c, float x, float y);
  void _load_fonts(const char * path);
  void _draw_selection(const Pos& pos);
  void _draw_tile(const Pos& pos, const Tile& tile);
  void _draw_flag(float pos_x, float pos_z);
  void _draw_board(uint8_t tiles_x, uint8_t tiles_z);
  void _draw_info(bool game_move_info);
  void _draw_filled_board();
};