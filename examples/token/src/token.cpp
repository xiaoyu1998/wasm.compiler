#include <token.hpp>
//#include <sstream>
#include <chrono>

using namespace wasm;
using std::chrono::system_clock;

ACTION token::create( name   issuer,
                    asset  maximum_supply )
{

    require_auth( _self );

    auto sym = maximum_supply.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( maximum_supply.is_valid(), "invalid supply" );
    check( maximum_supply.amount > 0, "max-supply must be positive" );

    currency_stats st;
    stats statstable( _self, sym.code().raw() );
    check( !statstable.get( st, sym.code().raw() ), "token with symbol already exists" );

    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = issuer;
    });
}


ACTION token::issue( name to, asset quantity, string memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    currency_stats st;
    stats statstable(_self, sym.code().raw()) ;
    check( statstable.get( st, sym.code().raw() ), "token with symbol does not exist, create token before issue" );

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, wasm::no_payer, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );

    if( to != st.issuer ) {
      // token::transfer_action transfer_act{get_self()};
      // transfer_act.send(st.issuer, to, quantity, memo);
      wasm::transaction inline_trx(get_self(), name("transfer"), std::vector<permission>{{get_self(), name("wasmio.code")}}, std::tuple(st.issuer, to, quantity, memo));
      inline_trx.send();

    }
}

ACTION token::retire( asset quantity, string memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    currency_stats st;
    stats statstable(_self, sym.code().raw()) ;
    check( statstable.get( st, sym.code().raw() ), "token with symbol does not exist" );

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must retire positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    statstable.modify( st, wasm::no_payer, [&]( auto& s ) {
       s.supply -= quantity;
    });

    sub_balance( st.issuer, quantity );
}

ACTION token::transfer( name    from,
                      name    to,
                      asset   quantity,
                      string  memo )
{
    print("from:", from, " ");
    print("to:",to, " ");
    print(quantity.to_string()," ");
    print(memo);

    check( from != to, "cannot transfer to self" );
    require_auth( from );
    check( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.code();

    currency_stats st;
    stats statstable( _self, sym.raw() );
    statstable.get( st, sym.raw() );

    require_recipient( from );
    require_recipient( to );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    auto payer = has_auth( to ) ? to : from;

    sub_balance( from, quantity );
    add_balance( to, quantity, payer );
}

void token::sub_balance( name owner, asset value ) {
   accounts from_acnts( _self, owner.value );

   account from;
   check( from_acnts.get( from, value.symbol.code().raw()), "no balance object found" );
   check( from.balance.amount >= value.amount, "overdrawn balance" );

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
      });
}

