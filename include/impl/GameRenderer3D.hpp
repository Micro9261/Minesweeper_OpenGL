#pragma once
#include "interface/IGameRenderer.hpp"
#include <map>
#include <string>
#include <GL/glu.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <map>
#include <SFML2/Window.hpp>
#include <SFML2/Graphics.hpp>
#include <SFML2/OpenGL.hpp>

struct Glyph {
    GLuint texture;
    int width;
    int height;
    int bearingX;
    int bearingY;
    int advance;
};

class GameRenderer3D final : public IGameRenderer
{
public:
  GameRenderer3D(sf::RenderWindow& window);
  GameRenderer3D(sf::RenderWindow& window, float tile_edge, float board_edge, float board_height);
  
  void init() override;
  void set_board(const std::weak_ptr<IGameBoard> && board_ptr) override;
  void render() override;
  void render_win() override;
  void render_lose() override;
  void render_difficulty_selection() override;
  void render_ask_start() override;
  void clear() override;

private:
  Size _board_size;
  std::weak_ptr<IGameBoard> _board_ptr;
  sf::RenderWindow& _window;

  std::map<char, Glyph> _glyphs;
  FT_Library _ft_lib;
  FT_Face _ft_face;

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
  void _draw_flag(const Pos& pos);
  void _draw_board(uint8_t tiles_x, uint8_t tiles_z);
};