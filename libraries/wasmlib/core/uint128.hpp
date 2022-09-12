#pragma once

#include <limits>
#include <stdint.h>
#include <string>
#include "check.hpp"

namespace wasm {

template<typename T>
    static void divide( const T &numerator, const T &denominator, T &quotient, T &remainder ) {
        static const int bits = sizeof(T) * 8;//CHAR_BIT;

        if (denominator == 0) {
            check(false, "divide by zero");
        } else {
            T n = numerator;
            T d = denominator;
            T x = 1;
            T answer = 0;


            while ((n >= d) && (((d >> (bits - 1)) & 1) == 0)) {
                x <<= 1;
                d <<= 1;
            }

            while (x != 0) {
                if (n >= d) {
                    n -= d;
                    answer |= x;
                }

                x >>= 1;
                d >>= 1;
            }

            quotient = answer;
            remainder = n;
        }
    }

    class uint128 {


    public:
        uint128() : hi(0), lo(0) {}
        uint128( uint32_t l ) : hi(0), lo(l) {}
        uint128( int32_t l ) : hi(-(l < 0)), lo(l) {}
        uint128( int64_t l ) : hi(-(l < 0)), lo(l) {}
        uint128( uint64_t l ) : hi(0), lo(l) {}
        uint128( const std::string &sz ): hi(0), lo(0) {
            // do we have at least one character?
            if (!sz.empty()) {
                // make some reasonable assumptions
                int radix = 10;
                bool minus = false;

                std::string::const_iterator i = sz.begin();

                // check for minus sign, i suppose technically this should only apply
                // to base 10, but who says that -0x1 should be invalid?
                if (*i == '-') {
                    ++i;
                    minus = true;
                }

                // check if there is radix changing prefix (0 or 0x)
                if (i != sz.end()) {
                    if (*i == '0') {
                        radix = 8;
                        ++i;
                        if (i != sz.end()) {
                            if (*i == 'x') {
                                radix = 16;
                                ++i;
                            }
                        }
                    }

                    while (i != sz.end()) {
                        unsigned int n = 0;
                        const char ch = *i;

                        if (ch >= 'A' && ch <= 'Z') {
                            if (((ch - 'A') + 10) < radix) {
                                n = (ch - 'A') + 10;
                            } else {
                                break;
                            }
                        } else if (ch >= 'a' && ch <= 'z') {
                            if (((ch - 'a') + 10) < radix) {
                                n = (ch - 'a') + 10;
                            } else {
                                break;
                            }
                        } else if (ch >= '0' && ch <= '9') {
                            if ((ch - '0') < radix) {
                                n = (ch - '0');
                            } else {
                                break;
                            }
                        } else {
                            /* completely invalid character */
                            break;
                        }

                        (*this) *= radix;
                        (*this) += n;

                        ++i;
                    }
                }

                // if this was a negative number, do that two's compliment madness :-P
                if (minus) {
                    *this = -*this;
                }
            }
        };
        uint128( uint64_t _h, uint64_t _l )
                : hi(_h), lo(_l) {}
        //uint128( const fc::bigint& bi );
        explicit uint128( unsigned __int128 i ) : hi(i >> 64), lo(i) {}

        operator std::string() const{
            if (*this == 0) { return "0"; }

            // at worst it will be size digits (base 2) so make our buffer
            // that plus room for null terminator
            static char sz[128 + 1];
            sz[sizeof(sz) - 1] = '\0';

            uint128 ii(*this);
            int i = 128 - 1;

            while (ii != 0 && i) {

                uint128 remainder;
                divide(ii, uint128(10), ii, remainder);
                sz[--i] = "0123456789abcdefghijklmnopqrstuvwxyz"[remainder.to_integer()];
            }

            return &sz[i];
        }

        //operator fc::bigint()const;

        explicit operator unsigned __int128() const {
            unsigned __int128 result(hi);
            result <<= 64;
            return result | lo;
        }

