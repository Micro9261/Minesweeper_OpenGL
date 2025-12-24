#include <gtest/gtest.h>
#include "GameBoard.hpp"
#include "IGameBoard.hpp"
#include <memory>
#include <cinttypes>

TEST( GameBoard, CreationWithoutParams )
{
  std::unique_ptr<IGameBoard> board(std::make_unique<GameBoard>());
  ASSERT_EQ(board->get_board_size(), Size(0,0));
  ASSERT_EQ(board->get_bombs_num(), 0);
  ASSERT_EQ(board->get_current_pos(), Pos(0,0));
  ASSERT_EQ(board->get_discovered_tiles_num(), 0);
  ASSERT_EQ(board->get_flagged_tiles_num(), 0);
  ASSERT_EQ(board->get_tiles_num(), 0);
  ASSERT_EQ(board->get_tile(0,0), Tile());
}

TEST( GameBoard, CreationWithParams )
{
  constexpr uint8_t bombs_num = 10;
  constexpr uint16_t tiles_num = 6 * 6;
  const Size board_size(6,6);

  std::unique_ptr<IGameBoard> board(std::make_unique<GameBoard>(board_size, bombs_num));
  ASSERT_EQ(board->get_board_size(), board_size);
  ASSERT_EQ(board->get_bombs_num(), bombs_num);
  ASSERT_EQ(board->get_current_pos(), Pos(3,3));
  ASSERT_EQ(board->get_discovered_tiles_num(), 0);
  ASSERT_EQ(board->get_flagged_tiles_num(), 0);
  ASSERT_EQ(board->get_tiles_num(), tiles_num);
  ASSERT_EQ(board->get_tile(0,0), Tile());
}

TEST( GameBoard, SetTile)
{
  constexpr uint8_t bombs_num = 10;
  const Size board_size(6,6);
  const Pos pos_within_borad(3,3);
  const Pos pos_out_of_board(6,6);

  std::unique_ptr<IGameBoard> board(std::make_unique<GameBoard>(board_size, bombs_num));
  const Tile tile_test(TileType::bomb, TileStatus::hidden);
  board->set_tile(pos_within_borad, tile_test);

  ASSERT_EQ(board->get_tile(pos_within_borad), tile_test);
  ASSERT_EQ(board->get_tile(pos_out_of_board), Tile());
}

TEST (GameBoard, BoardParametersSetAndGet)
{
  constexpr uint8_t bombs_num = 10;
  const Size board_size(6,6);

  std::unique_ptr<IGameBoard> board(std::make_unique<GameBoard>(board_size, bombs_num));
  //current pos test
  const Pos pos_within_borad(3,3);
  const Pos pos_out_of_board(6,6);

  board->set_current_pos(pos_within_borad);
  ASSERT_EQ(board->get_current_pos(), pos_within_borad);

  board->set_current_pos(pos_out_of_board);
  ASSERT_EQ(board->get_current_pos(), pos_within_borad);

  //discovered tiles num
  constexpr uint16_t discovered_tiles_num = 20;
  board->set_discovered_tiles_num(discovered_tiles_num);
  ASSERT_EQ(board->get_discovered_tiles_num(), discovered_tiles_num);

  constexpr uint16_t discovered_tiles_num_too_much = 300;
  board->set_discovered_tiles_num(discovered_tiles_num_too_much);
  ASSERT_EQ(board->get_discovered_tiles_num(), discovered_tiles_num);

  //flagged tiles
  constexpr uint16_t flagged_tiles_num = 4;
  board->set_flagged_tiles_num(flagged_tiles_num);
  ASSERT_EQ(board->get_flagged_tiles_num(), flagged_tiles_num);

  constexpr uint16_t flagged_tiles_num_too_much = 120;
  board->set_flagged_tiles_num(flagged_tiles_num_too_much);
  ASSERT_EQ(board->get_flagged_tiles_num(), flagged_tiles_num);
}