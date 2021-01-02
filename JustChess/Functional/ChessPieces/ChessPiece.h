#pragma once

namespace JC
{
  /// @brief Base class for all chess pieces
  class CChessPiece
  {
  public:
    CChessPiece(ePiece type, bool isWhite)
      : m_type(type)
      , m_isWhite(isWhite)
      , m_moveCount(0)
    {
    };
    virtual ~CChessPiece() = default;

    /// @brief
    /// @return @c true if chess piece is white
    bool IsWhite() const { return m_isWhite; }
    /// @brief 
    /// @return type of chess piece 
    ePiece GetType() const { return m_type; }
    /// @brief 
    /// @return number of moves  
    uint16_t GetMoveCount() const { return m_moveCount; }
    /// @brief 
    void IncrementMoveCount();
    /// @brief 
    /// @return all possible directions this piece can move
    const vecPairRankFile_t& GetMoveDirs() const;
    /// @brief 
    /// <returns> @c true if chess piece can make multiple steps (in one direction) 
    bool CanMoveMultipleSteps() const;
    /// @brief Static method for start positions of certain type of chess piece
    /// @param type Type of chess piece
    /// @param isWhite For  white (@c true) or black (@c false) piece
    /// @return start positions: rank, file
    static const vecPairRankFile_t& GetStartPositions(ePiece type, bool isWhite);

  protected:
    /// @brief Type of chess piece
    ePiece m_type;
    /// @brief Flag if chess piece is white or black
    bool m_isWhite;
    /// @brief Number of moves done with this chess piece
    uint16_t m_moveCount;

  private:
  };
}
