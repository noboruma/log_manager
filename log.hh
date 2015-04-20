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
#include <mutex>

// Do not use directly those defines
#define __LOG_ERR(who, msg, func) global::log::func<global::log::level::ERR>(who, msg)
#define __LOG_VRB(who, msg, func) global::log::func<global::log::level::VRB>(who, msg)
#define __LOG_NFO(who, msg, func) global::log::func<global::log::level::NFO>(who, msg)
#ifdef NDEBUG
  #define __LOG_DBG(who, msg, func)
#else
  #define __LOG_DBG(who, msg, func) global::log::func<global::log::level::DBG>(who, msg)
#endif
#define __LOG_WRN(who, msg, func) global::log::func<global::log::level::WRN>(who, msg)

//API
#define TRACE(type, msg) __LOG_##type(__FUNCTION__, msg, trace)
#define LOG(type, who, msg) __LOG_##type(who, msg, trace)

#define SYNC_TRACE(type, msg) __SYNC_LOG_##type(__FUNCTION__, msg, thread_safe_trace)
#define SYNC_LOG(type, who, msg) __SYNC_LOG_##type(who, msg, thread_safe_trace)

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

      ss<<format_date<<":"<<std::setfill('0')<<std::setw(3)<<millis;

      return ss.str();
    }

    static std::list<std::ofstream*>::iterator attach_log(const std::string &s)
    {
      get_logs().push_back(new std::ofstream(s));
      auto last_it = get_logs().end();
      --last_it;
      return last_it;
    }

    static void detach_log(std::list<std::ofstream*>::iterator it)
    {
      delete *it;
      get_logs().erase(it);
    }

    static void clean()
    {
      for(std::list<std::ofstream*>::iterator it = get_logs().begin();
          it != get_logs().end();
          ++it)
        delete *it;
       get_logs().clear();
    }

    template<log::level l>
    static inline void thread_safe_trace(const std::string &who, const std::string &msg);

    template<log::level l>
    static inline void trace(const std::string &who, const std::string &msg);

    inline static 
    bool& get_verbosity()
    {
      static bool v = false;
      return v;
    }

    private:
      static inline 
      std::list<std::ofstream*>& get_logs()
      {
        static std::list<std::ofstream*> logs;
        return logs;
      }
  };//! log

  static std::ostream& operator<<(std::ostream& o, log::level l);

} //!global

#include "log.hxx"

#endif

