#ifndef GLOBAL_LOG_HH
#define GLOBAL_LOG_HH

#include <iostream>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>
#include <stdexcept>
#include <fstream>
#include <list>
#include <fstream>

#define LOG(type, msg) global::log::trace<global::log::level::type>(__FUNCTION__, msg);

namespace global 
{
  namespace log
  {
    enum class level
    {
      DBG,
      ERR,
      WRN,
      NFO,
      VRB
    };

    static std::list<std::ofstream*> logs;
    static bool verbose = false;

    std::ostream& operator<<(std::ostream& o, log::level l)
    {
      switch(l)
      {
        case level::DBG:
          o<<" (DBG) ";
          break;
        case level::ERR:
          o<<" (ERR) ";
          break;
        case level::WRN:
          o<<" (WRN) ";
          break;
        case level::NFO:
          o<<" (NFO) ";
          break;
        case level::VRB:
          o<<" (VRB) ";
          break;
      }
      return o;
    }

    std::string get_now()
    {
      auto now = std::chrono::system_clock::now();
      auto in_time_t = std::chrono::system_clock::to_time_t(now);
      auto duration = now.time_since_epoch();
      auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;

      std::stringstream ss;
      char format_date [100];
      std::strftime(format_date,
                    sizeof(format_date),
                    "%Y-%m-%d %H:%M:%S",
                    std::localtime(&in_time_t)); 

      ss<<format_date<<":"<<millis;

      return ss.str();
    }

    void init(bool v = false)
    { 
      verbose = v;
    }

    std::list<std::ofstream*>::iterator attach_log(const std::string &s)
    {
      logs.push_back(new std::ofstream(s));
      auto last_it = logs.end();
      --last_it;
      return last_it;
    }

    void detach_log(std::list<std::ofstream*>::iterator it)
    {
      delete *it;
      logs.erase(it);
    }

    void clean()
    {
      for(std::list<std::ofstream*>::iterator it = logs.begin();
          it != logs.end();
          ++it)
        delete *it;
       logs.clear();
    }

    template<log::level l>
    inline void trace(const std::string &who, const std::string &msg)
    {

      std::stringstream ss;
      ss <<"["<<who<<"] "<<get_now()<< l <<msg;
      std::cout << ss.str() << std::endl;
      for(auto& log : logs)
        (*log)<< ss.str() <<std::endl;
    }

    template<>
    inline void trace<level::ERR>(const std::string &who, const std::string &msg)
    {
      std::stringstream ss;
      ss <<"["<<who<<"] "<<get_now()<< level::ERR <<msg;
      std::cerr<< ss <<std::endl;;
      for(auto& log : logs)
        (*log)<< ss <<std::endl;;
    }

    template<>
    inline void trace<level::VRB>(const std::string &who, const std::string &msg)
    {
      if(!verbose)
        return;

      std::stringstream ss;
      ss << "["<<who<<"] "<<get_now()<< level::VRB <<msg;
      std::cout << ss.str() << std::endl;
      for(auto& log : logs)
        (*log)<< ss.str() << std::endl;
    }
    
#ifdef NDEBUG
    template<>
    inline void trace<log::level::DBG>(const std::string &who,
                                       const std::string &msg)
    {}
#endif

  }//! log
} //!global

#endif

