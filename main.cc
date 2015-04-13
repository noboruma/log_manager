#include "log.hh"
#include <iostream>

int main()
{
  global::log<int>::print("main", "start");
  global::log<int>::print("main", "end");

  return 0;
}
