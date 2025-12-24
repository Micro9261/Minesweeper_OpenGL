#include "IGameBoard.hpp"

namespace GameBoardUtil {
  TileType bombs_num_to_tileType(uint8_t bombs_num)
  {
    TileType value = TileType::undefined;
    switch (bombs_num)
    {
      case 0:
        value = TileType::blank;
        break;
        
      case 1:
        value = TileType::bombs_1;
        break;

      case 2:
        value = TileType::bombs_2;
        break;

      case 3:
        value = TileType::bombs_3;
        break;

      case 4:
        value = TileType::bombs_4;
        break;

      case 5:
        value = TileType::bombs_5;
        break;

      case 6:
        value = TileType::bombs_6;
        break;

      case 7:
        value = TileType::bombs_7;
        break;

      case 8:
        value = TileType::bombs_8;
        break;
      
      default:
        //Nothing
        break;
    }

    return value;
  }

  uint8_t tileType_to_bomb_num(const TileType& type)
  {
    uint8_t value = 0;
    switch (type)
    {
      case TileType::blank:
        value = 0;
        break;
        
      case TileType::bombs_1:
        value = 1;
        break;

      case TileType::bombs_2:
        value = 2;
        break;

      case TileType::bombs_3:
        value = 3;
        break;

      case TileType::bombs_4:
        value = 4;
        break;

      case TileType::bombs_5:
        value = 5;
        break;

      case TileType::bombs_6:
        value = 6;
        break;

      case TileType::bombs_7:
        value = 7;
        break;

      case TileType::bombs_8:
        value = 8;
        break;
      
      default:
        //Nothing
        break;
    }

    return value;
  }
};