#pragma once

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
    {}
    virtual ~CChessBoard() = default;

    using board_t = std::vector<std::vector<std::unique_ptr<CChessPiece>>>;
    using removed_t = std::vector<std::unique_ptr<CChessPiece>>;
    using boolmat_t = std::vector<std::vector<bool>>;
    using intmat_t = std::vector<std::vector<int>>;
    using piece_t = std::pair<ePiece, bool>; /// type and color of chess piece
    using view_t = std::vector<std::vector<piece_t>>;
    using record_t = std::vector<view_t>;

    piece_t GetPieceType(eRank rank, eFile file);
    boolmat_t GetValidMoves(eRank rank, eFile file, bool forWhite);
    //intmat_t GetFieldsAttacked(bool byWhite);
    //
    //bool InCheck(bool forWhite);
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
    record_t m_record;

    char PieceCharRep(const std::unique_ptr<CChessPiece>& pPiece);

  };
}