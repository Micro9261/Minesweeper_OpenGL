#include "impl/GameRendererSFML.hpp"
#include <memory>
#include <iostream>
#include <vector>

GameRendererSFML::GameRendererSFML(sf::RenderWindow& window)
  : _window(window)
{
  _font_width = 0.10f;
  _font_height = 0.14f;
  _board_edge = 0.05f;

  //Tile params
  _tile_edge = 0.2f;
  _tile_slope = 0.03f;
  _tile_top_edge_offset = 0.01f;
  _tile_edge_offset = 0.01f;
}

GameRendererSFML::GameRendererSFML(sf::RenderWindow& window, float _tile_edge, float _board_edge, float board_height)
  : _window(window)
{

}
  
void GameRendererSFML::init()
{
  _load_fonts("assets/arial.ttf");
  _font.loadFromFile("assets/arial.ttf");
  glClearColor(0.7f,0.7f,0.7f,0.f);
  clear();
  int width = _window.getSize().x;
  int height = _window.getSize().y;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.f, width / (float)height, 1.f, 100.f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void GameRendererSFML::set_board(const std::weak_ptr<IGameBoard> && board_ptr) 
{
  _board_ptr = std::move(board_ptr);
}

void GameRendererSFML::render() 
{
  if (_board_ptr.expired())
    return;

   std::shared_ptr<IGameBoard> board = _board_ptr.lock();
  _board_size = board->get_board_size();

  _draw_board(_board_size.x, _board_size.y);
  _draw_selection(board->get_current_pos());

  for (uint8_t x = 0; x < _board_size.x; x++)
  {
    for (uint8_t y = 0; y < _board_size.y; y++)
    {
      const Pos pos(y,x);
      const Tile tile = board->get_tile(pos);
      _draw_tile(pos, tile);
      
    }
  }
}

void GameRendererSFML::render_win() 
{
  _window.pushGLStates();
  int width = _window.getSize().x;
  int height = _window.getSize().y;
  
  sf::Text text("You WIN!", _font);
  text.setFillColor(sf::Color::Magenta);
  text.setStyle(sf::Text::Bold);
  sf::Vector2f text_pos(static_cast<float>(width)/2.f, static_cast<float>(height)/2.f);
  text.setPosition(text_pos);
   
  _window.draw(text);

  _window.popGLStates();
}

void GameRendererSFML::render_lose() 
{
  _window.pushGLStates();
  int width = _window.getSize().x;
  int height = _window.getSize().y;
  
  sf::Text text("You LOSE!", _font);
  text.setFillColor(sf::Color::Magenta);
  text.setStyle(sf::Text::Bold);
  sf::Vector2f text_pos(static_cast<float>(width)/2.f, static_cast<float>(height)/2.f);
  text.setPosition(text_pos);
   
  _window.draw(text);

  _window.popGLStates();
}

void GameRendererSFML::render_difficulty_selection(const DifficultyLevel& diff_level) 
{
  _window.pushGLStates();
  int width = _window.getSize().x;
  int height = _window.getSize().y;
  const float middle_x = static_cast<float>(width) / 2.f;
  const float middle_y = static_cast<float>(height) / 2.f;
  const float options_spacing = 50.f;
  const float lines_num = 6.f;
  const float start_x = middle_x - 100.f;
  const float start_y = middle_y - options_spacing * (lines_num / 2.f);

  
  std::vector<std::pair<DifficultyLevel, sf::Text> > option_vec;
  //Prepare texts

  sf::Text text("Difficulty selection", _font);
  text.setFillColor(sf::Color::White);
  text.setStyle(sf::Text::Bold);
  sf::Vector2f text_pos(start_x, start_y);
  text.setPosition(text_pos);

  const float selection_width = 100.f;
  const float selection_height = options_spacing - 10.f;
  sf::RectangleShape selection({selection_width, selection_height});
  selection.setFillColor(sf::Color::Blue);

  option_vec.emplace_back(DifficultyLevel::easy, sf::Text("Easy", _font) );
  option_vec.emplace_back(DifficultyLevel::medium, sf::Text("Medium", _font) );
  option_vec.emplace_back(DifficultyLevel::hard, sf::Text("Hard", _font) );
  option_vec.emplace_back(DifficultyLevel::custom, sf::Text("Custom", _font) );
  option_vec.emplace_back(DifficultyLevel::quit_opt, sf::Text("Quit", _font) );

  std::vector<std::pair<DifficultyLevel, sf::Text> >::iterator iter = option_vec.begin();
  float pos_y = start_y + options_spacing;
  while (iter != option_vec.end())
  {
    sf::Text & elem = iter->second;
    elem.setStyle(sf::Text::Bold);
    sf::Vector2f pos(start_x, pos_y);
    elem.setPosition(pos);
    pos_y += options_spacing;

    if (diff_level != iter->first)
    {
      elem.setFillColor(sf::Color::Blue);
    }
    else
    {
      selection.setPosition(pos);
      elem.setFillColor(sf::Color::White);
    }
    iter++;
  }

  _window.draw(text);
  _window.draw(selection);

  iter = option_vec.begin();
  while(iter != option_vec.end())
  {
    _window.draw(iter->second);
    iter++;
  }

  _window.popGLStates();
}

void GameRendererSFML::render_ask_start() 
{
  _window.pushGLStates();
  int width = _window.getSize().x;
  int height = _window.getSize().y;
  const float middle_x = static_cast<float>(width) / 2.f;
  const float middle_y = static_cast<float>(height) / 2.f;
  const float options_spacing = 50.f;
  const float lines_num = 3.f;
  const float start_x = middle_x - 100.f;
  const float start_y = middle_y - options_spacing * (lines_num / 2.f);
  
  sf::Text text("Do you wish to start?", _font);
  text.setFillColor(sf::Color::Magenta);
  text.setStyle(sf::Text::Bold);
  sf::Vector2f text_pos(static_cast<float>(width)/2.f - 100.f, static_cast<float>(height)/2.f - 100.f);
  text.setPosition(text_pos);

  sf::Text option_text("Click 'S' to start, 'Q' to go back to difficulty selection", _font);
  option_text.setFillColor(sf::Color::Magenta);
  option_text.setStyle(sf::Text::Bold);
  text_pos = sf::Vector2f(static_cast<float>(width)/2.f - 300.f, static_cast<float>(height)/2.f);
  option_text.setPosition(text_pos); 

  _window.draw(text);
  _window.draw(option_text);

  _window.popGLStates();
}

void GameRendererSFML::render_view(const ViewParams& view)
{
  glLoadIdentity();
  ViewParams north_of_camera({view.get_distance(), view.get_theta(), view.get_phi() + 0.01f});
  gluLookAt(view.get_x(), view.get_y(), view.get_z(),
    0.0, 0.0, 0.0,
    north_of_camera.get_x(), north_of_camera.get_y(), north_of_camera.get_z());
}

//Private

void GameRendererSFML::clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _draw_background(_window, sf::Clock().getElapsedTime().asSeconds());
}

