#include <maker.hpp>
#include <chrono>
#include <wasm.hpp>

using namespace wasm;
using std::chrono::system_clock;

ACTION uniswap::createex( name    token_contract,
	                      symbol  token_symbol)
{
    check(is_account(token_contract), "token contract does not exist")

 	std::vector<char> key = pack(std::tuple(token_contract, token_symbol));
    checksum256 exchange_hash = sha256((const char*)key.data(), key.size());   

    exchange_type exchange;
    exchanges     exs_table(get_self(), exchange_scope);
    check( exs_table.get( exchange, exchange_hash ), "exchange already exist" );   

    exs_table.emplace( get_self(), [&]( auto& s ) {
       s.exchange_hash  = exchange_hash;
       s.token_contract = token_contract;
       s.base_reserve   = {0, symbol(base_symbol)};
       s.token_reserve  = {0, token_symbol};
       s.total_supply   = {0, token_symbol};
       s.closed         = false;            
    });

}

ACTION uniswap::transfer( name    from,
	                      name    to,
	                      asset   quantity,
	                      string  memo )
{

    if( to == get_self() && get_first_receiver() != get_self()){
    	std:vector<string> transfer_memo = string_split(memo, ':');
    	check(transfer_memo.size() > 2, "params mismatch");
    	auto action = transfer_memo[0];

    	capi_checksum256 exchange_hash;
		memcpy(&hash, from_hex(transfer_memo[1]).data(), 32);
		exchange_hash = {hash.hash};

		if( action == "add_liquidity") {
		     add_liquidity(exchange_hash);
		     return;
		}

		if( action == "base_2_token_in") {
		     base2tokenin(exchange_hash);
		     return;
		}

		if( action == "base_2_token_out") {
		     base2tokenout(exchange_hash);
		     return;
		}

		if( action == "token_2_base_in") {
		     token2basein(exchange_hash);
		     return;
		}

		if( action == "token_2_base_out") {
		     token2baseout(exchange_hash);
		     return;
		}
    }


    if( get_first_receiver() == get_self() ){
        // transfer liquidity
    	std:vector<string> transfer_memo = string_split(memo, ':');
    	check(transfer_memo.size() > 2, "params mismatch");
    	auto token_contract = transfer_memo[1];

 	    std::vector<char> key = pack(std::tuple(token_contract, quantity.symbol));
        checksum256 exchange_hash = sha256((const char*)key.data(), key.size()); 
	    transfer_internal(from, to , quantity, exchange_hash) 

        //remove_liquidity
	    if( to == get_self())  {
	    	auto action == transfer_memo[0];
	    	if( action == "remove_liquidity") {
		        remove_liquidity(exchange_hash);
		        return;
		    }
	    } 	
    }


}

