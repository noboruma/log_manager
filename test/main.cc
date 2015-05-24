#include "../log.hh"

int main()
{
  global::log::get_verbosity() = true;

  auto it = global::log::attach_log("toto.log");

  //global::log::trace<global::log::level::DBG>("main", "start");
  TRACE(DBG, "start");
  TRACE(NFO, "1");
  TRACE(NFO, "2");
  TRACE(NFO, "3");
  TRACE(NFO, "4");
  TRACE(ERR, "5");
  TRACE(WRN, "6");
  TRACE(VRB, "end");

  global::log::detach_log(it);

  LOG(NFO,"main panel", "detached");

  global::log::clean();

  return 0;
}
