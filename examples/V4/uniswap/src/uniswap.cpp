#include <uniswap.hpp>
#include <uniswap_utils.hpp>
#include <inline_transaction.hpp>
#include <uniswap_database.hpp>
#include <return.hpp>
#include <make_log.hpp>
#include <exception.hpp>

using namespace wasm;

std::optional<market_t> g_market;

void uniswap::_update(asset balance0, asset balance1)
{
    WASM_LOG_FPRINT(UNISWAP_DEBUG, "balance0:% balance1:%", balance0, balance1)

    check(g_market.has_value(), "market does not exist");

    time_point_sec block_timestamp = time_point_sec(current_block_time()) ;
    uint64_t       time_elapsed    = (block_timestamp - g_market->block_timestamp_last).count();

    if(time_elapsed > 0 && g_market->reserve0.amount != 0 && g_market->reserve1.amount != 0){
        g_market->price0_cumulative_last += divide_decimal(multiply_decimal(g_market->reserve0.amount, PRECISION_1), g_market->reserve1.amount) * time_elapsed;
        g_market->price1_cumulative_last += divide_decimal(multiply_decimal(g_market->reserve1.amount, PRECISION_1), g_market->reserve0.amount) * time_elapsed;
    }

    g_market->reserve0             = balance0;
    g_market->reserve1             = balance1;   
    g_market->block_timestamp_last = block_timestamp;

}

ACTION uniswap::init(regid token0, regid token1, symbol symbol0, symbol symbol1, regid liquidity_token)
{
    require_auth( get_maintainer(get_self()));
    check( symbol0.raw() < symbol1.raw(), "symbol1 must be > symbol0" );

    check(!g_market.has_value(), "market already exist");

    market_t market(get_self().value);
    market.token0               = token0;
    market.token1               = token1;

    market.reserve0             = asset(0, symbol0);
    market.reserve1             = asset(0, symbol1);
    market.block_timestamp_last = 0;

    market.liquidity_token         = liquidity_token;
    market.liquidity_total_supply  = TO_ASSET(0, add_symbol(symbol0, symbol1));
    market.closed                  = false;

    WASM_LOG_FPRINT(UNISWAP_DEBUG, "market:%", market)

    g_market = market;

}

ACTION uniswap::mint(regid to)
{
    require_auth( to );

    check(g_market.has_value(), "market does not exist");    
    check(!g_market->closed, "market has been closed");  

    asset balance0 = BALANCE_OF(g_market->token0, get_self(), g_market->reserve0.symbol);
    asset balance1 = BALANCE_OF(g_market->token1, get_self(), g_market->reserve1.symbol);

    asset amount0 = balance0 - g_market->reserve0;
    asset amount1 = balance1 - g_market->reserve1;

    WASM_LOG_FPRINT(UNISWAP_DEBUG, "balance0:% balance1:% amount0:% amount1:%", balance0, balance1, amount0, amount1)

    int64_t liquidity;
    if(g_market->liquidity_total_supply.amount == 0){

        liquidity = sqrt(multiply_decimal(amount0.amount, amount1.amount)) - MINIMUM_LIQUIDITY * PRECISION_1;

        asset init_liquidity = asset(MINIMUM_LIQUIDITY * PRECISION_1, g_market->liquidity_total_supply.symbol);
        MINT(g_market->liquidity_token, get_self(), init_liquidity);     
        g_market->liquidity_total_supply = init_liquidity;   

    }else{
        WASM_LOG_FPRINT(UNISWAP_DEBUG,"liquidity0:% liquidity1:%", 
            divide_decimal(multiply_decimal(amount0.amount, g_market->liquidity_total_supply.amount), g_market->reserve0.amount),
            divide_decimal(multiply_decimal(amount1.amount, g_market->liquidity_total_supply.amount), g_market->reserve1.amount))

        liquidity = min(divide_decimal(multiply_decimal(amount0.amount, g_market->liquidity_total_supply.amount), g_market->reserve0.amount), divide_decimal(multiply_decimal(amount1.amount, g_market->liquidity_total_supply.amount), g_market->reserve1.amount));
    }

    check( liquidity > 0, "insufficient liquidity minted" );

    asset to_mint_liquidity = asset(liquidity, g_market->liquidity_total_supply.symbol);

    MINT(g_market->liquidity_token, to, to_mint_liquidity);
    g_market->liquidity_total_supply += to_mint_liquidity;

    _update(balance0, balance1);

    WASM_LOG_FPRINT(UNISWAP_DEBUG,"to:% liquidity:% liquidity_total_supply:% balance0:% balance1:%", 
        to, to_mint_liquidity, g_market->liquidity_total_supply, balance0, balance1)

    set_return(wasm::pack<asset>(to_mint_liquidity));
}

