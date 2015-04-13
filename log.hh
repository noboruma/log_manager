#ifndef LOG_HH
#define LOG_HH

//#define LOG_VERBOSE_LEVEL N
//#define LOG_DEBUG_LEVEL 1
//#define LOG_INFO_LEVEL 2
#include <iostream>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>

namespace global {

  template<class out>
  struct log
  {
    enum class level
    {
      DEBUG,
      VERBOSE
    };

    template<level l=level::VERBOSE>
    static void print(const std::string &who, const std::string &msg)
    {
      std::cout<<"["<<who<<"] "<<log::get_now()<< " (INFO) " <<msg<<std::endl;
    }

    //template<>
    //static void operator()(const std::string &who, const std::string &msg)
    //{
    //  out<<"["<<who<<"] "<<log::get_now()<< " (DEBUG) " <<msg<<std::endl;
    //}


    static std::string get_now()
    {
      auto now = std::chrono::system_clock::now();
      auto in_time_t = std::chrono::system_clock::to_time_t(now);
      auto duration = now.time_since_epoch();
      auto duration_minus_one = now - std::chrono::seconds(1);
      auto millis1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
      auto millis2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration_minus_one.time_since_epoch()).count();
      auto millis = millis1 -millis2;

      std::stringstream ss;
      char format_date [100];
      std::strftime(format_date,
                    sizeof(format_date),
                    "%Y-%m-%d %H:%M:%S",
                    std::localtime(&in_time_t)); 

      ss<<format_date<<":"<<millis;
      
      return ss.str();
    }
    
  };
  
} //!global

#endif

