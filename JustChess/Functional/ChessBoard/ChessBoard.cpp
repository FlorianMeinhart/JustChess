﻿#include <stdafx.h>

#include "ChessBoard.h"
#include "..\ChessPieces\OtherPieces.h"

namespace JC
{

  CChessBoard::piece_t CChessBoard::GetPieceType(eRank rank, eFile file) const
  {
    if (!m_board[rank][file])
    {
      return std::pair(ePiece::none, false);
    }
    return std::pair(m_board[rank][file]->GetType(), m_board[rank][file]->IsWhite());
  }

  JC::CChessBoard::boolmat_t CChessBoard::GetValidMoves(eRank rank, eFile file, bool forWhite) const
  {
    boolmat_t boolmat(RANKS, std::vector<bool>(FILES));
    auto& piece = m_board[rank][file];
    if (!piece || piece->GetType() == ePiece::none || piece->IsWhite() != forWhite)
    {
      return boolmat;
    }

    for (const auto& dir : piece->GetMoveDirs())
    {
      int newRank = rank + dir.first;
      int newFile = file + dir.second;

      do // repeat when piece can move multiple steps
      {
        if (newRank >= RANKS || newRank < 0 || newFile >= FILES || newFile < 0)
        {
          break; // outside of the board
        }
        auto newPieceType = GetPieceType(static_cast<eRank>(newRank), static_cast<eFile>(newFile));

        //check if new position on board is free to move
        if (newPieceType.first == ePiece::none)
        {
          //#TODO check if player is checked after this move (then move would not be allowed)
          boolmat[newRank][newFile] = true;
        }
        //check if at new position is a piece of the opponent
        else if (newPieceType.second != forWhite)
        {
          //#TODO check if player is checked after this move (then move would not be allowed)
          boolmat[newRank][newFile] = true;
          break; // exit loop because piece can't move further in this direction after capturing

          newRank += dir.first;
          newFile += dir.second;
        }
      } while (piece->CanMoveMultipleSteps());
    }
    // special treatment for pawns
    if (piece->GetType() == ePiece::pawn && rank != r_1 && rank != r_8)
    {
      if (GetPieceType(static_cast<eRank>(rank + (forWhite ? 1 : -1)), file).first == ePiece::none)
      {
        //#TODO check if player is checked after this move (then move would not be allowed)
        boolmat[rank + (forWhite ? 1 : -1)][file] = true;
        // check if double step is possible
        if (rank == (forWhite ? r_2 : r_7))
        {
          if (GetPieceType(static_cast<eRank>(rank + (forWhite ? 2 : -2)), file).first == ePiece::none)
          {
            //#TODO check if player is checked after this move (then move would not be allowed)
            boolmat[rank + (forWhite ? 2 : -2)][file] = true;
          }
        }
      }
      // check if pawn can capture a piece
      if (file >= f_B)
      {
        auto leftPiece = GetPieceType(static_cast<eRank>(rank + (forWhite ? 1 : -1)), static_cast<eFile>(file - 1));
        if (leftPiece.first != ePiece::none && leftPiece.second != forWhite)
        {
          //#TODO check if player is checked after this move (then move would not be allowed)
          boolmat[rank + (forWhite ? 1 : -1)][file - 1] = true;
        }
      }
      if (file <= f_G)
      {
        auto rightPiece = GetPieceType(static_cast<eRank>(rank + (forWhite ? 1 : -1)), static_cast<eFile>(file + 1));
        if (rightPiece.first != ePiece::none && rightPiece.second != forWhite)
        {
          //#TODO check if player is checked after this move (then move would not be allowed)
          boolmat[rank + (forWhite ? 1 : -1)][file + 1] = true;
        }
      }
    }
    return boolmat;
  }

