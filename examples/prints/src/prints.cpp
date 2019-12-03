#include <prints.hpp>
#include <chrono>

using namespace wasm;
using std::chrono::system_clock;


// ACTION prints::printnormal( )
// {
//     printhex(string("1234567890").data(),10);

//     int128_t a(-128);
//     int128_t b(-129);

//     vector<char> key = wasm::pack(std::tuple(a,b));
//     printhex(key.data(), 10);
// }

// ACTION prints::print1( wasm::time_point a, wasm::time_point b, wasm::time_point c )
// {

//     system_clock::time_point *ta = (system_clock::time_point *)&a;
//     system_clock::time_point *tb = (system_clock::time_point *)&b;

//     print(std::chrono::duration_cast<std::chrono::microseconds>(*tb - *ta).count());
//     print("name:","xiaoyu");

// }


// ACTION prints::print2( int128_t a, int128_t b )
// {

//     int128_t a1(-128);
//     int128_t b1(-129);

//     vector<char> key1 = wasm::pack(std::tuple(a1,b1));
//     printhex(key1.data(), key1.size());

//     print("-");
//     vector<char> key2 = wasm::pack(std::tuple(a,b));
//     printhex(key2.data(), key2.size());

//     check( key1 == key2, "key1 is defferent from key2" );
// }

// ACTION prints::print3( float a, float b, float c )
// {

//     float a1 = 1.0f / 2.0f;
//     float b1 = 5.0f * -0.75f;
//     float c1 = 2e-6 / 3.0;

//     vector<char> key1 = wasm::pack(std::tuple(a1,b1,c1));
//     printhex(key1.data(), key1.size());

//     print("-");
//     vector<char> key2 = wasm::pack(std::tuple(a,b,c));
//     printhex(key2.data(), key2.size());

//     //check( c1 == c, "c1 is defferent from c" );
//     check( key1 == key2, "key1 is defferent from key2" );
// }

// ACTION prints::print4( double a, double b, double c )
// {

//     double a1 = 1.0 / 2.0;
//     double b1 = 5.0 * -0.75;
//     double c1 = 2e-6 / 3.0;

//     print(a1);
//     print(b1);
//     print(c1);
//     vector<char> key1 = wasm::pack(std::tuple(a1,b1,c1));
//     printhex(key1.data(), key1.size());


//     print(a);
//     print(b);
//     print(c);
//     print("-");
//     vector<char> key2 = wasm::pack(std::tuple(a,b,c));
//     printhex(key2.data(), key2.size());

//     //check( c1 == c, "c1 is defferent from c" );
//     check( key1 == key2, "key1 is defferent from key2" );
// }

// static inline string FromHex( string str ) {

//     std::map<char, uint8_t> hex = {
//             {'0', 0x00},
//             {'1', 0x01},
//             {'2', 0x02},
//             {'3', 0x03},
//             {'4', 0x04},
//             {'5', 0x05},
//             {'6', 0x06},
//             {'7', 0x07},
//             {'8', 0x08},
//             {'9', 0x09},
//             {'a', 0x0a},
//             {'b', 0x0b},
//             {'c', 0x0c},
//             {'d', 0x0d},
//             {'e', 0x0e},
//             {'f', 0x0f}
//     };
//     vector<uint8_t> v;

//     for (std::string::size_type i = 0; i < str.size();) {

//         //uint8_t t = hex[(char)str[i]] | hex[(char)str[i + 1]] << 4;
//         uint8_t h = hex[(char) str[i]];
//         uint8_t l = hex[(char) str[i + 1]];
//         uint8_t t = l | h << 4;
//         v.push_back( t );

//         i += 2;
//     }

//     return string(v.begin(), v.end());

// }


// ACTION prints::print5( checksum160 a, checksum256 b, checksum512 c )
// {

//     // print(sizeof(checksum160));
//     // print("\n");
//     // print(sizeof(checksum256));
//     // print("\n");
//     // print(sizeof(checksum512));
//     // print("\n");

//     vector<char> a_v(&a.hash[0], &a.hash[sizeof(a.hash)/sizeof(a.hash[0])]);
//     vector<char> b_v(&b.hash[0], &b.hash[sizeof(b.hash)/sizeof(b.hash[0])]);
//     vector<char> c_v(&c.hash[0], &c.hash[sizeof(c.hash)/sizeof(c.hash[0])]);

//     // print(a_v.size());
//     // print("\n");
//     // print(b_v.size());
//     // print("\n");
//     // print(c_v.size());
//     // print("\n");
    
//      printhex(a_v.data(), a_v.size());
//      print("\n");
//      printhex(b_v.data(), b_v.size());
//      print("\n");
//      printhex(c_v.data(), c_v.size());
//      print("\n");

//      string a_s("ba7816bf8f01cfea414140de5dae2223b00361a3000000000000000000000000");
//      string b_s("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
//      string c_s("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015adba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

//      vector<char> a_v2(FromHex(a_s).begin(), FromHex(a_s).end());
//      vector<char> b_v2(FromHex(b_s).begin(), FromHex(b_s).end());
//      vector<char> c_v2(FromHex(c_s).begin(), FromHex(c_s).end());

//      printhex(FromHex(a_s).data(), FromHex(a_s).size());
//      print("\n");
//      printhex(FromHex(b_s).data(), FromHex(b_s).size());
//      print("\n");
//      printhex(FromHex(c_s).data(), FromHex(c_s).size());

//      // check( a_v == a_v2, "a is defferent from a_v2" );
//      // check( b_v == b_v2, "b is defferent from b_v2" );
//      // check( c_v == c_v2, "c is defferent from c_v2" );

// }

// ACTION prints::printinf( )
// {
//     print("test infinite");
//     int i = 0;
//     while(1){
//         i++;
//     }

// }


// WASM_DISPATCH( prints, (printnormal)(print1)(print2)(print3)(print4)(print5)(printinf))

ACTION prints::printnormal( )
{

    uint64_t p = 0x1122334455667788;

    vector<char> v;
    v.resize(sizeof(uint64_t));
    memcpy(v.data(), (char *)(&p), sizeof(uint64_t));
    printhex(v.data(), v.size());

}

WASM_DISPATCH( prints, (printnormal))