void token::add_balance( name owner, asset value, name payer )
{
   accounts to_acnts( _self, owner.value );

   account to;
   if( !to_acnts.get( to, value.symbol.code().raw() ) ) {
      to_acnts.emplace( payer, [&]( auto& a ){
        a.owner = owner;
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, wasm::no_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
}

ACTION token::open( name owner, const symbol& symbol, name payer )
{
   require_auth( payer );

   auto sym_code_raw = symbol.code().raw();
   stats statstable( _self, sym_code_raw );

   currency_stats st;
   check( statstable.get( st, sym_code_raw), "symbol does not exist" );
   check( st.supply.symbol == symbol, "symbol precision mismatch" );

   accounts acnts( _self, owner.value );
   account account;

   if( ! acnts.get( account, sym_code_raw ) ) {
      acnts.emplace( payer, [&]( auto& a ){
        a.owner = owner;
        a.balance = asset{0, symbol};
      });
   }
}

ACTION token::close( name owner, const symbol& symbol )
{
   require_auth( owner );
   accounts acnts( _self, owner.value );

   account account;
   check( acnts.get(account, symbol.code().raw() ), "Balance row already deleted or never existed. Action won't have any effect." );
   check( account.balance.amount == 0, "Cannot close because the balance is not zero." );
   acnts.erase( account, wasm::no_payer );
}

ACTION token::printtest( )
{
    // int128_t a(1);
    // int128_t b(0);
    // int128_t c(std::numeric_limits<int128_t>::lowest());
    // int128_t d(-87654323456);
    // print(a);
    // print("\n");
    // print(b);
    // print("\n");
    // print(c);
    // print("\n");
    // print(d);
    // print("\n");

    // uint128_t a((uint128_t)-1);
    // uint128_t b(0);
    // uint128_t c(87654323456);
    // print(a);
    // print("\n");
    // print(b);
    // print("\n");
    // print(c);
    // print("\n");


   // float x = 1.0f / 2.0f;
   // float y = 5.0f * -0.75f;
   // float z = 2e-6f / 3.0f;
   // print(x);
   // print("\n");
   // print(y);
   // print("\n");
   // print(z);
   // print("\n");

   // double x = 1.0 / 2.0;
   // double y = 5.0 * -0.75;
   // double z = 2e-6 / 3.0;
   // print(x);
   // print("\n");
   // print(y);
   // print("\n");
   // print(z);
   // print("\n");


   // long double x = 1.0l / 2.0l;
   // long double y = 5.0l * -0.75l;
   // long double z = 2e-6l / 3.0l;
   // print(x);
   // print("\n");
   // print(y);
   // print("\n");
   // print(z);
   // print("\n");

    printhex(string("1234567890").data(),10);

    int128_t a(-128);
    int128_t b(-129);

    vector<char> key = wasm::pack(std::tuple(a,b));
    printhex(key.data(), 10);
}


ACTION token::print2( int128_t a, int128_t b )
{

    int128_t a1(-128);
    int128_t b1(-129);

    vector<char> key1 = wasm::pack(std::tuple(a1,b1));
    printhex(key1.data(), key1.size());

    print("-");
    vector<char> key2 = wasm::pack(std::tuple(a,b));
    printhex(key2.data(), key2.size());

    check( key1 == key2, "key1 is defferent from key2" );
}

ACTION token::print3( float a, float b, float c )
{

    float a1 = 1.0f / 2.0f;
    float b1 = 5.0f * -0.75f;
    float c1 = 2e-6 / 3.0;

    vector<char> key1 = wasm::pack(std::tuple(a1,b1,c1));
    printhex(key1.data(), key1.size());

    print("-");
    vector<char> key2 = wasm::pack(std::tuple(a,b,c));
    printhex(key2.data(), key2.size());

    //check( c1 == c, "c1 is defferent from c" );
    check( key1 == key2, "key1 is defferent from key2" );
}

ACTION token::print4( double a, double b, double c )
{

    // float a1(-1.28);
    // float b1(-1.29);

    double a1 = 1.0 / 2.0;
    double b1 = 5.0 * -0.75;
    double c1 = 2e-6 / 3.0;

    print(a1);
    print(b1);
    print(c1);
    vector<char> key1 = wasm::pack(std::tuple(a1,b1,c1));
    printhex(key1.data(), key1.size());


    print(a);
    print(b);
    print(c);
    print("-");
    vector<char> key2 = wasm::pack(std::tuple(a,b,c));
    printhex(key2.data(), key2.size());

    //check( c1 == c, "c1 is defferent from c" );
    check( key1 == key2, "key1 is defferent from key2" );
}

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
//     std::stringstream ss;

//     for (std::string::size_type i = 0; i < str.size();) {

//         //uint8_t t = hex[(char)str[i]] | hex[(char)str[i + 1]] << 4;
//         uint8_t h = hex[(char) str[i]];
//         uint8_t l = hex[(char) str[i + 1]];
//         uint8_t t = l | h << 4;
//         ss << t;

//         i += 2;
//     }

//     return ss.str();

// }


ACTION token::print5( checksum160 a, checksum256 b, checksum512 c )
{

     // string a_s("ba7816bf8f01cfea414140de5dae2223b00361a3");
     // string b_s("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
     // string c_s("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015adba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");


     // vector<char> a_v2(FromHex(a_s).data(), FromHex(a_s).data() + FromHex(a_s).size());
     // vector<char> b_v2(FromHex(b_s).data(), FromHex(b_s).data() + FromHex(b_s).size());
     // vector<char> c_v2(FromHex(c_s).data(), FromHex(c_s).data() + FromHex(c_s).size());


     // printhex((char * )&a.hash, sizeof(a));
     // printhex((char * )&b.hash, sizeof(b));
     // printhex((char * )&c.hash, sizeof(c));

     // print("\n");

     // vector<char> a_v((char * )&a.hash, (char * )(&a.hash + sizeof(a)));
     // vector<char> b_v((char * )&b.hash, (char * )(&b.hash + sizeof(b)));
     // vector<char> c_v((char * )&c.hash, (char * )(&c.hash + sizeof(c)));

     // check( a_v == a_v2, "a_v is defferent from a_v2" );
     // check( b_v == b_v2, "b_v is defferent from b_v2" );
     // check( b_v == c_v2, "c_v is defferent from c_v2" );

}

// static inline string FromTime(const std::time_t &t){

//     char szTime[128];
//     // std::tm *p;
//     // p = localtime(&t);
//     sprintf( szTime, "%4d-%2d-%2dT%2d:%2d:%2d",p->tm_year+1900, p->tm_mon+1, p->tm_mday,p->tm_hour, p->tm_min, p->tm_sec); 

//     return string(szTime);

// }


ACTION token::print1( wasm::time_point a, wasm::time_point b, wasm::time_point c )
{

    system_clock::time_point *ta = (system_clock::time_point *)&a;
    system_clock::time_point *tb = (system_clock::time_point *)&b;

    print(std::chrono::duration_cast<std::chrono::microseconds>(*tb - *ta).count());


    // print(a.time_since_epoch().count());
    // print(b.time_since_epoch().count()); 
    // print(c.time_since_epoch().count());


}

WASM_DISPATCH( token, (create)(issue)(retire)(transfer)(open)(close)(printtest)(print2)(print3)(print4)(print1))


