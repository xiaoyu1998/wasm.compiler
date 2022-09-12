

static constexpr int128_t PRECISION_1       = 100000000; 
static constexpr uint64_t PRECISION         = 8; 

static constexpr uint64_t INITIAL_FRAGMENTS_SUPPLY = 50 * 1000000 * PRECISION_1;
static constexpr uint64_t MAX_UINT64               = std::numeric_limits<int64_t>::max(); 
static constexpr uint64_t TOTAL_GONS               = MAX_UINT64 - MAX_UINT64 % INITIAL_FRAGMENTS_SUPPLY; 

static constexpr uint64_t MAX_SUPPLY               = MAX_UINT64; 
static constexpr uint64_t ONE_DAY                  = 24 * 60 * 60; 
static constexpr uint64_t TWO_HOURS                =  2 * 60 * 60; 
static constexpr uint64_t FIFTEEN_MINUTES          = 15 * 60; 
static constexpr bool     AMPL_DEBUG               = true;        

WASM_DECLARE_EXCEPTION( contract_failed, 1000000, "contract failed" )

#define TO_ASSET( amount, code ) \
    asset(amount, symbol(code, PRECISION))


#define GET_ORACLE_DATA(oracle) \
    get_return_wasm<uint64_t>(wasm::transaction{oracle, name("get_data"), std::vector<permission>{{get_self(), name("wasmio_code")}}, std::tuple<string>("data")}.call())

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

uint128_t div_u128(uint128_t a, uint128_t b){
    return a / b;
}

uint128_t mul_u128(uint128_t a, uint128_t b){
    return a * b;
}

int128_t div_i128(int128_t a, int128_t b){
    return a / b;
}

int128_t mul_i128(int128_t a, int128_t b){
    return a * b;
}

template<typename T>
inline T get_return_wasm(uint64_t size){
    T return_value = wasm::unpack<T>(get_return(size));
    return return_value;
}