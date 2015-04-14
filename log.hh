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

#define __LOG_ERR(who, msg) global::log::trace<global::log::level::ERR>(who, msg)
#define __LOG_VRB(who, msg) global::log::trace<global::log::level::VRB>(who, msg)
#define __LOG_NFO(who, msg) global::log::trace<global::log::level::NFO>(who, msg)
#ifdef NDEBUG
  #define __LOG_DBG(who, msg)
#else
  #define __LOG_DBG(who, msg) global::log::trace<global::log::level::DBG>(who, msg)
#endif
#define __LOG_WRN(who, msg) global::log::trace<global::log::level::WRN>(who, msg)

//API
#define TRACE(type, msg) __LOG_##type(__FUNCTION__, msg)
#define LOG(type, who, msg) __LOG_##type(who, msg)

namespace global 
{
  struct log
  {
    enum class level
    {
      DBG,
      ERR,
      WRN,
      NFO,
      VRB
    };

    static std::string get_now()
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

    static void init(bool v = false)
    { 
      verbose = v;
    }

    static std::list<std::ofstream*>::iterator attach_log(const std::string &s)
    {
      logs.push_back(new std::ofstream(s));
      auto last_it = logs.end();
      --last_it;
      return last_it;
    }

    static void detach_log(std::list<std::ofstream*>::iterator it)
    {
      delete *it;
      logs.erase(it);
    }

    static void clean()
    {
      for(std::list<std::ofstream*>::iterator it = logs.begin();
          it != logs.end();
          ++it)
        delete *it;
       logs.clear();
    }

    template<log::level l>
    static inline void trace(const std::string &who, const std::string &msg);


    private:
      static std::list<std::ofstream*> logs;
      static bool verbose;

  };//! log

  std::ostream& operator<<(std::ostream& o, log::level l);

} //!global

#include "log.hxx"

#endif

