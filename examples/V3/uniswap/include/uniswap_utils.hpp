
static constexpr uint64_t UNISWAP_MINT_STEP1 = 1;
static constexpr uint64_t UNISWAP_MINT_STEP2 = 2;
static constexpr uint64_t MINIMUM_LIQUIDITY  = 10*10*10;
static constexpr int128_t PRECISION_1        = 100000000; 
static constexpr uint64_t PRECISION          = 8; 
static constexpr bool     UNISWAP_DEBUG      = true; 

WASM_DECLARE_EXCEPTION( uniswap_failed, 1000000, "uniswap contract failed" )


#define TO_ASSET( amount, code ) \
    asset(amount, symbol(code, PRECISION))

inline string add_symbol(symbol symbol0, symbol symbol1){
    std::string code =  symbol0.code().to_string() + symbol1.code().to_string();
    return code.substr(0,7);
}

#define MINT(token, to, quantity) \
    {wasm::transaction trx(token, name("mint"), std::vector<permission>{{get_self(), name("wasmio_code")}}, std::tuple<regid, asset, string>(to, quantity, "mint"));\
    trx.send();}

#define BURN(token, from, quantity) \
    {wasm::transaction trx(token, name("burn"), std::vector<permission>{{get_self(), name("wasmio_code")}}, std::tuple<regid, asset, string>(from, quantity, "burn"));\
    trx.send();}

#define TRANSFER(token, from, to, quantity) \
    {wasm::transaction trx(token, name("transfer"), std::vector<permission>{{from, name("wasmio_code")}}, std::tuple<regid, regid, asset, string>(from, to, quantity, "transfer"));\
     trx.send();}

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

inline uint128_t string_to_market_id(std::string id){
    std:vector <string> market_id = string_split(id, '-');
    check(market_id.size() == 2, "market id error");
    return (int128_t)symbol_code(market_id[0]).raw() << 64 | (int128_t)symbol_code(market_id[1]).raw();  
}

inline std::string market_id_to_string(const uint128_t& market_id){  
    std:string code = symbol_code{static_cast<uint64_t>(market_id >> 64)}.to_string() + "-" +symbol_code{static_cast<uint64_t>(market_id)}.to_string();  
    return code.substr(0,7);
}
