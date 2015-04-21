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
    const std::string& s = ss.str();
    std::cout << s << std::endl;
    for(auto& log : get_logs())
      (*log)<< s <<std::endl;
  }

  template<>
  inline void log::trace<log::level::ERR>(const std::string &who,
                                          const std::string &msg)
  {
    std::stringstream ss;
    ss <<"["<<who<<"] "<<log::get_now()<< log::level::ERR <<msg;
    const std::string& s = ss.str();
    std::cerr << s << std::endl;
    for(auto& log : get_logs())
      (*log)<< s <<std::endl;
  }

  template<>
  inline void log::trace<log::level::VRB>(const std::string &who,
                                          const std::string &msg)
  {
    if(!get_verbosity())
      return;

    std::stringstream ss;
    ss << "["<<who<<"] "<<log::get_now()<< log::level::VRB <<msg;
    const std::string& s = ss.str();
    std::cout << s << std::endl;
    for(auto& log : get_logs())
      (*log)<< s << std::endl;
  }

  template<log::level l>
  inline void log::thread_safe_trace(const std::string &who, const std::string &msg)
  {
    static std::mutex mut;
    std::lock_guard<std::mutex> lock(mut);
    log::trace<l>(who,msg);
  }

} //!global