void GameRendererSFML::_draw_background(sf::RenderWindow& window, float timeSec)
{
    // Static shader and quad so we compile/create once
    static sf::Shader shader;
    static sf::VertexArray quad;
    static bool initialized = false;

    if (!initialized) {
        // Vertex shader (GLSL 1.20)
        const std::string vertSrc = R"(
            #version 120
            attribute vec2 aPos;
            varying vec2 vUV;
            void main() {
                vUV = aPos * 0.5 + 0.5;
                gl_Position = vec4(aPos, 0.0, 1.0);
            }
        )";

        // Fragment shader (GLSL 1.20) - procedural cosmic look
        const std::string fragSrc = R"(
            #version 120
            varying vec2 vUV;
            uniform vec2 u_resolution;
            uniform float u_time;

            float hash(vec2 p) {
                return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
            }

            float noise(vec2 p) {
                vec2 i = floor(p);
                vec2 f = fract(p);
                float a = hash(i + vec2(0.0,0.0));
                float b = hash(i + vec2(1.0,0.0));
                float c = hash(i + vec2(0.0,1.0));
                float d = hash(i + vec2(1.0,1.0));
                vec2 u = f*f*(3.0-2.0*f);
                return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
            }

            float fbm(vec2 p) {
                float v = 0.0;
                float amp = 0.5;
                for (int i = 0; i < 5; ++i) {
                    v += amp * noise(p);
                    p *= 2.0;
                    amp *= 0.5;
                }
                return v;
            }

            void main() {
                vec2 uv = (vUV * u_resolution - 0.5 * u_resolution) / min(u_resolution.x, u_resolution.y);
                vec2 p = uv * 2.0 + vec2(u_time * 0.03, u_time * 0.01);

                float n1 = fbm(p * 1.2);
                float n2 = fbm(p * 3.0 - vec2(u_time * 0.08));
                float neb = smoothstep(0.15, 0.8, n1 * 0.6 + n2 * 0.4);

                vec3 col = mix(vec3(0.02, 0.03, 0.08), vec3(0.9, 0.2, 0.6), neb);

                float starNoise = fbm(p * 40.0 + 10.0);
                float stars = pow(max(0.0, starNoise - 0.85), 6.0);
                col += vec3(1.0) * stars * 1.5;

                float speck = step(0.995, hash(floor(p * 200.0)));
                col += vec3(1.0) * speck * 0.9;

                float dist = length(uv);
                col *= smoothstep(1.2, 0.6, dist);

                gl_FragColor = vec4(col, 1.0);
            }
        )";

        // Load shader from memory (SFML will compile it)
        if (!shader.loadFromMemory(vertSrc, fragSrc)) {
            std::cerr << "Failed to load background shader\n";
            return;
        }

        // Create a fullscreen quad (two triangles) in NDC (-1..1)
        quad.setPrimitiveType(sf::Triangles);
        quad.resize(6);

        // Triangle 1
        quad[0].position = sf::Vector2f(-1.f, -1.f);
        quad[1].position = sf::Vector2f( 1.f, -1.f);
        quad[2].position = sf::Vector2f( 1.f,  1.f);
        // Triangle 2
        quad[3].position = sf::Vector2f(-1.f, -1.f);
        quad[4].position = sf::Vector2f( 1.f,  1.f);
        quad[5].position = sf::Vector2f(-1.f,  1.f);

        initialized = true;
    }

    // Prepare uniforms
    sf::Vector2f resolution((float)window.getSize().x, (float)window.getSize().y);
    shader.setUniform("u_time", timeSec);
    shader.setUniform("u_resolution", resolution);

    // When mixing raw OpenGL and SFML drawing, preserve GL states:
    // pushGLStates/popGLStates ensures SFML's draw uses its own state and restores GL afterwards.
    window.pushGLStates();

    // Draw the quad with the shader. We must tell SFML to use the shader's attribute mapping:
    // SFML maps vertex attributes automatically: position -> "gl_Vertex" / attribute 0 in GLSL 1.20.
    // The vertex shader above expects attribute "aPos" bound to location 0; SFML binds position there.
    sf::RenderStates states;
    states.shader = &shader;
    // No texture, no transform (we already use NDC positions)
    window.draw(quad, states);

    window.popGLStates();
}

