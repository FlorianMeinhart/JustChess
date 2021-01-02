#include <stdafx.h>

#include "ChessBoard.h"
#include "..\ChessPieces\OtherPieces.h"

namespace JC
{

  CChessBoard::piece_t CChessBoard::GetPieceType(eRank rank, eFile file)
  {
    if (!m_board[rank][file])
    {
      return std::pair(ePiece::none, false);
    }
    return std::pair(m_board[rank][file]->GetType(), m_board[rank][file]->IsWhite());
  }

  JC::CChessBoard::boolmat_t CChessBoard::GetValidMoves(eRank rank, eFile file, bool forWhite)
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
        std::cout << s_charRepMap[m_record[uind][rank][file]] << " | ";
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

  char CChessBoard::PieceCharRep(const std::unique_ptr<CChessPiece>& pPiece)
  {
    if (pPiece)
      return s_charRepMap[std::pair(pPiece->GetType(), pPiece->IsWhite())];
    else
      return s_charRepMap[std::pair(ePiece::none, true)];
  }
}
