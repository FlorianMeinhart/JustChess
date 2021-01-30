#pragma once

#define _UINT8(x) static_cast<uint8_t>(x)
#define _INT64(x) static_cast<int64_t>(x)

namespace JC
{
  /// @brief States
  enum class eState : uint8_t
  {
    eNone = 0,
    eInCheck,
    eCheckmate,
    eStalemate
  };
  
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

  enum class eFile : uint8_t
  {
    A = 0, B, C, D, E, F, G, H
  };

  enum class eRank: uint8_t
  {
    _1 = 0, _2, _3, _4, _5, _6, _7, _8
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

    {std::pair(ePiece::pawn, true), {{_UINT8(eRank::_2),_UINT8(eFile::A)}, {_UINT8(eRank::_2),_UINT8(eFile::B)}, 
                                     {_UINT8(eRank::_2),_UINT8(eFile::C)}, {_UINT8(eRank::_2),_UINT8(eFile::D)},
                                     {_UINT8(eRank::_2),_UINT8(eFile::E)}, {_UINT8(eRank::_2),_UINT8(eFile::F)}, 
                                     {_UINT8(eRank::_2),_UINT8(eFile::G)}, {_UINT8(eRank::_2),_UINT8(eFile::H)}}},
    {std::pair(ePiece::pawn, false), {{_UINT8(eRank::_7),_UINT8(eFile::A)}, {_UINT8(eRank::_7),_UINT8(eFile::B)}, 
                                      {_UINT8(eRank::_7),_UINT8(eFile::C)}, {_UINT8(eRank::_7),_UINT8(eFile::D)},
                                      {_UINT8(eRank::_7),_UINT8(eFile::E)}, {_UINT8(eRank::_7),_UINT8(eFile::F)},
                                      {_UINT8(eRank::_7),_UINT8(eFile::G)}, {_UINT8(eRank::_7),_UINT8(eFile::H)}}},

    {std::pair(ePiece::rook, true), {{_UINT8(eRank::_1),_UINT8(eFile::A)}, {_UINT8(eRank::_1),_UINT8(eFile::H)}}},
    {std::pair(ePiece::rook, false), {{_UINT8(eRank::_8),_UINT8(eFile::A)}, {_UINT8(eRank::_8),_UINT8(eFile::H)}}},

    {std::pair(ePiece::knight, true), {{_UINT8(eRank::_1),_UINT8(eFile::B)}, {_UINT8(eRank::_1),_UINT8(eFile::G)}}},
    {std::pair(ePiece::knight, false), {{_UINT8(eRank::_8),_UINT8(eFile::B)}, {_UINT8(eRank::_8),_UINT8(eFile::G)}}},

    {std::pair(ePiece::bishop, true), {{_UINT8(eRank::_1),_UINT8(eFile::C)}, {_UINT8(eRank::_1),_UINT8(eFile::F)}}},
    {std::pair(ePiece::bishop, false), {{_UINT8(eRank::_8),_UINT8(eFile::C)}, {_UINT8(eRank::_8),_UINT8(eFile::F)}}},

    {std::pair(ePiece::queen, true), {{_UINT8(eRank::_1),_UINT8(eFile::D)}}},
    {std::pair(ePiece::queen, false), {{_UINT8(eRank::_8),_UINT8(eFile::D)}}},

    {std::pair(ePiece::king, true), {{_UINT8(eRank::_1),_UINT8(eFile::E)}}},
    {std::pair(ePiece::king, false), {{_UINT8(eRank::_8),_UINT8(eFile::E)}}},
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