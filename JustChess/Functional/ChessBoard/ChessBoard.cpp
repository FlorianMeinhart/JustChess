﻿#include <stdafx.h>

#include "ChessBoard.h"
#include "..\ChessPieces\OtherPieces.h"

namespace JC
{

  CChessBoard::piece_t CChessBoard::GetPieceType(eRank rank, eFile file) const
  {
    if (!m_board[_UINT8(rank)][_UINT8(file)])
    {
      return std::pair(ePiece::none, false);
    }
    return std::pair(m_board[_UINT8(rank)][_UINT8(file)]->GetType(), 
                     m_board[_UINT8(rank)][_UINT8(file)]->IsWhite());
  }

  JC::CChessBoard::boolmat_t CChessBoard::GetValidMoves(eRank rank, eFile file, bool forWhite) const
  {
    boolmat_t boolmat(RANKS, std::vector<bool>(FILES));
    auto& piece = m_board[_UINT8(rank)][_UINT8(file)];
    if (!piece || piece->GetType() == ePiece::none || piece->IsWhite() != forWhite)
    {
      return boolmat;
    }

    for (const auto& dir : piece->GetMoveDirs())
    {
      int newRank = _UINT8(rank) + dir.first;
      int newFile = _UINT8(file) + dir.second;

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
          if (!WouldBeCheckedAfterMove(rank, file, static_cast<eRank>(newRank),
            static_cast<eFile>(newFile), forWhite))
          {
            boolmat[newRank][newFile] = true;
          }
        }
        //check if at new position is a piece of the opponent
        else if (newPieceType.second != forWhite)
        {
          if (!WouldBeCheckedAfterMove(rank, file, static_cast<eRank>(newRank),
            static_cast<eFile>(newFile), forWhite))
          {
            boolmat[newRank][newFile] = true;
          }
          break; // exit loop because piece can't move further in this direction after capturing
        }
        //otherwise it's a piece of the same color and it can't move further
        else
        {
          break;
        }
        newRank += dir.first;
        newFile += dir.second;
      } while (piece->CanMoveMultipleSteps());
    }
    // special treatment for pawns
    if (piece->GetType() == ePiece::pawn && rank != eRank::_1 && rank != eRank::_8)
    {
      if (GetPieceType(static_cast<eRank>(_UINT8(rank) + (forWhite ? 1 : -1)), file).first == ePiece::none)
      {
        if (!WouldBeCheckedAfterMove(rank, file, static_cast<eRank>(_UINT8(rank) + (forWhite ? 1 : -1)),
          static_cast<eFile>(file), forWhite))
        {
          boolmat[_INT64(rank) + (forWhite ? 1 : -1)][_UINT8(file)] = true;
        }
        // check if double step is possible
        if (rank == (forWhite ? eRank::_2 : eRank::_7))
        {
          if (GetPieceType(static_cast<eRank>(_UINT8(rank)+ (forWhite ? 2 : -2)), file).first == ePiece::none)
          {
            if (!WouldBeCheckedAfterMove(rank, file, static_cast<eRank>(_UINT8(rank) + (forWhite ? 2 : -2)),
              static_cast<eFile>(file), forWhite))
            {
              boolmat[_INT64(rank) + (forWhite ? 2 : -2)][_UINT8(file)] = true;
            }
          }
        }
      }
      // check if pawn can capture a piece
      if (file >= eFile::B)
      {
        auto leftPiece = GetPieceType(static_cast<eRank>(_UINT8(rank) + (forWhite ? 1 : -1)), static_cast<eFile>(_UINT8(file) - 1));
        if ((leftPiece.first != ePiece::none && leftPiece.second != forWhite) ||
             // check also if en passant capture is possible
            (m_enPassantPos.has_value() &&
             m_enPassantPos->first == static_cast<eRank>(_UINT8(rank )+ (forWhite ? 1 : -1)) &&
             m_enPassantPos->second == static_cast<eFile>(_UINT8(file) - 1)))
        {
          if (!WouldBeCheckedAfterMove(rank, file, static_cast<eRank>(_UINT8(rank) + (forWhite ? 1 : -1)),
            static_cast<eFile>(_UINT8(file) - 1), forWhite))
          {
            boolmat[_INT64(rank) + (forWhite ? 1 : -1)][_UINT8(file) - 1] = true;
          }
        }
      }
      if (file <= eFile::G)
      {
        auto rightPiece = GetPieceType(static_cast<eRank>(_UINT8(rank) + (forWhite ? 1 : -1)), static_cast<eFile>(_UINT8(file) + 1));
        if ((rightPiece.first != ePiece::none && rightPiece.second != forWhite) ||
             // check also if en passant capture is possible
            (m_enPassantPos.has_value() &&
             m_enPassantPos->first == static_cast<eRank>(_UINT8(rank) + (forWhite ? 1 : -1)) &&
             m_enPassantPos->second == static_cast<eFile>(_UINT8(file) + 1)))
        {
          if (!WouldBeCheckedAfterMove(rank, file, static_cast<eRank>(_UINT8(rank) + (forWhite ? 1 : -1)),
            static_cast<eFile>(_UINT8(file) + 1), forWhite))
          {
            boolmat[_INT64(rank) + (forWhite ? 1 : -1)][_INT64(file) + 1] = true;
          }
        }
      }
    }
    // check if castling is possible
    if (piece->GetType() == ePiece::king)
    {
      if (CanCastle(forWhite, true))
      {
        boolmat[forWhite ? _INT64(eRank::_1) : _INT64(eRank::_8)][_INT64(eFile::C)] = true;
      }
      if (CanCastle(forWhite, false))
      {
        boolmat[forWhite ? _INT64(eRank::_1) : _INT64(eRank::_8)][_INT64(eFile::G)] = true;
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
      m_logger->Error("There are no records: IsChecked() not possible.", __FILE__, __LINE__);
      return false;
    }
    const auto& boardView = m_record.back();

    // assert that kings are actually at correct (saved) position
    DEBUG_ASSERT(boardView[_UINT8(kingPos.first)][_UINT8(kingPos.second)].first == ePiece::king &&
                 boardView[_UINT8(kingPos.first)][_UINT8(kingPos.second)].second == forWhite);
    DEBUG_ASSERT(boardView[_UINT8(oppKingPos.first)][_UINT8(oppKingPos.second)].first == ePiece::king &&
                 boardView[_UINT8(oppKingPos.first)][_UINT8(oppKingPos.second)].second != forWhite);

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
      if (kingPos.first < eRank::_7)
      {
        if (kingPos.second >= eFile::B &&
            boardView[_INT64(kingPos.first) + 1][_UINT8(kingPos.second) - 1].second != forWhite &&
            boardView[_INT64(kingPos.first) + 1][_UINT8(kingPos.second) - 1].first == ePiece::pawn)
        {
          return true;
        }
        if (kingPos.second <= eFile::G &&
          boardView[_INT64(kingPos.first) + 1][_INT64(kingPos.second) + 1].second != forWhite &&
          boardView[_INT64(kingPos.first) + 1][_INT64(kingPos.second) + 1].first == ePiece::pawn)
        {
          return true;
        }
      }
    }
    
    // check if king is checked by other king (in the game not possible, but validation
    // necessary in order to get the next valid moves of the king
    if (std::abs((int)kingPos.first - (int)oppKingPos.first) <= 1 &&
        std::abs((int)kingPos.second - (int)oppKingPos.second) <= 1)
    {
      return true;
    }
    return false;
  }

  bool CChessBoard::Move(eRank fromRank, eFile fromFile, eRank toRank, eFile toFile, bool forWhite)
  {
    boolmat_t validMoves = GetValidMoves(fromRank, fromFile, forWhite);

    if (!validMoves[_UINT8(toRank)][_UINT8(toFile)])
    {
      return false;
    }
    m_removed.push_back(std::move(m_board[_UINT8(toRank)][_UINT8(toFile)]));
    m_board[_UINT8(toRank)][_UINT8(toFile)] = std::move(m_board[_UINT8(fromRank)][_UINT8(fromFile)]);

    // if king was moved two files, it was castling and also the rook has to be moved
    if (m_board[_UINT8(toRank)][_UINT8(toFile)] && m_board[_UINT8(toRank)][_UINT8(toFile)]->GetType() == ePiece::king)
    {
      if (fromFile == eFile::E && toFile == eFile::C)
      {
        m_board[_UINT8(toRank)][_UINT8(eFile::D)] = std::move(m_board[_UINT8(toRank)][_UINT8(eFile::A)]);
      }
      else if (fromFile == eFile::E && toFile == eFile::G)
      {
        m_board[_UINT8(toRank)][_UINT8(eFile::F)] = std::move(m_board[_UINT8(toRank)][_UINT8(eFile::H)]);
      }
      else
      {
        auto& kingMoved = (forWhite ? m_whiteKingMoved : m_blackKingMoved);
        kingMoved = true;
      }
    }
    // if rook was moved from initial position, set corresponding flag for castling
    if (m_board[_UINT8(toRank)][_UINT8(toFile)] && m_board[_UINT8(toRank)][_UINT8(toFile)]->GetType() == ePiece::rook)
    {
      if (fromRank == eRank::_1 && fromFile == eFile::A)
      {
        m_whiteRookAtQueenSideMoved = true;
      }
      else if (fromRank == eRank::_1 && fromFile == eFile::H)
      {
        m_whiteRookAtKingSideMoved = true;
      }
      else if (fromRank == eRank::_8 && fromFile == eFile::A)
      {
        m_blackRookAtQueenSideMoved = true;
      }
      else if (fromRank == eRank::_8 && fromFile == eFile::H)
      {
        m_blackRookAtKingSideMoved = true;
      }
    }
    // if pawn was moved, reset corresponding counter
    else if (m_board[_UINT8(toRank)][_UINT8(toFile)] && m_board[_UINT8(toRank)][_UINT8(toFile)]->GetType() == ePiece::pawn)
    {
      m_turnsWithoutPawn = 0;
    }
    else
    {
      m_turnsWithoutPawn++;
    }
    // if king was moved, update his position
    if (m_board[_UINT8(toRank)][_UINT8(toFile)] && m_board[_UINT8(toRank)][_UINT8(toFile)]->GetType() == ePiece::king)
    {
      auto& refKingPos = forWhite ? m_whiteKingPos : m_blackKingPos;
      refKingPos = std::pair(toRank, toFile);
    }
    // check if pawn captured en passant
    else if (m_board[_UINT8(toRank)][_UINT8(toFile)] && m_board[_UINT8(toRank)][_UINT8(toFile)]->GetType() == ePiece::pawn &&
      m_enPassantPos.has_value() &&
      m_enPassantPos->first == toRank && m_enPassantPos->second == toFile)
    {
      // delete pawn which was captured en passant (and assert that it actually is a pawn)
      eRank rankOfPawnToCapture = static_cast<eRank>(_UINT8(toRank) + (forWhite ? -1 : 1));
      DEBUG_ASSERT(m_board[_UINT8(rankOfPawnToCapture)][_UINT8(toFile)] &&
                   m_board[_UINT8(rankOfPawnToCapture)][_UINT8(toFile)]->GetType() == ePiece::pawn &&
                   m_board[_UINT8(rankOfPawnToCapture)][_UINT8(toFile)]->IsWhite() != forWhite);
      m_removed.back() = std::move(m_board[_UINT8(rankOfPawnToCapture)][_UINT8(toFile)]);
    }

    // if pawn did double step, set en passant capture position
    if (m_board[_UINT8(toRank)][_UINT8(toFile)] && m_board[_UINT8(toRank)][_UINT8(toFile)]->GetType() == ePiece::pawn &&
        std::abs((int)fromRank - (int)toRank) == 2)
    {
      // set en passant capture position: one rank below (for white) or above (for black) the current pawn position
      m_enPassantPos = std::make_optional<std::pair<eRank, eFile>>();
      m_enPassantPos->first = static_cast<eRank>(_UINT8(toRank) + (forWhite ? -1 : 1));
      m_enPassantPos->second = toFile;
    }
    else
    {
      m_enPassantPos.reset();
    }
    CreateNextRecord();
    return true;
  }

  bool CChessBoard::CanCastle(bool forWhite, bool forQueenSide) const
  {
    bool kingMoved = forWhite ? m_whiteKingMoved : m_blackKingMoved;
    bool rookMoved = forWhite ? 
      (forQueenSide ? m_whiteRookAtQueenSideMoved : m_whiteRookAtKingSideMoved) :
      (forQueenSide ? m_blackRookAtQueenSideMoved : m_blackRookAtKingSideMoved);
    if (kingMoved || rookMoved)
    {
      return false;
    }

    eRank rank = forWhite ? eRank::_1 : eRank::_8;
    eFile fromFile = eFile::E;
    eFile toFile1 = forQueenSide ? eFile::C : eFile::F;
    eFile toFile2 = forQueenSide ? eFile::D : eFile::G;
    
    if (m_board[_UINT8(rank)][_UINT8(toFile1)] != nullptr ||
        m_board[_UINT8(rank)][_UINT8(toFile2)] != nullptr)
    {
      return false;
    }
    if (IsChecked(forWhite) ||
        WouldBeCheckedAfterMove(rank, fromFile, rank, toFile1, forWhite) ||
        WouldBeCheckedAfterMove(rank, fromFile, rank, toFile2, forWhite))
    {
      return false;
    }
    return true;
  }

  eState CChessBoard::CheckmateState(bool forWhite) const
  {
    /// count number of valid moves for all pieces for black or white;
    /// if number is zero, player is either checkmate or it's a stalemate
    
    std::size_t countValidMoves = 0;
    for (uint8_t rank = 0; rank < RANKS; rank++)
    {
      for (uint8_t file = 0; file < FILES; file++)
      {
        auto boolMat = GetValidMoves(static_cast<eRank>(rank), 
                                     static_cast<eFile>(file), forWhite);
        countValidMoves += CountBoolMat(boolMat);
      }
    }

    bool inCheck = IsChecked(forWhite);
    if (inCheck)
    {
      if (countValidMoves == 0)
      {
        return eState::eCheckmate;
      }
      else
      {
        return eState::eInCheck;
      }
    }
    else
    {
      if (countValidMoves == 0)
      {
        return eState::eStalemate;
      }
      else
      {
        return eState::eNone;
      }
    }
  }

  bool CChessBoard::ThreefoldRepetition()
  {
    // look up the last moves until a pawn was moved,
    // because with pawn move a repetition is not possible
    if (m_turnsWithoutPawn < 12)
    {
      // at least 12 moves (turns) are necessary that threefold repetition can happen
      return false;
    }
    int repetitions = 0;
    const auto& currentView = m_record.back();
    for (std::size_t ind = 1; ind <= m_turnsWithoutPawn; ind++) 
    {
      if (m_record[m_record.size() - 1 - ind] == currentView)
      {
        repetitions++;
        if (repetitions == 3)
        {
          return true;
        }
      }
    }
    return false;
  }

  bool CChessBoard::DueFiftyMovesRule()
  {
    return m_turnsWithoutPawn >= 50;
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
    return std::pair(eRank::_1, eFile::A);
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

    m_whiteKingPos = std::pair(eRank::_1, eFile::E);
    m_blackKingPos = std::pair(eRank::_8, eFile::E);
    m_enPassantPos.reset();

    m_removed.clear();
    m_record.clear();
    CreateNextRecord();
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
      newRank = _UINT8(kingPos.first);
      newFile = _UINT8(kingPos.second);

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

  bool CChessBoard::WouldBeCheckedAfterMove(eRank fromRank, eFile fromFile, 
    eRank toRank, eFile toFile, bool forWhite) const
  {
    bool wouldBeChecked;

    // change current board view
    piece_t pieceCopy = m_record.back()[_UINT8(toRank)][_UINT8(toFile)];
    m_record.back()[_UINT8(toRank)][_UINT8(toFile)] = m_record.back()[_UINT8(fromRank)][_UINT8(fromFile)];
    m_record.back()[_UINT8(fromRank)][_UINT8(fromFile)] = std::pair(ePiece::none, true);

    if (m_record.back()[_UINT8(toRank)][_UINT8(toFile)].first == ePiece::king)
    {
      auto& refKingPos = forWhite ? m_whiteKingPos : m_blackKingPos;
      refKingPos.first = toRank;
      refKingPos.second = toFile;
      wouldBeChecked = IsChecked(forWhite);
      refKingPos.first = fromRank;
      refKingPos.second = fromFile;
    }
    else
    {
      wouldBeChecked = IsChecked(forWhite);
    }
    // undo changed board view
    m_record.back()[_UINT8(fromRank)][_UINT8(fromFile)] = m_record.back()[_UINT8(toRank)][_UINT8(toFile)];
    m_record.back()[_UINT8(toRank)][_UINT8(toFile)] = pieceCopy;
    
    return wouldBeChecked;
  }

  std::size_t CChessBoard::CountBoolMat(const boolmat_t& boolMat) const
  {
    std::size_t count = 0;
    for (const auto& vec : boolMat)
    {
      count += std::count(vec.begin(), vec.end(), true);
    }
    return count;
  }

}
