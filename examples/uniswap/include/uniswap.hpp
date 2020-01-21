#include <wasm.hpp>
#include <asset.hpp>
#include <string>
using namespace wasm;
using namespace std;

#define base_bank "wasmio.bank"
#define base_symbol "8,WUSD"

static constexpr int64_t  exchange_scope = 100;


CONTRACT uniswap : public contract {
    public:
        using contract::contract;

        ACTION createex( name token_contract, symbol token_symbol );
        ACTION transfer( name from, name to, asset quantity, string memo );

    private:
        void add_liquidity( checksum256 exchange, name owner, asset quantity, uint64_t deadline);
        void remove_liquidity( checksum256 exchange, name owner, asset quantity, uint64_t deadline);

        void base2tokenin(checksum256 exchange_hash, name to, asset base_sold, asset min_token, uint64_t deadline)
        void base2tokenout(checksum256 exchange_hash, name to, asset max_base, asset tokens_bought,  uint64_t deadline)

        void token2basein(checksum256 exchange_hash, name to, asset quantity);
        void token2baseout(checksum256 exchange_hash, name to, asset quantity);

        // void token2tokenin(name to, asset quantity);
        // void token2tokenout(name to, asset quantity);

        // void token2exin(checksum256 exchange_hash, name to, asset quantity);
        // void token2exout(checksum256 exchange_hash, name to, asset quantity);
        uint64_t get_input_price(uint64_t input_amount, uint64_t input_reserve, uint64_t output_reserve);
        uint64_t get_output_price(uint64_t output_amount, uint64_t input_reserve, uint64_t output_reserve);

        TABLE  exchange_type {
            checksum256 exchange_hash,
            name        token_contract,
            symbol      token_symbol,
            asset       base_reserve,
            asset       token_reserve,
            asset       total_supply,
            boold       closed
            checksum256 primary_key()const { return exchange_hash; }
        };

        TABLE liquidity_action_type {
            checksum256    liquidity_hash,
            name           owner,
            uint64_t       nonce,
            asset          min_liquidity, 
            asset          max_tokens, 
            asset          base_quantity, 
            asset          token_quantity,
            uint64_t       deadline,
            bool           closed

            checksum256 primary_key()const { return liquidity_hash; }
        }

        TABLE  account {
            checksum256 exchange_hash,
            name        owner;
            asset       balance;

            checksum256 primary_key()const { return exchange_hash; }
        };

        typedef wasm::table< "exchanges"_n, exchange_type, checksum256 >     exchanges;  
        typedef wasm::table< "liqas"_n, liquidity_action_type, checksum256 > liqas;  
        typedef wasm::table< "accounts"_n, account, checksum256 >            accounts;

        void transfer_internal( name from, name to, asset quantity, checksum256 exchange_hash );
        void sub_balance( name owner, asset value, checksum256 exchange_hash );
        void add_balance( name owner, asset value, name ram_payer, checksum256 exchange_hash );
};