ACTION uniswap::burn(regid to)
{
    require_auth( to );

    check(g_market.has_value(), "market does not exist");    
    check(!g_market->closed, "market has been closed");     

    asset balance0 = BALANCE_OF(g_market->token0, get_self(), g_market->reserve0.symbol);
    asset balance1 = BALANCE_OF(g_market->token1, get_self(), g_market->reserve1.symbol);

    asset to_burn_liquidity;
    to_burn_liquidity = BALANCE_OF(g_market->liquidity_token, to, g_market->liquidity_total_supply.symbol);

    asset amount0 = asset(divide_decimal(multiply_decimal(balance0.amount, divide_decimal(multiply_decimal(to_burn_liquidity.amount, PRECISION_1), g_market->liquidity_total_supply.amount)), PRECISION_1), balance0.symbol);
    asset amount1 = asset(divide_decimal(multiply_decimal(balance1.amount, divide_decimal(multiply_decimal(to_burn_liquidity.amount, PRECISION_1), g_market->liquidity_total_supply.amount)), PRECISION_1), balance1.symbol);

    WASM_LOG_FPRINT(UNISWAP_DEBUG,"to:% to_burn_liquidity:% liquidity_total_supply:% balance0:% balance1:% amount0:% amount1:%", 
        to, to_burn_liquidity, g_market->liquidity_total_supply, balance0, balance1, amount0, amount1)     

    BURN(g_market->liquidity_token, to, to_burn_liquidity)

    TRANSFER(g_market->token0, get_self(), to, amount0);
    TRANSFER(g_market->token1, get_self(), to, amount1);

    balance0 = BALANCE_OF(g_market->token0, get_self(), g_market->reserve0.symbol);
    balance1 = BALANCE_OF(g_market->token1, get_self(), g_market->reserve1.symbol);

    g_market->liquidity_total_supply = g_market->liquidity_total_supply - to_burn_liquidity;

    _update(balance0, balance1);

    WASM_LOG_FPRINT(UNISWAP_DEBUG,"to:% to_burn_liquidity:% liquidity_total_supply:% balance0:% balance1:% amount0:% amount1:%", 
        to, to_burn_liquidity, g_market->liquidity_total_supply, balance0, balance1, amount0, amount1) 

    set_return(wasm::pack<asset>(to_burn_liquidity));
}

ACTION uniswap::swap(asset amount0_out, asset amount1_out, regid to)
{
    //require_auth( to );
    check(amount0_out > 0 || amount1_out > 0, "insufficient output amount");

    check(g_market.has_value(), "market does not exist");    
    check(!g_market->closed, "market has been closed");     

    check(amount0_out < g_market->reserve0  || amount1_out < g_market->reserve1, "insufficient liquidity");

    WASM_LOG_FPRINT(UNISWAP_DEBUG,"to:% reserve0:% reserve1:%", to, g_market->reserve0, g_market->reserve1) 

    asset balance0, balance1;
    {
        check(g_market->token0 != to  && g_market->token1 != to, "invalid to");
        if(amount0_out > 0) TRANSFER(g_market->token0, get_self(), to, amount0_out);
        if(amount1_out > 0) TRANSFER(g_market->token1, get_self(), to, amount1_out);
        balance0 = BALANCE_OF(g_market->token0, get_self(), g_market->reserve0.symbol);
        balance1 = BALANCE_OF(g_market->token1, get_self(), g_market->reserve1.symbol);
    }

    asset amount0_in = balance0 > g_market->reserve0 - amount0_out ? balance0 - g_market->reserve0 - amount0_out : asset(0, g_market->reserve0.symbol);
    asset amount1_in = balance1 > g_market->reserve1 - amount1_out ? balance1 - g_market->reserve1 - amount1_out : asset(0, g_market->reserve1.symbol);    
    check(amount0_in > 0 || amount0_in > 0, "insufficient input amount");
    {
        asset balance0_adjusted = balance0 * 1000 - amount0_in * 3;
        asset balance1_adjusted = balance1 * 1000 - amount1_in * 3;

        auto index0 = multiply_decimal(balance0_adjusted.amount, balance1_adjusted.amount);
        auto index1 = multiply_decimal(g_market->reserve0.amount, g_market->reserve1.amount) * 1000 * 1000;

        check(index0 >= index1, uniswap_failed{}, "invalid amount in balance0_adjusted:% balance1_adjusted:% index0:% index1:%", 
            balance0_adjusted, balance1_adjusted, index0, index1);

        WASM_LOG_FPRINT(UNISWAP_DEBUG, "to:% amount0_in:% amount1_in:% amount0_out:% amount1_out:% balance0:% balance1:% index0:% index1:%", 
        to, amount0_in, amount1_in, amount0_out, amount1_out, balance0, balance1, index0, index1) 

    }
    
    _update(balance0, balance1);
}

ACTION uniswap::skim(regid to)
{
    require_auth( get_maintainer(get_self()));

    check(g_market.has_value(), "market does not exist");    

    TRANSFER(g_market->token0, get_self(), to, BALANCE_OF(g_market->token0, to, g_market->reserve0.symbol) - g_market->reserve0);
    TRANSFER(g_market->token1, get_self(), to, BALANCE_OF(g_market->token1, to, g_market->reserve1.symbol) - g_market->reserve1);
}

ACTION uniswap::sync()
{
    require_auth( get_maintainer(get_self()));

    check(g_market.has_value(), "market does not exist"); 

    _update(BALANCE_OF(g_market->token0, get_self(), g_market->reserve0.symbol), BALANCE_OF(g_market->token1, get_self(), g_market->reserve1.symbol));
}

ACTION uniswap::close(bool closed)
{
    require_auth( get_maintainer(get_self()));

    check(g_market.has_value(), "market does not exist");   

    g_market->closed = closed;
}

ACTION uniswap::get_market(){

    check(g_market.has_value(), "market does not exist"); 

    WASM_LOG_FPRINT(true, "market:%", g_market.value());
    set_return(wasm::pack<market_t>(g_market.value()));
}

extern "C" bool pre_dispatch(regid self, regid original_receiver, name action) {
   market_t market(self.value);
   if(wasm::db::get(market)) g_market = market;

   return true;
}

extern "C" void post_dispatch(regid self, regid original_receiver, name action) {
  if(g_market.has_value()) wasm::db::set(g_market.value());

}


