#pragma once
#include <stdint.h>
#include <string>
#include "serialize.hpp"
#include "check.hpp"
#include "print.hpp"
//#include <boost/date_time/posix_time/posix_time.hpp>

namespace wasm {
  /**
   *  @defgroup time
   *  @ingroup core
   *  @brief Classes for working with time.
   */

  class microseconds {
    public:
        explicit microseconds( int64_t c = 0) :_count(c){}

        /// @cond INTERNAL
        static microseconds maximum() { return microseconds(0x7fffffffffffffffll); }
        friend microseconds operator + (const  microseconds& l, const microseconds& r ) { return microseconds(l._count+r._count); }
        friend microseconds operator - (const  microseconds& l, const microseconds& r ) { return microseconds(l._count-r._count); }

        bool operator==(const microseconds& c)const { return _count == c._count; }
        bool operator!=(const microseconds& c)const { return _count != c._count; }
        friend bool operator>(const microseconds& a, const microseconds& b){ return a._count > b._count; }
        friend bool operator>=(const microseconds& a, const microseconds& b){ return a._count >= b._count; }
        friend bool operator<(const microseconds& a, const microseconds& b){ return a._count < b._count; }
        friend bool operator<=(const microseconds& a, const microseconds& b){ return a._count <= b._count; }
        microseconds& operator+=(const microseconds& c) { _count += c._count; return *this; }
        microseconds& operator-=(const microseconds& c) { _count -= c._count; return *this; }
        int64_t count()const { return _count; }
        int64_t to_seconds()const { return _count/1000000; }

        int64_t _count;
        /// @endcond
        WASMLIB_SERIALIZE( microseconds, (_count) )
    private:
        friend class time_point;
  };

  inline microseconds seconds( int64_t s ) { return microseconds( s * 1000000 ); }
  inline microseconds milliseconds( int64_t s ) { return microseconds( s * 1000 ); }
  inline microseconds minutes(int64_t m) { return seconds(60*m); }
  inline microseconds hours(int64_t h) { return minutes(60*h); }
  inline microseconds days(int64_t d) { return hours(24*d); }

  /**
   *  High resolution time point in microseconds
   *
   *  @ingroup time
   */
  class time_point {
    public:
        explicit time_point( microseconds e = microseconds() ) :elapsed(e){}
        const microseconds& time_since_epoch()const { return elapsed; }
        uint32_t            sec_since_epoch()const  { return uint32_t(elapsed.count() / 1000000); }

        time_point operator = ( const uint64_t t )
        {
            elapsed = microseconds(t);
            return *this;
        }

        std::string to_string() const{
            auto count = elapsed.count();
            return std::to_string(count);
        }

        void print() const {::wasm::print(to_string());}

        /// @cond INTERNAL
        bool   operator > ( const time_point& t )const                              { return elapsed._count > t.elapsed._count; }
        bool   operator >=( const time_point& t )const                              { return elapsed._count >=t.elapsed._count; }
        bool   operator < ( const time_point& t )const                              { return elapsed._count < t.elapsed._count; }
        bool   operator <=( const time_point& t )const                              { return elapsed._count <=t.elapsed._count; }
        bool   operator ==( const time_point& t )const                              { return elapsed._count ==t.elapsed._count; }
        bool   operator !=( const time_point& t )const                              { return elapsed._count !=t.elapsed._count; }
        time_point&  operator += ( const microseconds& m)                           { elapsed+=m; return *this;                 }
        time_point&  operator -= ( const microseconds& m)                           { elapsed-=m; return *this;                 }
        time_point   operator + (const microseconds& m) const { return time_point(elapsed+m); }
        time_point   operator + (const time_point& m) const { return time_point(elapsed+m.elapsed); }
        time_point   operator - (const microseconds& m) const { return time_point(elapsed-m); }
        microseconds operator - (const time_point& m) const { return microseconds(elapsed.count() - m.elapsed.count()); }
        microseconds elapsed;
        /// @endcond

