#pragma once

#include "Example.h"

class CAnotherExample : public CExample
{
public:
  CAnotherExample()
  : a(0)
  , b(0)
  , c(0)
  {};

  int a, b, c;

  void DoSomething();
};
