namespace global 
{

  std::ostream& operator<<(std::ostream& o, log::level l)
  {
    switch(l)
    {
      case log::level::DBG:
        o<<" (DBG) ";
        break;
      case log::level::ERR:
        o<<" (ERR) ";
        break;
      case log::level::WRN:
        o<<" (WRN) ";
        break;
      case log::level::NFO:
        o<<" (NFO) ";
        break;
      case log::level::VRB:
        o<<" (VRB) ";
        break;
    }
    return o;
  }

  template<log::level l>
  inline void log::trace(const std::string &who, const std::string &msg)
  {

    std::stringstream ss;
    ss <<"["<<who<<"] "<<get_now()<< l <<msg;
    std::cout << ss.str() << std::endl;
    for(auto& log : logs)
      (*log)<< ss.str() <<std::endl;
  }

  template<>
  inline void log::trace<log::level::ERR>(const std::string &who,
                                          const std::string &msg)
  {
    std::stringstream ss;
    ss <<"["<<who<<"] "<<log::get_now()<< log::level::ERR <<msg;
    std::cerr<< ss <<std::endl;;
    for(auto& log : logs)
      (*log)<< ss <<std::endl;;
  }

  template<>
  inline void log::trace<log::level::VRB>(const std::string &who,
                                          const std::string &msg)
  {
    if(!verbose)
      return;

    std::stringstream ss;
    ss << "["<<who<<"] "<<log::get_now()<< log::level::VRB <<msg;
    std::cout << ss.str() << std::endl;
    for(auto& log : logs)
      (*log)<< ss.str() << std::endl;
  }

  template<log::level l>
  inline void log::thread_safe_trace(const std::string &who, const std::string &msg)
  {
    static std::mutex mut;
    {
      std::lock_guard<std::mutex> lock(mut);
      log::trace<l>(who,msg);
    }
  }

} //!global