void uniswap::add_liquidity( checksum256 exchange_hash, 
	                name     owner,
	                uint64_t nonce,
	                asset    min_liquidity, 
	                asset    max_tokens, 
	                asset    quantity, 
	                uint64_t deadline)
{
	require_auth( owner );

	check( current_block_time() >  deadline, "tx was expired" );
    check( quantity.amount >  0, "amount must be > 0" );

    exchange_type exchange;
    exchanges     exs_table(get_self(), exchange_scope);
    check( exs_table.get( exchange, exchange_hash ), "exchange does not exist" );   

    if(quantity.symbol == symbol(base_symbol) && get_first_receiver() == name(base_bank)){

    	std::vector<char> key = pack(std::tuple(exchange_hash, nonce));
        checksum256 liquidity_hash = sha256((const char*)key.data(), key.size());

    	liquidity_action_type liquidity_action;
    	liqas liqas_table(get_self(), owner.value);
    	check( !liqas_table.get( liquidity_action, liquidity_hash ), "add liquidity by base token already exist" );   

	    liqas_table.emplace( get_self(), [&]( auto& s ) {
	       s.liquidity_hash  = liquidity_hash;
	       s.owner           = owner;
	       s.nonce           = nonce;
	       s.min_liquidity   = min_liquidity;
	       s.max_tokens      = max_tokens;
	       s.base_quantity   = base_quantity;
	       s.token_quantity  = {};
	       s.deadline        = deadline;
	       s.closed          = false;             
	    });

        return;
    } else if ( quantity.symbol == exchange.token_symbol && get_first_receiver() == exchange.token_contract ){

    	std::vector<char> key = pack(std::tuple(exchange_hash, nonce));
        checksum256 liquidity_hash = sha256((const char*)key.data(), key.size());

    	liquidity_action_type liquidity_action;
    	liqas liqas_table(get_self(), owner.value);
    	check( liqas_table.get( liquidity_action, liquidity_hash ), "add liquidity by base token does not exist" );  

    	check( liquidity_action.owner ==  owner, "owner mismatch" );  
    	check( liquidity_action.nonce ==  nonce, "nonce mismatch" );  
    	check( liquidity_action.min_liquidity ==  min_liquidity, "min_liquidity mismatch" );  
    	check( liquidity_action.max_tokens ==  max_tokens, "min_liquidity mismatch" );  
    	check( liquidity_action.base_quantity.amount > 0 , "base coin must > 0" ); 
      	check( liquidity_action.deadline == deadline , "deadline mismatch" ); 
      	check( !liquidity_action.closed , "add liquidity was closed" ); 

        auto total_liquidity = exchange.total_supply;
        auto base_reserve  = exchange.base_reserve;//- liquidity_action.base_quantity;
        auto taken_reserve = exchange.token_reserve;
        token_amount       = taken_reserve * (liquidity_action.base_quantity / base_reserve) + 1;

        check(  token_amount <= quantity , "token quantity is not enough" ); 
      	liqas_table.modify( exchange, wasm::no_payer, [&]( auto& s ) {
      		s.token_quantity = quantity;
       		s.closed = true;
        });
     
        auto liquidity_minted = total_liquidity * (liquidity_action.base_quantity / base_reserve);

      	exs_table.modify( liquidity_action, wasm::no_payer, [&]( auto& s ) {
      		s.base_reserve  += liquidity_action.base_quantity;
      		s.taken_reserve += token_amount;
      		s.total_supply  += liquidity_minted;
       		s.closed = true;
        });

	    wasm::transaction inline_trx_liqs( get_self(),  
	                                  name("transfer"), 
	                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
	                                  std::tuple(get_self(), owner, liquidity_minted, get_self().to_string() +string(":add_liquidity")));
	    inline_trx_liqs.send();

	    wasm::transaction inline_trx_token( exchange.token_contract,  
	                                  name("transfer"), 
	                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
	                                  std::tuple(get_self(), owner, quantity - token_amount, string("add_liquidity")));
	    inline_trx_token.send();

        return;
    }

    check( false, "symbol is mismatch" );  
}

void uniswap::remove_liquidity( checksum256 exchange_hash, 
	                name     owner,
	                asset    amount, 
	                asset    min_base, 
	                asset    min_token, 
	                uint64_t deadline){

    require_auth( owner );

	check( current_block_time() >  deadline, "tx was expired" );
    check( amount  > 0 , "amount must be > 0"  )
	check( min_base > 0 ,  "base coin must be > 0"  )
	check( min_token > 0 , "token coin must be > 0" )

    exchange_type exchange;
    exchanges     exs_table(get_self(), exchange_scope);
    check( exs_table.get( exchange, exchange_hash ), "exchange does not exist" ); 

 	check( exchange.total_supply > 0 , "total_supply must be > 0" )   
 	auto base_amount  = exchange.base_reserve *  ( amount / exchange.total_supply );
 	auto token_amount = exchange.token_reserve * ( amount / exchange.total_supply );

	check( base_amount  > min_base ,  "base amount must be > min_base"  )
	check( token_amount > min_token , "token amount must be > min_token" )

  	exs_table.modify( exchange, wasm::no_payer, [&]( auto& s ) {
  		s.base_reserve  -= base_amount;
        s.token_reserve -= token_amount;
        s.total_supply  -= amount;

    });

    wasm::transaction inline_trx_liqs( wasm:name(base_bank),  
                                  name("transfer"), 
                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
                                  std::tuple(get_self(), owner, base_amount, string("add_liquidity")));
    inline_trx_liqs.send();

    wasm::transaction inline_trx_token( exchange.token_contract,  
                                  name("transfer"), 
                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
                                  std::tuple(get_self(), owner, token_amount, string("remove_liquidity")));
    inline_trx_token.send();
}