        bool operator==( const uint128 &o ) const { return hi == o.hi && lo == o.lo; }
        bool operator!=( const uint128 &o ) const { return hi != o.hi || lo != o.lo; }
        bool operator<( const uint128 &o ) const { return (hi == o.hi) ? lo < o.lo : hi < o.hi; }
        bool operator<( const int64_t &o ) const { return *this < uint128(o); }
        bool operator!() const { return !(hi != 0 || lo != 0); }
        uint128 operator-() const { return ++uint128(~hi, ~lo); }
        uint128 operator~() const { return uint128(~hi, ~lo); }

        uint128 &operator++() {
            hi += (++lo == 0);
            return *this;
        }
        uint128 &operator--() {
            hi -= (lo-- == 0);
            return *this;
        }
        uint128 operator++( int ) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }
        uint128 operator--( int ) {
            auto tmp = *this;
            --(*this);
            return tmp;
        }

        uint128 &operator|=( const uint128 &u ) {
            hi |= u.hi;
            lo |= u.lo;
            return *this;
        }
        uint128 &operator&=( const uint128 &u ) {
            hi &= u.hi;
            lo &= u.lo;
            return *this;
        }
        uint128 &operator^=( const uint128 &u ) {
            hi ^= u.hi;
            lo ^= u.lo;
            return *this;
        }
        uint128 &operator<<=( const uint128 &rhs ){
            if (rhs >= 128) {
                hi = 0;
                lo = 0;
            } else {
                unsigned int n = rhs.to_integer();
                const unsigned int halfsize = 128 / 2;

                if (n >= halfsize) {
                    n -= halfsize;
                    hi = lo;
                    lo = 0;
                }

                if (n != 0) {
                    // shift high half
                    hi <<= n;

                    const uint64_t mask(~(uint64_t(-1) >> n));

                    // and add them to high half
                    hi |= (lo & mask) >> (halfsize - n);

                    // and finally shift also low half
                    lo <<= n;
                }
            }

            return *this;
        }

        uint128 &operator>>=( const uint128 &rhs ){
            if (rhs >= 128) {
                hi = 0;
                lo = 0;
            } else {
                unsigned int n = rhs.to_integer();
                const unsigned int halfsize = 128 / 2;

                if (n >= halfsize) {
                    n -= halfsize;
                    lo = hi;
                    hi = 0;
                }

                if (n != 0) {
                    // shift low half
                    lo >>= n;

                    // get lower N bits of high half
                    const uint64_t mask(~(uint64_t(-1) << n));

                    // and add them to low qword
                    lo |= (hi & mask) << (halfsize - n);

                    // and finally shift also high half
                    hi >>= n;
                }
            }
            return *this;            
        }

        uint128 &operator+=( const uint128 &u ) {
            const uint64_t old = lo;
            lo += u.lo;
            hi += u.hi + (lo < old);
            return *this;
        }
        uint128 &operator-=( const uint128 &u ) { return *this += -u; }
        uint128 &operator*=( const uint128 &b ){
            uint64_t a0 = (uint32_t)(this->lo);
            uint64_t a1 = (uint32_t)(this->lo >> 0x20);
            uint64_t a2 = (uint32_t)(this->hi);
            uint64_t a3 = (uint32_t)(this->hi >> 0x20);

            uint64_t b0 = (uint32_t)(b.lo);
            uint64_t b1 = (uint32_t)(b.lo >> 0x20);
            uint64_t b2 = (uint32_t)(b.hi);
            uint64_t b3 = (uint32_t)(b.hi >> 0x20);

            // (a0 + (a1 << 0x20) + (a2 << 0x40) + (a3 << 0x60)) *
            // (b0 + (b1 << 0x20) + (b2 << 0x40) + (b3 << 0x60)) =
            //  a0 * b0
            //
            // (a1 * b0 + a0 * b1) << 0x20
            // (a2 * b0 + a1 * b1 + a0 * b2) << 0x40
            // (a3 * b0 + a2 * b1 + a1 * b2 + a0 * b3) << 0x60
            //
            // all other cross terms are << 0x80 or higher, thus do not appear in result

            this->hi = 0;
            this->lo = a3 * b0;
            (*this) += a2 * b1;
            (*this) += a1 * b2;
            (*this) += a0 * b3;
            (*this) <<= 0x20;
            (*this) += a2 * b0;
            (*this) += a1 * b1;
            (*this) += a0 * b2;
            (*this) <<= 0x20;
            (*this) += a1 * b0;
            (*this) += a0 * b1;
            (*this) <<= 0x20;
            (*this) += a0 * b0;

            return *this;            
        }
        //uint128& operator /= ( const uint128& u );
        // uint128& uint128::operator/=(const uint128 &b)
        // {
        //     auto self = (m128(hi) << 64) + m128(lo);
        //     auto other = (m128(b.hi) << 64) + m128(b.lo);
        //     self /= other;
        //     hi = static_cast<uint64_t>(self >> 64);
        //     lo = static_cast<uint64_t>((self << 64 ) >> 64);