void GameRendererSFML::_load_fonts(const char* path)
{
    if (FT_Init_FreeType(&_ft_lib))
    {
      std::cout << "FT_init_error" << std::endl;
      return;
    }

    FT_Error err = FT_New_Face(_ft_lib, path, 0, &_ft_face);
    if (err)
    {
      std::cout << "FT_face_error" << err << std::endl;
      return;
    }

    FT_Set_Pixel_Sizes(_ft_face, 0, 20);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (char c = '1'; c <= '8'; ++c)
    {
        FT_Load_Char(_ft_face, c, FT_LOAD_RENDER);

        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            _ft_face->glyph->bitmap.width,
            _ft_face->glyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            _ft_face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        _glyphs[c] = {
            tex,
            (int)_ft_face->glyph->bitmap.width,
            (int)_ft_face->glyph->bitmap.rows,
            _ft_face->glyph->bitmap_left,
            _ft_face->glyph->bitmap_top,
            (int)_ft_face->glyph->advance.x
        };
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(_ft_face);
    FT_Done_FreeType(_ft_lib);
}

void GameRendererSFML::_draw_text3D(char c, float x, float y)
{
  Glyph& g = _glyphs[c];

  const float start_x = x + (_tile_edge - _font_width) / 2.f;
  const float start_z = y + (_tile_edge - _font_height) / 2.f;

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D, g.texture);

  glBegin(GL_QUADS);
    glColor4f(0.0f, 0.000f, 0.804f, 1.0f);
    glTexCoord2f(0, 0); glVertex3f(start_x, 0.002f, start_z);
    glTexCoord2f(0, 1); glVertex3f(start_x, 0.002f, start_z + _font_height);
    glTexCoord2f(1, 1); glVertex3f(start_x + _font_width, 0.002f, start_z + _font_height);
    glTexCoord2f(1, 0); glVertex3f(start_x + _font_width, 0.002f, start_z);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
}

