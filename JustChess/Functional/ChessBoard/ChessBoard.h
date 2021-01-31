#pragma once

#ifdef _DEBUG
#define DEBUG_ASSERT(x) assert(x)
#else
#define DEBUG_ASSERT(x) /* DEBUG_ASSERT does nothing when not debugging */
#endif

#define RANKS 8
#define FILES 8

#include "Logger/Logger.h"

namespace JC
{
  class CChessPiece;

  class CChessBoard
  {
  public:
    CChessBoard(Logger logger)
      : m_logger(logger)
      , m_whiteKingPos()
      , m_blackKingPos()
      , m_enPassantPos(std::nullopt)
      , m_turnsWithoutPawn(0)
      , m_whiteRookAtQueenSideMoved(false)
      , m_whiteRookAtKingSideMoved(false)
      , m_whiteKingMoved(false)
      , m_blackRookAtQueenSideMoved(false)
      , m_blackRookAtKingSideMoved(false)
      , m_blackKingMoved(false)
    {}
    virtual ~CChessBoard() = default;

    using board_t = std::vector<std::vector<std::unique_ptr<CChessPiece>>>;
    using removed_t = std::vector<std::unique_ptr<CChessPiece>>;
    using boolmat_t = std::vector<std::vector<bool>>;
    using intmat_t = std::vector<std::vector<int>>;
    using piece_t = std::pair<ePiece, bool>; /// type and color of chess piece
    using view_t = std::vector<std::vector<piece_t>>;
    using record_t = std::vector<view_t>;

    piece_t GetPieceType(eRank rank, eFile file) const;
    boolmat_t GetValidMoves(eRank rank, eFile file, bool forWhite) const;

    /// @brief Check if white or black is checked. 
    /// Makes use of last board view in records.
    /// @param forWhite
    /// @return @c true if specified color is checked.
    bool IsChecked(bool forWhite) const;

    /// @brief Move a chess piece from a rank and file to another rank and file. 
    /// @param fromRank 
    /// @param fromFile 
    /// @param toRank 
    /// @param toFile 
    /// @param forWhite 
    /// @return @c true if the move is possible.
    bool Move(eRank fromRank, eFile fromFile, eRank toRank, eFile toFile, bool forWhite);

    /// @brief Check if castling is possible for either black or white.
    /// @param forWhite for white or black
    /// @param queenSide for queen side or king side
    /// @return @c true if castling is possible.
    bool CanCastle(bool forWhite, bool forQueenSide) const;

    //intmat_t GetFieldsAttacked(bool byWhite);
    //void PromotePawn(eRank rank, eFile file, ePiece pieceType);
    
    /// @brief State (checkmate or stalemate)
    eState CheckmateState(bool forWhite) const;
    
    //bool MaterialInsufficient();
    bool ThreefoldRepetition();
    bool DueFiftyMovesRule();

    /// @brief Reset the chess board.
    void Reset();
    void PrintCurrentBoard();
    void PrintBoolMat(boolmat_t boolmat);

    /// @brief Prints a chess board record to the console. 
    /// @param ind Index of record to print. Pass @c -1 to print last record.
    void PrintRecord(int ind);
    void CreateNextRecord();

    static std::unique_ptr<CChessPiece> CreatePiece(ePiece type, bool isWhite);

  private:
    Logger m_logger;
    board_t m_board;
    removed_t m_removed;
    /// @brief Record of all board views.
    mutable record_t m_record;
    mutable std::size_t m_turnsWithoutPawn;

    /// @brief Save white king position for faster access
    mutable std::pair<eRank, eFile> m_whiteKingPos;
    /// @brief Save black king position for faster access
    mutable std::pair<eRank, eFile> m_blackKingPos;
    /// @brief Remember position to capture en passant
    std::optional<std::pair<eRank, eFile>> m_enPassantPos;

    bool m_whiteRookAtQueenSideMoved;
    bool m_whiteRookAtKingSideMoved;
    bool m_whiteKingMoved;

    bool m_blackRookAtQueenSideMoved;
    bool m_blackRookAtKingSideMoved;
    bool m_blackKingMoved;

    /// @brief Returns current position of white or black king
    /// @param boardView 
    /// @param forWhite 
    /// @return position: rank, file
    std::pair<eRank, eFile> FindKing(const view_t& boardView, bool forWhite) const;

    /// @brief Counts the number of 'true' elements in a boolean matrix
    /// @param boolMat vector of vector of booleans
    /// @return count of true elements
    std::size_t CountBoolMat(const boolmat_t& boolMat) const;

    char PieceCharRep(const std::unique_ptr<CChessPiece>& pPiece) const;
    bool IsCheckedPieceDirection(ePiece pieceDir, bool forWhite) const;
    bool WouldBeCheckedAfterMove(eRank fromRank, eFile fromFile, eRank toRank, eFile toFile, bool forWhite) const;

  };
}
