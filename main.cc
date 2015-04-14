#include "log.hh"

int main()
{
  global::log::init(true);

  auto it = global::log::attach_log("toto.log");

  //global::log::trace<global::log::level::DBG>("main", "start");
  TRACE(DBG, "start");
  TRACE(VRB, "end");

  global::log::detach_log(it);

  LOG(NFO,"main panel", "detached");

  global::log::clean();

  return 0;
}
