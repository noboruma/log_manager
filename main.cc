#include "log.hh"
#include <iostream>

int main()
{
  global::log::init(true);

  auto it = global::log::attach_log("toto.log");

  global::log::trace<global::log::level::DBG>("main", "start");
  global::log::trace<global::log::level::VRB>("main", "end");

  global::log::clean();

  return 0;
}