  bool CChessBoard::IsChecked(bool forWhite) const
  {
    const auto& kingPos = forWhite ? m_whiteKingPos : m_blackKingPos;
    const auto& oppKingPos = forWhite ? m_blackKingPos : m_whiteKingPos;

    if (m_record.empty())
    {
      return false;
    }
    // assert that kings are actually at correct (saved) position
    DEBUG_ASSERT(m_record.back()[kingPos.first][kingPos.second].first == ePiece::king &&
                 m_record.back()[kingPos.first][kingPos.second].second == forWhite);
    DEBUG_ASSERT(m_record.back()[oppKingPos.first][oppKingPos.second].first == ePiece::king &&
                 m_record.back()[oppKingPos.first][oppKingPos.second].second != forWhite);

    // check if king is checked by one the following pieces (queen is included in bishop and rook)
    if (IsCheckedPieceDirection(ePiece::bishop, forWhite) ||
        IsCheckedPieceDirection(ePiece::rook, forWhite)   ||
        IsCheckedPieceDirection(ePiece::knight, forWhite))
    {
      return true;
    }

    // check if king is checked by a pawn
    if (forWhite)
    {
      if (kingPos.first < r_7)
      {
        if (kingPos.second >= f_B &&
            m_record.back()[kingPos.first + 1][kingPos.second - 1].second != forWhite &&
            m_record.back()[kingPos.first + 1][kingPos.second - 1].first == ePiece::pawn)
        {
          return true;
        }
        if (kingPos.second <= f_G &&
          m_record.back()[kingPos.first + 1][kingPos.second + 1].second != forWhite &&
          m_record.back()[kingPos.first + 1][kingPos.second + 1].first == ePiece::pawn)
        {
          return true;
        }
      }
    }
    
    // check if king is checked by other king (in the game not possible, but validation
    // necessary in order to get the next valid moves of the king
    if (std::abs(kingPos.first - oppKingPos.first) <= 1 &&
        std::abs(kingPos.second - oppKingPos.second) <= 1)
    {
      return true;
    }
    return false;
  }

  std::pair<eRank, eFile> CChessBoard::FindKing(const view_t& boardView, bool forWhite) const
  {
    for (std::uint16_t rankInt = 0; rankInt < boardView.size(); rankInt++)
    {
      for (std::uint16_t fileInt = 0; fileInt < boardView[rankInt].size(); fileInt++)
      {
        if (boardView[rankInt][fileInt].first == ePiece::king &&
          boardView[rankInt][fileInt].second == forWhite)
        {
          return std::pair(static_cast<eRank>(rankInt), static_cast<eFile>(fileInt));
        }
      }
    }
    DEBUG_ASSERT(false); // no king found, should never happen
    return std::pair(r_1, f_A);
  }

  void CChessBoard::Reset()
  {
    m_board.clear();
    m_board.resize(RANKS);
    for (auto& rank : m_board)
    {
      rank.resize(FILES);
    }

    for (auto type : PIECES)
    {
      for (bool isWhite : {true, false})
      {
        for (const auto& pos : CChessPiece::GetStartPositions(type, isWhite))
        {
          auto [rank, file] = pos;
          m_board[rank][file] = std::move(CreatePiece(type, isWhite));
        }
      }
    }

    m_whiteKingPos = std::pair(r_1, f_E);
    m_blackKingPos = std::pair(r_8, f_E);
  }

  void CChessBoard::PrintCurrentBoard()
  {
    std::cout << "   +---+---+---+---+---+---+---+---+";

    for (int rank = RANKS-1; rank >= 0; rank--)
    {
      std::cout << "\n " << rank + 1 << " | ";
      for (int file = 0; file < FILES; file++)
      {
        std::cout << PieceCharRep(m_board[rank][file]) << " | ";
      }
      std::cout << "\n   +---+---+---+---+---+---+---+---+";
    }
    std::cout << "\n     A   B   C   D   E   F   G   H" << std::endl;
  }

  void CChessBoard::PrintBoolMat(boolmat_t boolmat)
  {
    std::cout << "   +---+---+---+---+---+---+---+---+";

    for (int rank = RANKS - 1; rank >= 0; rank--)
    {
      std::cout << "\n " << rank + 1 << " | ";
      for (int file = 0; file < FILES; file++)
      {
        std::cout << (boolmat[rank][file] ? 'X' : ' ') << " | ";
      }
      std::cout << "\n   +---+---+---+---+---+---+---+---+";
    }
    std::cout << "\n     A   B   C   D   E   F   G   H" << std::endl;
  }

