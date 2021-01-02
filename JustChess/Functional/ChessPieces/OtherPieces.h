#pragma once
#include "ChessPiece.h"

namespace JC
{
  class CPawn : public CChessPiece
  {
  public:
    CPawn(bool isWhite)
      : CChessPiece(ePiece::pawn, isWhite)
    {
    };
  };

  class CRook : public CChessPiece
  {
  public:
    CRook(bool isWhite)
      : CChessPiece(ePiece::rook, isWhite)
    {
    };
  };

  class CKnight : public CChessPiece
  {
  public:
    CKnight(bool isWhite)
      : CChessPiece(ePiece::knight, isWhite)
    {
    };
  };

  class CBishop : public CChessPiece
  {
  public:
    CBishop(bool isWhite)
      : CChessPiece(ePiece::bishop, isWhite)
    {
    };
  };

  class CQueen : public CChessPiece
  {
  public:
    CQueen(bool isWhite)
      : CChessPiece(ePiece::queen, isWhite)
    {
    };
  };

  class CKing : public CChessPiece
  {
  public:
    CKing(bool isWhite)
      : CChessPiece(ePiece::king, isWhite)
    {
    };
  };
}