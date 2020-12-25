#pragma once

/** @defgroup group1 The First Group
 *  This is the first group
 */

/**
 * \class CExample
 *
 * \brief Provide an example
 *
 * This class is meant as an example.  It is not useful by itself
 * rather its usefulness is only a function of how much it helps
 * the reader.  It is in a sense defined by the person who reads it
 * and otherwise does not exist in any real form.
 *
 * \note Attempts at zen rarely work.
 *
 * \author (last to touch it) $Author: bv $
 *
 * \version $Revision: 1.5 $
 *
 * \date $Date: 2005/04/14 14:16:20 $
 *
 * Contact: bv@bnl.gov
 *
 * Created on: Wed Apr 13 18:39:37 2005
 *
 * $Id: doxygen-howto.html,v 1.5 2005/04/14 14:16:20 bv Exp $
 *
 */

#ifndef CExample_H
#define CExample_H

class CExample
{
public:

  /// Constructor
  CExample()
    : m_question(nullptr)
    , m_answer()
  {}

  /// Destructor
  ~CExample() = default;

  /// This method does something
  void DoSomething();

  /** This is a method that does so
    * much that I must write an epic
    * novel just to describe how much
    * it truly does. */
  void DoNothing();

  /** \brief A useful method.
    * \param level an integer setting how useful to be
    * \return Output that is extra useful
    *
    * This method does unbelievably useful things.
    * And returns exceptionally useful results.
    * Use it everyday with good health.
    */
  void* VeryUsefulMethod(int level);

  /**
  * This is just another comment of an inline funciton
  */
  void DoReallyNothing(){}

  void DoReallyNothingAgain(){}

  /**
  * @ingroup group1
  * @brief function 1 in group 1
  */
  void func1InGroup1(){}
  /**
  * @ingroup group1
  * @brief function 2 in group 1
  */
  void func2InGroup1(){}

private:
  const char* m_question; ///< the question
  int m_answer;           ///< the answer 

};                              // end of class CExample

#endif  // CExample_H