void GameRendererSFML::_draw_selection(const Pos& pos)
{
  const float start_x = -_board_start_x + _board_edge + pos.x * _tile_edge;
  const float start_z = -_board_start_z + _board_edge + pos.y * _tile_edge;

  glBegin(GL_QUADS);
    glColor3f(1.000f, 0.549f, 0.000f);
    glVertex3f(start_x, 0.0005f, start_z);
    glVertex3f(start_x + _tile_edge, 0.0005f, start_z);
    glVertex3f(start_x + _tile_edge, 0.0005f, start_z + _tile_edge);
    glVertex3f(start_x, 0.0005f, start_z + _tile_edge);
  glEnd();
}

void GameRendererSFML::_draw_tile(const Pos& pos, const Tile& tile)
{
  const float start_x = -_board_start_x + _board_edge + pos.x * _tile_edge + _tile_edge_offset;
  const float start_z = -_board_start_z + _board_edge + pos.y * _tile_edge + _tile_edge_offset;
  const float tile_edge_draw = _tile_edge - 2*_tile_edge_offset;
  _tile_top_edge = tile_edge_draw - 2*(_tile_slope + _tile_top_edge_offset);
  const float start_top_x = start_x + _tile_slope + _tile_top_edge_offset;
  const float start_top_z = start_z + _tile_slope + _tile_top_edge_offset;

  if (TileStatus::hidden == tile.status || TileStatus::flagged == tile.status)
  {
    glBegin(GL_QUADS);
      //left face
      glColor3f(0.125f, 0.698f, 0.667f);
      glVertex3f(start_x, 0.f, start_z);
      glVertex3f(start_x, 0.f, start_z + tile_edge_draw);
      glVertex3f(start_x + _tile_slope, _tile_slope, start_z + tile_edge_draw - _tile_slope);
      glVertex3f(start_x + _tile_slope, _tile_slope, start_z + _tile_slope);

      //up fac
      glColor3f(0.125f, 0.698f, 0.667f);
      glVertex3f(start_x, 0.f, start_z);
      glVertex3f(start_x + tile_edge_draw, 0.f, start_z);
      glVertex3f(start_x + tile_edge_draw - _tile_slope, _tile_slope, start_z + _tile_slope);
      glVertex3f(start_x + _tile_slope, _tile_slope, start_z + _tile_slope);

      //right face
      glColor3f(0.125f, 0.698f, 0.667f);
      glVertex3f(start_x + tile_edge_draw, 0.f, start_z);
      glVertex3f(start_x + tile_edge_draw, 0.f, start_z + tile_edge_draw);
      glVertex3f(start_x + tile_edge_draw - _tile_slope, _tile_slope, start_z + tile_edge_draw - _tile_slope);
      glVertex3f(start_x + tile_edge_draw - _tile_slope, _tile_slope, start_z + _tile_slope);

      //down face
      glColor3f(0.125f, 0.698f, 0.667f);
      glVertex3f(start_x, 0.f, start_z + tile_edge_draw);
      glVertex3f(start_x + tile_edge_draw, 0.f, start_z + tile_edge_draw);
      glVertex3f(start_x + tile_edge_draw - _tile_slope, _tile_slope, start_z + tile_edge_draw - _tile_slope);
      glVertex3f(start_x + _tile_slope, _tile_slope, start_z + tile_edge_draw - _tile_slope);

      //top face
      glColor3f(0.125f, 0.698f, 0.667f);
      glVertex3f(start_top_x, _tile_slope, start_top_z);
      glVertex3f(start_top_x + _tile_top_edge, _tile_slope, start_top_z);
      glVertex3f(start_top_x + _tile_top_edge, _tile_slope, start_top_z + _tile_top_edge);
      glVertex3f(start_top_x, _tile_slope, start_top_z + _tile_top_edge);

    glEnd();

    if (TileStatus::flagged == tile.status)
    {
      _draw_flag(start_top_x, start_top_z);
    }
  }
  else if (TileStatus::discovered == tile.status && TileType::bomb != tile.type)
  {
    glBegin(GL_QUADS);
      glColor3f(0.561f, 0.737f, 0.561f);
      glVertex3f(start_x, 0.001f, start_z);
      glVertex3f(start_x + tile_edge_draw, 0.001f, start_z);
      glVertex3f(start_x + tile_edge_draw, 0.001f, start_z + tile_edge_draw);
      glVertex3f(start_x, 0.001f, start_z + tile_edge_draw);
    glEnd();
    char num = GameBoardUtil::tileType_to_bomb_num(tile.type);
    if (0 != num)
    {
      //From binary number to ASCII (add 0x30)
      _draw_text3D(num + 0x30, start_x, start_z);
    }
  } else if (TileStatus::discovered == tile.status && TileType::bomb == tile.type)
  {
    glBegin(GL_QUADS);
      glColor3f(1.f, 0.f, 0.f);
      glVertex3f(start_x, 0.001f, start_z);
      glVertex3f(start_x + tile_edge_draw, 0.001f, start_z);
      glVertex3f(start_x + tile_edge_draw, 0.001f, start_z + tile_edge_draw);
      glVertex3f(start_x, 0.001f, start_z + tile_edge_draw);
    glEnd();
  }
}