        //     return *this;
        // }
        
        uint128 &operator%=( const uint128 &b ){
            uint128 quotient;
            divide(*this, b, quotient, *this);
            return *this;            
        }


        friend uint128 operator+( const uint128 &l, const uint128 &r ) { return uint128(l) += r; }
        friend uint128 operator-( const uint128 &l, const uint128 &r ) { return uint128(l) -= r; }
        friend uint128 operator*( const uint128 &l, const uint128 &r ) { return uint128(l) *= r; }
        //friend uint128 operator / ( const uint128& l, const uint128& r )   { return uint128(l)/=r;   }
        friend uint128 operator%( const uint128 &l, const uint128 &r ) { return uint128(l) %= r; }
        friend uint128 operator|( const uint128 &l, const uint128 &r ) { return uint128(l) = (r); }
        friend uint128 operator&( const uint128 &l, const uint128 &r ) { return uint128(l) &= r; }
        friend uint128 operator^( const uint128 &l, const uint128 &r ) { return uint128(l) ^= r; }
        friend uint128 operator<<( const uint128 &l, const uint128 &r ) { return uint128(l) <<= r; }
        friend uint128 operator>>( const uint128 &l, const uint128 &r ) { return uint128(l) >>= r; }
        friend bool operator>( const uint128 &l, const uint128 &r ) { return r < l; }
        friend bool operator>( const uint128 &l, const int64_t &r ) { return uint128(r) < l; }
        friend bool operator>( const int64_t &l, const uint128 &r ) { return r < uint128(l); }

        friend bool operator>=( const uint128 &l, const uint128 &r ) { return l == r || l > r; }
        friend bool operator>=( const uint128 &l, const int64_t &r ) { return l >= uint128(r); }
        friend bool operator>=( const int64_t &l, const uint128 &r ) { return uint128(l) >= r; }
        friend bool operator<=( const uint128 &l, const uint128 &r ) { return l == r || l < r; }
        friend bool operator<=( const uint128 &l, const int64_t &r ) { return l <= uint128(r); }
        friend bool operator<=( const int64_t &l, const uint128 &r ) { return uint128(l) <= r; }

        //friend std::size_t hash_value( const uint128& v ) { return city_hash_size_t((const char*)&v, sizeof(v)); }

        uint32_t to_integer() const {
            //FC_ASSERT( hi == 0 );
            uint32_t lo32 = (uint32_t) lo;
            //FC_ASSERT( lo == lo32 );
            return lo32;
        }
        uint64_t to_uint64() const {
            //FC_ASSERT( hi == 0 );
            return lo;
        }
        uint32_t low_32_bits() const { return (uint32_t) lo; }
        uint64_t low_bits() const { return lo; }
        uint64_t high_bits() const { return hi; }

        static uint128 max_value() {
            const uint64_t max64 = std::numeric_limits<uint64_t>::max();
            return uint128(max64, max64);
        }