void uniswap::base2tokenin(checksum256 exchange_hash, name to, asset base_sold, asset min_tokens, uint64_t deadline)
{

	check( current_block_time() >  deadline, "tx was expired" );	
	check( base_sold.amount >  0, "base_sold must > 0" );
	check( min_tokens.amount >  0, "min_tokens must > 0" );

	exchange_type exchange;
    exchanges     exs_table(get_self(), exchange_scope);
    check( exs_table.get( exchange, exchange_hash ), "exchange does not exist" ); 

    //auto token_reserve = exchange.token_reserve;
    auto tokens_bought = get_input_price(base_sold, exchange.base_reserve,  exchange.token_reserve);
    check( tokens_bought >  min_tokens, "bought tokens must be > min quantity" );


  	exs_table.modify( exchange, wasm::no_payer, [&]( auto& s ) {
  		s.base_reserve  -= base_sold;
        s.token_reserve -= tokens_bought;
    });

    wasm::transaction inline_trx_token( exchange.token_contract,  
                                  name("transfer"), 
                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
                                  std::tuple(get_self(), to, tokens_bought, string("base2tokenin")));
    inline_trx_token.send();   

}

void uniswap::base2tokenout(checksum256 exchange_hash, name to, asset max_base, asset tokens_bought,  uint64_t deadline)
{
	check( current_block_time() >  deadline, "tx was expired" );	
	check( tokens_bought.amount >  0, "tokens_bought must > 0" );
	check( max_base.amount      >  0, "max_base must > 0" );

	exchange_type exchange;
    exchanges     exs_table(get_self(), exchange_scope);
    check( exs_table.get( exchange, exchange_hash ), "exchange does not exist" ); 

	auto token_reserve = exchange.token_reserve;
	auto base_sold = getOutputPrice(tokens_bought, exchange.base_reserve - max_base, token_reserve);

	auto base_refund = max_base - base_sold;
	check( base_refund >  0, "bought tokens must be > min quantity" );

    wasm::transaction inline_trx_liqs( wasm:name(base_bank),  
                                  name("transfer"), 
                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
                                  std::tuple(get_self(), sender, base_refund, string("add_liquidity")));
    inline_trx_liqs.send();

    wasm::transaction inline_trx_token( exchange.token_contract,  
                                  name("transfer"), 
                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
                                  std::tuple(get_self(), to, base_sold, string("remove_liquidity")));
    inline_trx_token.send();	

}

void uniswap::token2basein(checksum256 exchange_hash, name to, asset tokens_sold, asset min_base, uint64_t deadline)
{
	check( current_block_time() >  deadline, "tx was expired" );
	check( tokens_sold.amount   >  0, "tokens_sold must > 0" );
	check( min_base.amount      >  0, "min_base must > 0" );

	exchange_type exchange;
    exchanges     exs_table(get_self(), exchange_scope);
    check( exs_table.get( exchange, exchange_hash ), "exchange does not exist" ); 

    base_bought = getInputPrice(tokens_sold, exchange.token_reserve, exchange.base_reserve);
  	check( base_bought >  min_base, "base_bought must be > min_base" );  

    wasm::transaction inline_trx_liqs( wasm:name(base_bank),  
                                  name("transfer"), 
                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
                                  std::tuple(get_self(), to, base_bought, string("token2basein")));
    inline_trx_liqs.send();	
}

