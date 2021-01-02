#pragma once

namespace JC
{
  
  /// @brief Types of chess pieces
  enum class ePiece : uint8_t
  {
    none = 0,
    pawn,
    rook,
    knight,
    bishop,
    queen,
    king
  };

  /// @brief Array of possible pieces
  static const ePiece PIECES[] = {
    ePiece::pawn,
    ePiece::rook,
    ePiece::knight,
    ePiece::bishop,
    ePiece::queen,
    ePiece::king 
  };

  enum eFile : uint8_t
  {
    f_A = 0, f_B, f_C, f_D, f_E, f_F, f_G, f_H
  };

  enum eRank: uint8_t
  {
    r_1 = 0, r_2, r_3, r_4, r_5, r_6, r_7, r_8
  };
  
  using vecPairRankFile_t = std::vector<std::pair<int, int>>;

  /// @brief Map holding possible move directions (rank, file) for each chess piece
  static const std::map<ePiece, vecPairRankFile_t> s_moveDirMap
  {
    {ePiece::none, {{}}},
    {ePiece::pawn, {{}}}, // has to get special treatment
    {ePiece::rook, {{1,0}, {0,1}, {-1,0}, {0,-1}}},
    {ePiece::knight, {{2,1}, {1,2}, {-1,2}, {-2,1}, {-2,-1}, {-1,-2}, {1,-2}, {2,-1}}},
    {ePiece::bishop, {{1,1}, {-1,1}, {-1,-1}, {1,-1}}},
    {ePiece::queen, {{1,1}, {-1,1}, {-1,-1}, {1,-1}, {1,0}, {0,1}, {-1,0}, {0,-1}}},
    {ePiece::king, {{1,1}, {-1,1}, {-1,-1}, {1,-1}, {1,0}, {0,1}, {-1,0}, {0,-1}}},
  };

  /// @brief Map holding start positions (rank, file) for each type of chess piece. Second key entry indicates if white (@c true) or black (@c false)
  static const std::map<std::pair<ePiece, bool>, vecPairRankFile_t> s_startPosMap
  {
    {std::pair(ePiece::none, true), {{}}},
    {std::pair(ePiece::none, false), {{}}},

    {std::pair(ePiece::pawn, true), {{r_2,f_A}, {r_2,f_B}, {r_2,f_C}, {r_2,f_D},
                                     {r_2,f_E}, {r_2,f_F}, {r_2,f_G}, {r_2,f_H}}},
    {std::pair(ePiece::pawn, false), {{r_7,f_A}, {r_7,f_B}, {r_7,f_C}, {r_7,f_D},
                                      {r_7,f_E}, {r_7,f_F}, {r_7,f_G}, {r_7,f_H}}},

    {std::pair(ePiece::rook, true), {{r_1,f_A}, {r_1,f_H}}},
    {std::pair(ePiece::rook, false), {{r_8,f_A}, {r_8,f_H}}},

    {std::pair(ePiece::knight, true), {{r_1,f_B}, {r_1,f_G}}},
    {std::pair(ePiece::knight, false), {{r_8,f_B}, {r_8,f_G}}},

    {std::pair(ePiece::bishop, true), {{r_1,f_C}, {r_1,f_F}}},
    {std::pair(ePiece::bishop, false), {{r_8,f_C}, {r_8,f_F}}},

    {std::pair(ePiece::queen, true), {{r_1,f_D}}},
    {std::pair(ePiece::queen, false), {{r_8,f_D}}},

    {std::pair(ePiece::king, true), {{r_1,f_E}}},
    {std::pair(ePiece::king, false), {{r_8,f_E}}},
  };

  /// @brief Map indicating which chess piece can make multiple moves
  static const std::map<ePiece, bool> s_moveMultipleStepsMap
  {
    {ePiece::none, false},
    {ePiece::pawn, false},
    {ePiece::rook, true},
    {ePiece::knight, false},
    {ePiece::bishop, true},
    {ePiece::queen, true},
    {ePiece::king, false},
  };

  /// @brief Map for chess piece representation with a character.
  /// Second element in the key pair indicates if white (or black).
  static const std::map<std::pair<ePiece, bool>, char> s_charRepMap
  {
    {{ePiece::none, true}, ' '},
    {{ePiece::pawn, true}, 'P'},
    {{ePiece::rook, true}, 'R'},
    {{ePiece::knight, true}, 'N'},
    {{ePiece::bishop, true}, 'B'},
    {{ePiece::queen, true}, 'Q'},
    {{ePiece::king, true}, 'K'},
    {{ePiece::none, false}, ' '},
    {{ePiece::pawn, false}, 'p'},
    {{ePiece::rook, false}, 'r'},
    {{ePiece::knight, false}, 'n'},
    {{ePiece::bishop, false}, 'b'},
    {{ePiece::queen, false}, 'q'},
    {{ePiece::king, false}, 'k'},
  };
}