        static void full_product( const uint128 &a, const uint128 &b, uint128 &result_hi, uint128 &result_lo )
        {
            uint64_t ah = a.hi;
            uint64_t al = a.lo;
            uint64_t bh = b.hi;
            uint64_t bl = b.lo;

            uint128 s = al;
            s *= bl;
            uint128 r = ah;
            r *= bl;
            uint128 q = al;
            q *= bh;
            uint128 p = ah;
            p *= bh;

            uint64_t sl = s.lo;
            uint64_t sh = s.hi;
            uint64_t rl = r.lo;
            uint64_t rh = r.hi;
            uint64_t ql = q.lo;
            uint64_t qh = q.hi;
            uint64_t pl = p.lo;
            uint64_t ph = p.hi;

            uint64_t y[4];    // final result
            y[0] = sl;

            uint128 acc = sh;
            acc += ql;
            acc += rl;
            y[1] = acc.lo;
            acc = acc.hi;
            acc += qh;
            acc += rh;
            acc += pl;
            y[2] = acc.lo;
            y[3] = acc.hi + ph;

            result_hi = uint128(y[3], y[2]);
            result_lo = uint128(y[1], y[0]);

            return;            
        }

        static uint8_t _popcount_64( uint64_t x ) {
            static const uint64_t m[] = {
                    0x5555555555555555ULL,
                    0x3333333333333333ULL,
                    0x0F0F0F0F0F0F0F0FULL,
                    0x00FF00FF00FF00FFULL,
                    0x0000FFFF0000FFFFULL,
                    0x00000000FFFFFFFFULL
            };
            // TODO future optimization:  replace slow, portable version
            // with fast, non-portable __builtin_popcountll intrinsic
            // (when available)

            for (int i = 0, w = 1; i < 6; i++, w += w) {
                x = (x & m[i]) + ((x >> w) & m[i]);
            }
            return uint8_t(x);
        }

        uint8_t popcount() const {
            return _popcount_64(lo) + _popcount_64(hi);
        }

