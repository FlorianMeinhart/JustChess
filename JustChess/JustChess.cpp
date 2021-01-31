#include <stdafx.h>

#include "Logger\StandardOutputLogger.h"
#include "Functional\ChessBoard\ChessBoard.h"
#include "Functional\ChessPieces\ChessPiece.h"

#define INIT_TIME \
    std::chrono::time_point<std::chrono::system_clock> startTime;
#define START_TIMER \
    startTime = std::chrono::system_clock::now()
#define STOP_TIMER \
    std::chrono::duration_cast<std::chrono::microseconds>( \
    std::chrono::system_clock::now() - startTime).count()
    

void CheckValidMoves(JC::CChessBoard& board);
void Play(JC::CChessBoard& board);
bool CharToChessRank(char rankChar, JC::eRank& rank);
bool CharToChessFile(char fileChar, JC::eFile& file);
std::string TimeText(std::string text, long long time_ms);
void PrintTime(std::string text, long long time_ms);


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

  INIT_TIME;

  board.Reset();
  while (true)
  {
    std::cout << std::endl;
    board.PrintRecord(-1);
    std::cout << std::endl;

    START_TIMER;
    if (board.ThreefoldRepetition())
    {
      std::cout << "Threefold repetition." << std::endl;
      PrintTime("ThreefoldRepetition (true): %t ms", STOP_TIMER);
      break;
    }
    PrintTime("ThreefoldRepetition (false): %t ms", STOP_TIMER);

    START_TIMER;
    switch (board.CheckmateState(whiteToMove))
    {
    case JC::eState::eNone:
      PrintTime("CheckmateState (eNone): %t ms", STOP_TIMER);
      std::cout << "" << std::endl;
      break;
    case JC::eState::eInCheck:
      PrintTime("CheckmateState (eInCheck): %t ms", STOP_TIMER);
      std::cout << "Check!" << std::endl;
      break;
    case JC::eState::eCheckmate:
      PrintTime("CheckmateState (eCheckmate): %t ms", STOP_TIMER);
      std::cout << "Checkmate!" << std::endl;
      break;
    case JC::eState::eStalemate:
      PrintTime("CheckmateState (eStalemate): %t ms", STOP_TIMER);
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

    START_TIMER;
    if (!board.Move(fromRank, fromFile, toRank, toFile, whiteToMove))
    {
      PrintTime("Move (false): %t ms", STOP_TIMER);
      std::cout << "Not a valid move." << std::endl;
      continue;
    }
    PrintTime("Move (true): %t ms", STOP_TIMER);
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

std::string TimeText(std::string text, long long time_micro_s)
{
  std::string replace = "%t";
  std::size_t index;
  std::string time_micro_s_str = std::to_string((double)time_micro_s / 1000);
  if ((index = text.find(replace)) != std::string::npos)
  {
    text.replace(index, replace.length(), time_micro_s_str);
  }
  return text;
}

void PrintTime(std::string text, long long time_micro_s)
{
  std::cout << "\t" << TimeText(text, time_micro_s) << std::endl;
}

