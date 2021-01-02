#include <stdafx.h>

#include "ChessPiece.h"

namespace JC
{
  void CChessPiece::IncrementMoveCount()
  {
    m_moveCount++;
  }

  const vecPairRankFile_t& CChessPiece::GetMoveDirs() const
  {
    return s_moveDirMap.at(m_type);
  }

  bool CChessPiece::CanMoveMultipleSteps() const
  {
    return s_moveMultipleStepsMap.at(m_type);
  }

  const JC::vecPairRankFile_t& CChessPiece::GetStartPositions(ePiece type, bool isWhite)
  {
    return s_startPosMap.at(std::pair(type, isWhite));
  }

}