        // fields must be public for serialization
        uint64_t hi;
        uint64_t lo;
    };

    // uint128::uint128( const std::string &sz )
    //         : hi(0), lo(0) {
    //     // do we have at least one character?
    //     if (!sz.empty()) {
    //         // make some reasonable assumptions
    //         int radix = 10;
    //         bool minus = false;

    //         std::string::const_iterator i = sz.begin();

    //         // check for minus sign, i suppose technically this should only apply
    //         // to base 10, but who says that -0x1 should be invalid?
    //         if (*i == '-') {
    //             ++i;
    //             minus = true;
    //         }

    //         // check if there is radix changing prefix (0 or 0x)
    //         if (i != sz.end()) {
    //             if (*i == '0') {
    //                 radix = 8;
    //                 ++i;
    //                 if (i != sz.end()) {
    //                     if (*i == 'x') {
    //                         radix = 16;
    //                         ++i;
    //                     }
    //                 }
    //             }

    //             while (i != sz.end()) {
    //                 unsigned int n = 0;
    //                 const char ch = *i;

    //                 if (ch >= 'A' && ch <= 'Z') {
    //                     if (((ch - 'A') + 10) < radix) {
    //                         n = (ch - 'A') + 10;
    //                     } else {
    //                         break;
    //                     }
    //                 } else if (ch >= 'a' && ch <= 'z') {
    //                     if (((ch - 'a') + 10) < radix) {
    //                         n = (ch - 'a') + 10;
    //                     } else {
    //                         break;
    //                     }
    //                 } else if (ch >= '0' && ch <= '9') {
    //                     if ((ch - '0') < radix) {
    //                         n = (ch - '0');
    //                     } else {
    //                         break;
    //                     }
    //                 } else {
    //                     /* completely invalid character */
    //                     break;
    //                 }

    //                 (*this) *= radix;
    //                 (*this) += n;

    //                 ++i;
    //             }
    //         }

    //         // if this was a negative number, do that two's compliment madness :-P
    //         if (minus) {
    //             *this = -*this;
    //         }
    //     }
    // }


    // uint128::operator bigint()const
    // {
    //    auto tmp  = uint128( bswap_64( hi ), bswap_64( lo ) );
    //    bigint bi( (char*)&tmp, sizeof(tmp) );
    //    return bi;
    // }
    // uint128::uint128( const fc::bigint& bi )
    // {
    //    *this = uint128( std::string(bi) ); // TODO: optimize this...
    // }

    // uint128::operator std::string() const {
    //     if (*this == 0) { return "0"; }

    //     // at worst it will be size digits (base 2) so make our buffer
    //     // that plus room for null terminator
    //     static char sz[128 + 1];
    //     sz[sizeof(sz) - 1] = '\0';

    //     uint128 ii(*this);
    //     int i = 128 - 1;

    //     while (ii != 0 && i) {

    //         uint128 remainder;
    //         divide(ii, uint128(10), ii, remainder);
    //         sz[--i] = "0123456789abcdefghijklmnopqrstuvwxyz"[remainder.to_integer()];
    //     }

    //     return &sz[i];
    // }


    // uint128 &uint128::operator<<=( const uint128 &rhs ) {
    //     if (rhs >= 128) {
    //         hi = 0;
    //         lo = 0;
    //     } else {
    //         unsigned int n = rhs.to_integer();
    //         const unsigned int halfsize = 128 / 2;

    //         if (n >= halfsize) {
    //             n -= halfsize;
    //             hi = lo;
    //             lo = 0;
    //         }

    //         if (n != 0) {
    //             // shift high half
    //             hi <<= n;

    //             const uint64_t mask(~(uint64_t(-1) >> n));

    //             // and add them to high half
    //             hi |= (lo & mask) >> (halfsize - n);

    //             // and finally shift also low half
    //             lo <<= n;
    //         }
    //     }

    //     return *this;
    // }

    // uint128 &uint128::operator>>=( const uint128 &rhs ) {
    //     if (rhs >= 128) {
    //         hi = 0;
    //         lo = 0;
    //     } else {
    //         unsigned int n = rhs.to_integer();
    //         const unsigned int halfsize = 128 / 2;

    //         if (n >= halfsize) {
    //             n -= halfsize;
    //             lo = hi;
    //             hi = 0;
    //         }

    //         if (n != 0) {
    //             // shift low half
    //             lo >>= n;

    //             // get lower N bits of high half
    //             const uint64_t mask(~(uint64_t(-1) << n));

    //             // and add them to low qword
    //             lo |= (hi & mask) << (halfsize - n);

    //             // and finally shift also high half
    //             hi >>= n;
    //         }
    //     }
    //     return *this;
    // }

    // uint128& uint128::operator/=(const uint128 &b)
    // {
    //     auto self = (m128(hi) << 64) + m128(lo);
    //     auto other = (m128(b.hi) << 64) + m128(b.lo);
    //     self /= other;
    //     hi = static_cast<uint64_t>(self >> 64);
    //     lo = static_cast<uint64_t>((self << 64 ) >> 64);

    //     return *this;
    // }

    // uint128 &uint128::operator%=( const uint128 &b ) {
    //     uint128 quotient;
    //     divide(*this, b, quotient, *this);
    //     return *this;
    // }

    // uint128 &uint128::operator*=( const uint128 &b ) {
    //     uint64_t a0 = (uint32_t)(this->lo);
    //     uint64_t a1 = (uint32_t)(this->lo >> 0x20);
    //     uint64_t a2 = (uint32_t)(this->hi);
    //     uint64_t a3 = (uint32_t)(this->hi >> 0x20);

    //     uint64_t b0 = (uint32_t)(b.lo);
    //     uint64_t b1 = (uint32_t)(b.lo >> 0x20);
    //     uint64_t b2 = (uint32_t)(b.hi);
    //     uint64_t b3 = (uint32_t)(b.hi >> 0x20);

    //     // (a0 + (a1 << 0x20) + (a2 << 0x40) + (a3 << 0x60)) *
    //     // (b0 + (b1 << 0x20) + (b2 << 0x40) + (b3 << 0x60)) =
    //     //  a0 * b0
    //     //
    //     // (a1 * b0 + a0 * b1) << 0x20
    //     // (a2 * b0 + a1 * b1 + a0 * b2) << 0x40
    //     // (a3 * b0 + a2 * b1 + a1 * b2 + a0 * b3) << 0x60
    //     //
    //     // all other cross terms are << 0x80 or higher, thus do not appear in result

    //     this->hi = 0;
    //     this->lo = a3 * b0;
    //     (*this) += a2 * b1;
    //     (*this) += a1 * b2;
    //     (*this) += a0 * b3;
    //     (*this) <<= 0x20;
    //     (*this) += a2 * b0;
    //     (*this) += a1 * b1;
    //     (*this) += a0 * b2;
    //     (*this) <<= 0x20;
    //     (*this) += a1 * b0;
    //     (*this) += a0 * b1;
    //     (*this) <<= 0x20;
    //     (*this) += a0 * b0;

    //     return *this;
    // }

    // void uint128::full_product( const uint128 &a, const uint128 &b, uint128 &result_hi, uint128 &result_lo ) {
    //     //   (ah * 2**64 + al) * (bh * 2**64 + bl)
    //     // = (ah * bh * 2**128 + al * bh * 2**64 + ah * bl * 2**64 + al * bl
    //     // =  P * 2**128 + (Q + R) * 2**64 + S
    //     // = Ph * 2**192 + Pl * 2**128
    //     // + Qh * 2**128 + Ql * 2**64
    //     // + Rh * 2**128 + Rl * 2**64
    //     // + Sh * 2**64  + Sl
    //     //

    //     uint64_t ah = a.hi;
    //     uint64_t al = a.lo;
    //     uint64_t bh = b.hi;
    //     uint64_t bl = b.lo;

    //     uint128 s = al;
    //     s *= bl;
    //     uint128 r = ah;
    //     r *= bl;
    //     uint128 q = al;
    //     q *= bh;
    //     uint128 p = ah;
    //     p *= bh;

    //     uint64_t sl = s.lo;
    //     uint64_t sh = s.hi;
    //     uint64_t rl = r.lo;
    //     uint64_t rh = r.hi;
    //     uint64_t ql = q.lo;
    //     uint64_t qh = q.hi;
    //     uint64_t pl = p.lo;
    //     uint64_t ph = p.hi;

    //     uint64_t y[4];    // final result
    //     y[0] = sl;

    //     uint128 acc = sh;
    //     acc += ql;
    //     acc += rl;
    //     y[1] = acc.lo;
    //     acc = acc.hi;
    //     acc += qh;
    //     acc += rh;
    //     acc += pl;
    //     y[2] = acc.lo;
    //     y[3] = acc.hi + ph;

    //     result_hi = uint128(y[3], y[2]);
    //     result_lo = uint128(y[1], y[0]);

    //     return;
    // }

    // static uint8_t _popcount_64( uint64_t x ) {
    //     static const uint64_t m[] = {
    //             0x5555555555555555ULL,
    //             0x3333333333333333ULL,
    //             0x0F0F0F0F0F0F0F0FULL,
    //             0x00FF00FF00FF00FFULL,
    //             0x0000FFFF0000FFFFULL,
    //             0x00000000FFFFFFFFULL
    //     };
    //     // TODO future optimization:  replace slow, portable version
    //     // with fast, non-portable __builtin_popcountll intrinsic
    //     // (when available)

    //     for (int i = 0, w = 1; i < 6; i++, w += w) {
    //         x = (x & m[i]) + ((x >> w) & m[i]);
    //     }
    //     return uint8_t(x);
    // }

    // uint8_t uint128::popcount() const {
    //     return _popcount_64(lo) + _popcount_64(hi);
    // }


}