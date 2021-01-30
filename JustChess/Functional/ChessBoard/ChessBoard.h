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

    bool Move(eRank fromRank, eFile fromFile, eRank toRank, eFile toFile, bool forWhite);

    //intmat_t GetFieldsAttacked(bool byWhite);
    //bool CanCastle(bool forWhite);
    //void PromotePawn(eRank rank, eFile file, ePiece pieceType);
    //
    //bool Checkmate();
    //bool MaterialInsufficient();
    //bool ThreefoldRepitition();
    //bool DueFiftyMoveRule();
    //bool Stalemate();

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

    /// @brief Save white king position for faster access
    mutable std::pair<eRank, eFile> m_whiteKingPos;
    /// @brief Save black king position for faster access
    mutable std::pair<eRank, eFile> m_blackKingPos;
    /// @brief Remember position to capture en passant
    std::optional<std::pair<eRank, eFile>> m_enPassantPos;

    /// @brief Returns current position of white or black king
    /// @param boardView 
    /// @param forWhite 
    /// @return position: rank, file
    std::pair<eRank, eFile> FindKing(const view_t& boardView, bool forWhite) const;

    char PieceCharRep(const std::unique_ptr<CChessPiece>& pPiece) const;

    bool IsCheckedPieceDirection(ePiece pieceDir, bool forWhite) const;
    bool WouldBeCheckedAfterMove(eRank fromRank, eFile fromFile, eRank toRank, eFile toFile, bool forWhite) const;

  };
}
