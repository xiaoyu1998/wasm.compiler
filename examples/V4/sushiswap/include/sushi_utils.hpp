

static constexpr bool     DEBUG             = true;  
static constexpr int128_t PRECISION_1       = 100000000; 
static constexpr uint64_t PRECISION         = 8;  

static constexpr uint64_t BONUS_MULTIPLIER  = 10;
     

WASM_DECLARE_EXCEPTION( contract_failed, 1000000, "contract failed" )

#define TO_ASSET( amount, code ) \
    asset(amount, symbol(code, PRECISION))

#define BALANCE_OF(token, owner, sym) \
    get_return_wasm<asset>(wasm::transaction{token, name("balance_of"), std::vector<permission>{{get_self(), name("wasmio_code")}}, std::tuple<regid, class symbol>(owner, sym)}.call())

#define MINT(token, to, quantity) \
    {wasm::transaction inline_trx(token, name("mint"), std::vector<permission>{{get_self(), name("wasmio_code")}}, std::tuple<regid, asset, string>(to, quantity, "mint"));\
    inline_trx.call();}

#define BURN(token, from, quantity) \
    {wasm::transaction inline_trx(token, name("burn"), std::vector<permission>{{get_self(), name("wasmio_code")}}, std::tuple<regid, asset, string>(from, quantity, "BURN"));\
    inline_trx.call();}

#define TRANSFER(token, from, to, quantity) \
    {wasm::transaction trx(token, name("transfer"), std::vector<permission>{{from, name("wasmio_code")}}, std::tuple<regid, regid, asset, string>(from, to, quantity, "transfer"));\
     trx.call();}


// uint128_t divide_decimal(uint128_t a, uint128_t b){
//     return a / b;
// }

// uint128_t multiply_decimal(uint128_t a, uint128_t b){
//     return a * b;
// }
inline uint128_t divide_decimal(uint128_t a, uint128_t b){return a * PRECISION_1 / b;}
inline uint128_t multiply_decimal(uint128_t a, uint128_t b){return a * b / PRECISION_1;}

inline uint128_t sqrt(uint128_t y) {
    if(y > 3) {
       uint128_t z = y;
       uint128_t x = y / 2 + 1;
       while(x < z){
          z = x;
          x = (y / x + x) / 2;
       }
       return z;
    } else if(y != 0) {
       return 1;
    }

    return 0;
}

template<typename T>
inline T get_return_wasm(uint64_t size){
    T return_value = wasm::unpack<T>(get_return(size));
    return return_value;
}