        WASMLIB_SERIALIZE( time_point, (elapsed) )
  };

  /**
   *  A lower resolution time_point accurate only to seconds from 1970
   *
   *  @ingroup time
   */
  class time_point_sec {
    public:
        time_point_sec()
        :utc_seconds(0){}

        explicit time_point_sec(uint64_t seconds )
        :utc_seconds(seconds){}

        // explicit time_point_sec(uint32_t seconds )
        // :utc_seconds(seconds){}

        time_point_sec( const time_point& t )
        :utc_seconds( uint32_t(t.time_since_epoch().count() / 1000000ll) ){}

        static time_point_sec maximum() { return time_point_sec(0xffffffff); }
        static time_point_sec min() { return time_point_sec(0); }

        operator time_point()const { return time_point( wasm::seconds( utc_seconds) ); }
        uint32_t sec_since_epoch()const { return utc_seconds; }

        std::string to_string() const{
          return std::to_string(utc_seconds);
        }

        void print() const{ 
          ::wasm::print(to_string()); 
        }

        /// @cond INTERNAL
         time_point_sec operator = ( const uint32_t t )
        {
            utc_seconds = t;
            return *this;
        }

        time_point_sec operator = ( const wasm::time_point& t )
        {
            utc_seconds = uint32_t(t.time_since_epoch().count() / 1000000ll);
            return *this;
        }
        friend bool      operator < ( const time_point_sec& a, const time_point_sec& b )  { return a.utc_seconds < b.utc_seconds; }
        friend bool      operator > ( const time_point_sec& a, const time_point_sec& b )  { return a.utc_seconds > b.utc_seconds; }
        friend bool      operator <= ( const time_point_sec& a, const time_point_sec& b )  { return a.utc_seconds <= b.utc_seconds; }
        friend bool      operator >= ( const time_point_sec& a, const time_point_sec& b )  { return a.utc_seconds >= b.utc_seconds; }
        friend bool      operator == ( const time_point_sec& a, const time_point_sec& b ) { return a.utc_seconds == b.utc_seconds; }
        friend bool      operator != ( const time_point_sec& a, const time_point_sec& b ) { return a.utc_seconds != b.utc_seconds; }
        time_point_sec&  operator += ( uint32_t m ) { utc_seconds+=m; return *this; }
        time_point_sec&  operator += ( microseconds m ) { utc_seconds+=m.to_seconds(); return *this; }
        time_point_sec&  operator += ( time_point_sec m ) { utc_seconds+=m.utc_seconds; return *this; }
        time_point_sec&  operator -= ( uint32_t m ) { utc_seconds-=m; return *this; }
        time_point_sec&  operator -= ( microseconds m ) { utc_seconds-=m.to_seconds(); return *this; }
        time_point_sec&  operator -= ( time_point_sec m ) { utc_seconds-=m.utc_seconds; return *this; }
        time_point_sec   operator +( uint32_t offset )const { return time_point_sec(utc_seconds + offset); }
        time_point_sec   operator -( uint32_t offset )const { return time_point_sec(utc_seconds - offset); }

        friend time_point   operator + ( const time_point_sec& t, const microseconds& m )   { return time_point(t) + m;             }
        friend time_point   operator - ( const time_point_sec& t, const microseconds& m )   { return time_point(t) - m;             }
        friend microseconds operator - ( const time_point_sec& t, const time_point_sec& m ) { return time_point(t) - time_point(m); }
        friend microseconds operator - ( const time_point& t, const time_point_sec& m ) { return time_point(t) - time_point(m); }
        uint32_t utc_seconds;

        /// @endcond

        WASMLIB_SERIALIZE( time_point_sec, (utc_seconds) )
  };


    // inline std::string to_string(const wasm::microseconds& v){
    //   return v.to_string();
    // }

    // inline std::string to_string(const wasm::time_point& v){
    //     return v.to_string();
    // }

    // inline std::string to_string(const wasm::time_point_sec& v){
    //   return v.to_string();

    // }


} // namespace wasm
