﻿#include <stdafx.h>

#include "Logger\StandardOutputLogger.h"
#include "Functional\ChessBoard\ChessBoard.h"
#include "Functional\ChessPieces\ChessPiece.h"

void CheckValidMoves(JC::CChessBoard& board);
void Play(JC::CChessBoard& board);
bool CharToChessRank(char rankChar, JC::eRank& rank);
bool CharToChessFile(char fileChar, JC::eFile& file);

int main()
{
  Logger logger = std::make_shared<CStandardOutputLogger>();
  logger->Info("Start JustChess");
  JC::CChessBoard board(logger);

  Play(board);
  //CheckValidMoves(board);

  logger->Info("End JustChess");
}

void Play(JC::CChessBoard& board)
{
  bool whiteToMove = true;
  int turnCount = 0;
  std::string moveStr;
  JC::eFile fromFile;
  JC::eRank fromRank;
  JC::eFile toFile;
  JC::eRank toRank;

  board.Reset();
  while (true)
  {
    std::cout << std::endl;
    board.PrintRecord(-1);
    std::cout << std::endl;

    if (board.ThreefoldRepetition())
    {
      std::cout << "Threefold repetition." << std::endl;
      break;
    }

    switch (board.CheckmateState(whiteToMove))
    {
    case JC::eState::eNone:
      std::cout << "" << std::endl;
      break;
    case JC::eState::eInCheck:
      std::cout << "Check!" << std::endl;
      break;
    case JC::eState::eCheckmate:
      std::cout << "Checkmate!" << std::endl;
      break;
    case JC::eState::eStalemate:
      std::cout << "Stalemate!" << std::endl;
      break;
    default:
      break;
    }
    
    std::cout << (turnCount/2 + 1) << ". move of " <<
      (whiteToMove ? "white: " : "black: ") << std::flush;
    std::cin >> moveStr;
    //std::getline(std::cin, moveStr); // example: a2a4

    // remove white space
    moveStr.erase(std::remove_if(moveStr.begin(), moveStr.end(), ::isspace), moveStr.end());

    if (moveStr == "exit")
    {
      std::cout << "Exit." << std::endl;
      break;
    }
    if (moveStr == "reset")
    {
      std::cout << "Reset." << std::endl;
      whiteToMove = true;
      turnCount = 0;
      board.Reset();
      continue;
    }

    if (moveStr.length() != 4)
    {
      std::cout << "Invalid input." << std::endl;
      continue;
    }
    
    if (!CharToChessFile(moveStr[0], fromFile) ||
        !CharToChessRank(moveStr[1], fromRank) ||
        !CharToChessFile(moveStr[2], toFile) ||
        !CharToChessRank(moveStr[3], toRank))
    {
      std::cout << "Invalid input." << std::endl;
      continue;
    }
    if (!board.Move(fromRank, fromFile, toRank, toFile, whiteToMove))
    {
      std::cout << "Not a valid move." << std::endl;
      continue;
    }
    turnCount++;
    whiteToMove = !whiteToMove;
  }
}


bool CharToChessRank(char rankChar, JC::eRank& rank)
{
  switch (rankChar)
  {
  case '1': rank = JC::eRank::_1; break;
  case '2': rank = JC::eRank::_2; break;
  case '3': rank = JC::eRank::_3; break;
  case '4': rank = JC::eRank::_4; break;
  case '5': rank = JC::eRank::_5; break;
  case '6': rank = JC::eRank::_6; break;
  case '7': rank = JC::eRank::_7; break;
  case '8': rank = JC::eRank::_8; break;
  default:
    return false;
  }
  return true;
}

bool CharToChessFile(char fileChar, JC::eFile& file)
{
  switch (fileChar)
  {
  case 'A': file = JC::eFile::A; break;
  case 'a': file = JC::eFile::A; break;
  case 'B': file = JC::eFile::B; break;
  case 'b': file = JC::eFile::B; break;
  case 'C': file = JC::eFile::C; break;
  case 'c': file = JC::eFile::C; break;
  case 'D': file = JC::eFile::D; break;
  case 'd': file = JC::eFile::D; break;
  case 'E': file = JC::eFile::E; break;
  case 'e': file = JC::eFile::E; break;
  case 'F': file = JC::eFile::F; break;
  case 'f': file = JC::eFile::F; break;
  case 'G': file = JC::eFile::G; break;
  case 'g': file = JC::eFile::G; break;
  case 'H': file = JC::eFile::H; break;
  case 'h': file = JC::eFile::H; break;
  default:
    return false;
  }
  return true;
}


void CheckValidMoves(JC::CChessBoard& board)
{
  std::string moveStr;
  JC::eFile file;
  JC::eRank rank;
  bool forWhite;

  while (true)
  {
    std::cout << "\nCheck valid moves (enter \'e\' to exit)" << std::endl;
    board.PrintCurrentBoard();

    std::cout << "\nEnter file, rank and color (examples: D4w, a4b): " << std::flush;
    std::cin >> moveStr;

    // remove white space
    moveStr.erase(std::remove_if(moveStr.begin(), moveStr.end(), ::isspace), moveStr.end());
    
    if (moveStr.length() == 1 && moveStr[0] == 'e')
    {
      std::cout << "Exit." << std::endl;
      break;
    }

    if (moveStr.length() != 3)
    {
      std::cout << "Invalid input." << std::endl;
      continue;
    }

    switch (moveStr[0])
    {
    case 'A': file = JC::eFile::A; break;
    case 'a': file = JC::eFile::A; break;
    case 'B': file = JC::eFile::B; break;
    case 'b': file = JC::eFile::B; break;
    case 'C': file = JC::eFile::C; break;
    case 'c': file = JC::eFile::C; break;
    case 'D': file = JC::eFile::D; break;
    case 'd': file = JC::eFile::D; break;
    case 'E': file = JC::eFile::E; break;
    case 'e': file = JC::eFile::E; break;
    case 'F': file = JC::eFile::F; break;
    case 'f': file = JC::eFile::F; break;
    case 'G': file = JC::eFile::G; break;
    case 'g': file = JC::eFile::G; break;
    case 'H': file = JC::eFile::H; break;
    case 'h': file = JC::eFile::H; break;
    default:
      std::cout << "Invalid input." << std::endl;
      continue;
      break;
    }

    switch (moveStr[1])
    {
    case '1': rank = JC::eRank::_1; break;
    case '2': rank = JC::eRank::_2; break;
    case '3': rank = JC::eRank::_3; break;
    case '4': rank = JC::eRank::_4; break;
    case '5': rank = JC::eRank::_5; break;
    case '6': rank = JC::eRank::_6; break;
    case '7': rank = JC::eRank::_7; break;
    case '8': rank = JC::eRank::_8; break;
    default:
      std::cout << "Invalid input." << std::endl;
      continue;
      break;
    }

    switch (moveStr[2])
    {
    case 'w': forWhite = true; break;
    case 'W': forWhite = true; break;
    case 'b': forWhite = false; break;
    case 'B': forWhite = false; break;
    default:
      std::cout << "Invalid input." << std::endl;
      continue;
      break;
    }

    std::cout << std::endl;
    board.PrintBoolMat(board.GetValidMoves(rank, file, forWhite));
  }
}