void GameRendererSFML::_draw_flag(float pos_x, float pos_z)
{
  const float flag_width = 0.05f;
  const float flag_heigth = 0.025f;
  const float stick_heigth = 0.07f;
  const float stick_width = 0.02f;

  const float start_flag_x = pos_x + (_tile_top_edge - flag_width)/2.f;
  const float start_flag_z = pos_z + (_tile_top_edge - stick_heigth)/2.f;
  const float flag_tile_height = _tile_slope + 0.001f;

  glBegin(GL_QUADS);
    //flag upper part
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(start_flag_x, flag_tile_height, start_flag_z);
    glVertex3f(start_flag_x + flag_width, flag_tile_height, start_flag_z);
    glVertex3f(start_flag_x + flag_width, flag_tile_height, start_flag_z + flag_heigth);
    glVertex3f(start_flag_x, flag_tile_height, start_flag_z + flag_heigth);

    //flag lower part
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(start_flag_x, flag_tile_height, start_flag_z + flag_heigth);
    glVertex3f(start_flag_x + stick_width, flag_tile_height, start_flag_z  + flag_heigth);
    glVertex3f(start_flag_x + stick_width, flag_tile_height, start_flag_z + stick_heigth);
    glVertex3f(start_flag_x, flag_tile_height, start_flag_z + stick_heigth);

  glEnd();
}