void uniswap::token2baseout(checksum256 exchange_hash, name to, asset base_bought, asset max_tokens, uint64_t deadline)
{
	check( current_block_time() >  deadline, "tx was expired" );
	check( tokens_sold.amount   >  0, "tokens_sold must > 0" );
	check( min_base.amount      >  0, "min_base must > 0" );

	exchange_type exchange;
    exchanges     exs_table(get_self(), exchange_scope);
    check( exs_table.get( exchange, exchange_hash ), "exchange does not exist" ); 

    auto tokens_sold = getOutputPrice(base_bought, exchange.token_reserve, exchange.base_reserve);

    auto token_refund = max_tokens - tokens_sold;
    check( token_refund >= 0 , "max_tokens must be > tokens_sold" );  

    wasm::transaction inline_trx_liqs( wasm:name(base_bank),  
                                  name("transfer"), 
                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
                                  std::tuple(get_self(), to, base_bought, string("add_liquidity"))); 
    inline_trx_liqs.send();

    wasm::transaction inline_trx_token( exchange.token_contract,  
                                  name("transfer"), 
                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
                                  std::tuple(get_self(), sender, token_refund, string("token2baseout")));//
    inline_trx_token.send();    

}



asset uniswap::get_input_price(asset input_amount, asset input_reserve, asset output_reserve)
{
    check( input_reserve  > 0 , "input reserve must be > 0"  )
	check( output_reserve > 0 , "output reserve must be > 0" )

    uint64_t input_amount_with_fee = input_amount * 997;
    //uint64_t numerator             = input_amount_with_fee * output_reserve;
    uint64_t numerator             = input_amount_with_fee;
    uint64_t denominator           = (input_reserve * 1000) + input_amount_with_fee;

    //fixme:this should be double
    return ( numerator * 1000000 / denominator) * output_reserve / 1000000;
}

asset uniswap::get_output_price(asset output_amount, asset input_reserve, asset output_reserve)
{
	check( input_reserve  > 0 , "input reserve must be > 0"  )
	check( output_reserve > 0 , "output reserve must be > 0" )

    // uint64_t numerator             = input_reserve * output_amount;
    uint64_t numerator             = output_amount;
    uint64_t denominator           = (output_reserve - output_amount) * 997;

    //fixme:this should be double
    return input_reserve * ( numerator  * 1000000 / denominator ) / 1000000 + 1;

}

void uniswap::transfer_internal( name    from,
		                         name    to,
		                         asset   quantity,
		                         checksum256  exchange_hash )
{
    check( from != to, "cannot transfer to self" );
    require_auth( from );
    check( is_account( to ), "to account does not exist");

	exchange_type exchange;
    exchanges     exs_table(get_self(), exchange_scope);
    check( exs_table.get( exchange, exchange_hash ), "exchange does not exist" ); 

    require_recipient( from );
    require_recipient( to );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( quantity.symbol == exchange.total_supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    auto payer = has_auth( to ) ? to : from;

    sub_balance( from, quantity, exchange_hash );
    add_balance( to, quantity, payer, exchange_hash );    
}

void uniswap::sub_balance( name owner, asset value, checksum256 exchange_hash ) {
   accounts from_acnts( _self, owner.value );

   account from;
   check( from_acnts.get( from, exchange_hash), "no balance object found" );
   check( from.balance.amount >= value.amount, "overdrawn balance" );

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
      });
}

void uniswap::add_balance( name owner, asset value, name payer, checksum256 exchange_hash )
{
   accounts to_acnts( _self, owner.value );

   account to;
   if( !to_acnts.get( to, exchange_hash ) ) {
      to_acnts.emplace( payer, [&]( auto& a ){
        a.exchange_hash = exchange_hash;
        a.owner         = owner;
        a.balance       = value;
      });
   } else {
      to_acnts.modify( to, wasm::no_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
}

extern "C" {
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
       switch( action ) { 
         case wasm::name( "transfer" ).value: 
             wasm::execute_action( wasm::name(receiver), wasm::name(code), &uniswap::transfer ); 
             break;
         case wasm::name( "createex" ).value: 
             wasm::execute_action( wasm::name(receiver), wasm::name(code), &uniswap::createex ); 
             break;
         default:
             check(false, "action does not exist");
             break;
       }
   }
}