  void CChessBoard::PrintRecord(int ind)
  {
    std::uint32_t uind;

    if (ind <= -1)
    {
      uind = m_record.empty() ? 0 : static_cast<std::uint32_t>(m_record.size() - 1);
    }
    else
    {
      uind = ind;
    }
    if (uind >= m_record.size())
    {
      m_logger->Error("Index " + std::to_string(uind) + " exceeds number of records (" +
        std::to_string(m_record.size()) + ")", __FILE__, __LINE__);
      return;
    }
    std::cout << "   +---+---+---+---+---+---+---+---+";

    for (int rank = RANKS - 1; rank >= 0; rank--)
    {
      std::cout << "\n " << rank + 1 << " | ";
      for (int file = 0; file < FILES; file++)
      {
        std::cout << s_charRepMap.at(m_record[uind][rank][file]) << " | ";
      }
      std::cout << "\n   +---+---+---+---+---+---+---+---+";
    }
    std::cout << "\n     A   B   C   D   E   F   G   H" << std::endl;
  }

  std::unique_ptr<JC::CChessPiece> CChessBoard::CreatePiece(ePiece type, bool isWhite)
  {
    switch (type)
    {
    case ePiece::pawn:
      return std::make_unique<CPawn>(isWhite);
    case ePiece::rook:
      return std::make_unique<CRook>(isWhite);
    case ePiece::knight:
      return std::make_unique<CKnight>(isWhite);
    case ePiece::bishop:
      return std::make_unique<CBishop>(isWhite);
    case ePiece::queen:
      return std::make_unique<CQueen>(isWhite);
    case ePiece::king:
      return std::make_unique<CKing>(isWhite);
    default:
      return nullptr;
    }
  }

  void CChessBoard::CreateNextRecord()
  {
    view_t view;
    view.resize(RANKS);
    for (int rank = 0; rank < RANKS; rank++)
    {
      view[rank].resize(FILES);
      for (int file = 0; file < FILES; file++)
      {
        view[rank][file] = m_board[rank][file] ? 
        std::pair(m_board[rank][file]->GetType(), m_board[rank][file]->IsWhite()) :
        std::pair(ePiece::none, true);
      }
    }
    m_record.push_back(view);
  }

  char CChessBoard::PieceCharRep(const std::unique_ptr<CChessPiece>& pPiece) const
  {
    if (pPiece)
      return s_charRepMap.at(std::pair(pPiece->GetType(), pPiece->IsWhite()));
    else
      return s_charRepMap.at(std::pair(ePiece::none, true));
  }

  bool CChessBoard::IsCheckedPieceDirection(ePiece pieceDir, bool forWhite) const
  {
    const auto& kingPos = forWhite ? m_whiteKingPos : m_blackKingPos;

    bool bishopOrRook = (pieceDir == ePiece::bishop || pieceDir == ePiece::rook);

    int newRank;
    int newFile;

    for (const auto& dir : s_moveDirMap.at(pieceDir))
    {
      newRank = kingPos.first;
      newFile = kingPos.second;

      do {
        newRank += dir.first;
        newFile += dir.second;

        if (newRank >= RANKS || newRank < 0 || newFile >= FILES || newFile < 0)
        {
          break; // outside of the board
        }
        // check if piece is in the way
        if (m_record.back()[newRank][newFile].first != ePiece::none)
        {
          // break if own piece is in the way
          if (m_record.back()[newRank][newFile].second == forWhite)
          {
            break;
          }
          // check if current piece (or queen) is in the way
          if (m_record.back()[newRank][newFile].first == pieceDir ||
              (bishopOrRook && m_record.back()[newRank][newFile].first == ePiece::queen))
          {
            return true;
          }
          // else no piece to give check is in the way
          break;
        }
      } while (bishopOrRook);
    }
    return false;
  }

}