void GameRendererSFML::_draw_board(uint8_t tiles_x, uint8_t tiles_z)
{
    const float _board_edge = 0.05f;

    const float board_width = _board_edge*2.f + tiles_x * _tile_edge;
    const float board_height = _board_edge*2.f + tiles_z * _tile_edge;
    const float start_x = board_width / 2.f;
    const float start_z = board_height / 2.f;
    const float board_layer_height = 0.05f;

    _board_start_x = start_x;
    _board_start_z = start_z;

    glBegin(GL_QUADS);

      //Board Bottom
      glColor3f(0.8f, 0.8f, 0.8f);
      glVertex3f(-start_x, -board_layer_height, -start_z);
      glVertex3f(-start_x + board_width, -board_layer_height, -start_z);
      glVertex3f(-start_x + board_width, -board_layer_height, -start_z + board_height);
      glVertex3f(-start_x, -board_layer_height, -start_z + board_height);

      //Board left bottom face
      glColor3f(0.8f, 0.8f, 0.8f);
      glVertex3f(-start_x, -board_layer_height, -start_z);
      glVertex3f(-start_x, -board_layer_height, -start_z + board_height);
      glVertex3f(-start_x, 0.f, -start_z + board_height);
      glVertex3f(-start_x, 0.f, -start_z);

      //Board up bottom face
      glColor3f(0.8f, 0.8f, 0.8f);
      glVertex3f(-start_x, -board_layer_height, -start_z);
      glVertex3f(-start_x + board_width, -board_layer_height, -start_z);
      glVertex3f(-start_x + board_width, 0.f, -start_z);
      glVertex3f(-start_x, 0.f, -start_z);

      //Board right bottom face
      glColor3f(0.8f, 0.8f, 0.8f);
      glVertex3f(-start_x + board_width, -board_layer_height, -start_z);
      glVertex3f(-start_x + board_width, -board_layer_height, -start_z + board_height);
      glVertex3f(-start_x + board_width, 0.f, -start_z + board_height);
      glVertex3f(-start_x + board_width, 0.f, -start_z);

      //Board down bottom face
      glColor3f(0.8f, 0.8f, 0.8f);
      glVertex3f(-start_x, -board_layer_height, -start_z + board_height);
      glVertex3f(-start_x + board_width, -board_layer_height, -start_z + board_height);
      glVertex3f(-start_x + board_width, 0.f, -start_z + board_height);
      glVertex3f(-start_x, 0.f, -start_z + board_height);

      //Board game face
      glColor3f(0.f, 0.f, 0.f);
      glVertex3f(-start_x, 0.f, -start_z);
      glVertex3f(-start_x + board_width, 0.f, -start_z);
      glVertex3f(-start_x + board_width, 0.f, -start_z + board_height);
      glVertex3f(-start_x, 0.f, -start_z + board_height);

      //Board edge left
      //left face
      glColor3f(0.5f, 0.7f, 1.f);
      glVertex3f(-start_x, 0.f, -start_z);
      glVertex3f(-start_x, 0.f, -start_z + board_height);
      glVertex3f(-start_x, board_layer_height, -start_z + board_height);
      glVertex3f(-start_x, board_layer_height, -start_z);

      //right face
      glColor3f(0.5f, 0.7f, 1.f);
      glVertex3f(-start_x + _board_edge, 0.f, -start_z + _board_edge);
      glVertex3f(-start_x + _board_edge, 0.f, -start_z + board_height - _board_edge);
      glVertex3f(-start_x + _board_edge, board_layer_height, -start_z + board_height - _board_edge);
      glVertex3f(-start_x + _board_edge, board_layer_height, -start_z + _board_edge);

      //top face
      glColor3f(0.5f, 0.7f, 1.f);
      glVertex3f(-start_x, board_layer_height, -start_z);
      glVertex3f(-start_x + _board_edge, board_layer_height, -start_z + _board_edge);
      glVertex3f(-start_x + _board_edge, board_layer_height, -start_z + board_height - _board_edge);
      glVertex3f(-start_x, board_layer_height, -start_z + board_height);
      
      //Board edge up
      //up face
      glColor3f(0.5f, 0.7f, 1.f);
      glVertex3f(-start_x, 0.f, -start_z);
      glVertex3f(-start_x + board_width, 0.f, -start_z);
      glVertex3f(-start_x + board_width, board_layer_height, -start_z);
      glVertex3f(-start_x, board_layer_height, -start_z);

      //down face
      glColor3f(0.5f, 0.7f, 1.f);
      glVertex3f(-start_x + _board_edge, 0.f, -start_z + _board_edge);
      glVertex3f(-start_x + board_width - _board_edge, 0.f, -start_z + _board_edge);
      glVertex3f(-start_x + board_width - _board_edge, board_layer_height, -start_z + _board_edge);
      glVertex3f(-start_x + _board_edge, board_layer_height, -start_z + _board_edge);

      //top face
      glColor3f(0.5f, 0.7f, 1.f);
      glVertex3f(-start_x, board_layer_height, -start_z);
      glVertex3f(-start_x + _board_edge, board_layer_height, -start_z + _board_edge);
      glVertex3f(-start_x + board_width - _board_edge, board_layer_height, -start_z + _board_edge);
      glVertex3f(-start_x + board_width, board_layer_height, -start_z);

      //Board edge right
      //right face
      glColor3f(0.863f, 0.078f, 0.235f);
      glVertex3f(-start_x + board_width, 0.f, -start_z);
      glVertex3f(-start_x + board_width, 0.f, -start_z + board_height);
      glVertex3f(-start_x + board_width, board_layer_height, -start_z + board_height);
      glVertex3f(-start_x + board_width, board_layer_height, -start_z);

      //left face
      glColor3f(0.863f, 0.078f, 0.235f);
      glVertex3f(-start_x + board_width - _board_edge, 0.f, -start_z + _board_edge);
      glVertex3f(-start_x + board_width - _board_edge, 0.f, -start_z + board_height - _board_edge);
      glVertex3f(-start_x + board_width - _board_edge, board_layer_height, -start_z + board_height - _board_edge);
      glVertex3f(-start_x + board_width - _board_edge, board_layer_height, -start_z + _board_edge);

      //top face
      glColor3f(0.863f, 0.078f, 0.235f);
      glVertex3f(-start_x + board_width, board_layer_height, -start_z);
      glVertex3f(-start_x + board_width - _board_edge, board_layer_height, -start_z + _board_edge);
      glVertex3f(-start_x + board_width - _board_edge, board_layer_height, -start_z + board_height - _board_edge);
      glVertex3f(-start_x  + board_width, board_layer_height, -start_z + board_height);

      //Board edge down
      //down face
      glColor3f(0.863f, 0.078f, 0.235f);
      glVertex3f(-start_x, 0.f, -start_z  + board_height);
      glVertex3f(-start_x + board_width, 0.f, -start_z  + board_height);
      glVertex3f(-start_x + board_width, board_layer_height, -start_z  + board_height);
      glVertex3f(-start_x, board_layer_height, -start_z  + board_height);

      //up face
      glColor3f(0.863f, 0.078f, 0.235f);
      glVertex3f(-start_x + _board_edge, 0.f, -start_z + board_height - _board_edge);
      glVertex3f(-start_x + board_width - _board_edge, 0.f, -start_z + board_height - _board_edge);
      glVertex3f(-start_x + board_width - _board_edge, board_layer_height, -start_z + board_height - _board_edge);
      glVertex3f(-start_x + _board_edge, board_layer_height, -start_z  + board_height - _board_edge);

      //top face
      glColor3f(0.863f, 0.078f, 0.235f);
      glVertex3f(-start_x, board_layer_height, -start_z  + board_height);
      glVertex3f(-start_x + _board_edge, board_layer_height, -start_z + board_height - _board_edge);
      glVertex3f(-start_x + board_width - _board_edge, board_layer_height, -start_z + board_height - _board_edge);
      glVertex3f(-start_x + board_width, board_layer_height, -start_z  + board_height);

    glEnd();
}
