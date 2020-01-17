#include <hashtimelock.hpp>
#include <system.hpp>

using namespace wasm;
struct __attribute__((aligned (16))) capi_checksum256 { uint8_t hash[32]; };

static inline vector<string> string_split(string str, char delimiter){
    vector<string> r;
    string tmpstr;
    while (!str.empty()){
        int ind = str.find_first_of(delimiter);
        if (ind == -1){
            r.push_back(str);
            str.clear();
        }
        else{
            r.push_back(str.substr(0, ind));
            str = str.substr(ind + 1, str.size() - ind - 1);
        }
    }
    return r;

}

static inline string from_hex( string str ) {

    std::map<char, uint8_t> hex = {
            {'0', 0x00},
            {'1', 0x01},
            {'2', 0x02},
            {'3', 0x03},
            {'4', 0x04},
            {'5', 0x05},
            {'6', 0x06},
            {'7', 0x07},
            {'8', 0x08},
            {'9', 0x09},
            {'a', 0x0a},
            {'b', 0x0b},
            {'c', 0x0c},
            {'d', 0x0d},
            {'e', 0x0e},
            {'f', 0x0f}
    };
    vector<uint8_t> v;

    for (std::string::size_type i = 0; i < str.size();) {

        uint8_t h = hex[(char) str[i]];
        uint8_t l = hex[(char) str[i + 1]];
        uint8_t t = l | h << 4;
        v.push_back( t );

        i += 2;
    }

    return string(v.begin(), v.end());
}


ACTION hashtimelock::transfer( name    from,
		                       name    to,
		                       asset   quantity,
		                       string  memo )
{
	if(to != get_self() ) return;

    //memo: "68feb6a4097a45d6e56f5b84f6c381b0c638a1306eb95b7ee2354e19838461e4:walker:120" 
	std:vector<string> transfer_memo = string_split(memo, ':');
    check( transfer_memo.size()    == 3,  "memo must be hash:name:locktime in seconds, eg. 68feb6a4097a45d6e56f5b84f6c381b0c638a1306eb95b7ee2354e19838461e4:walker:120" );
	check( transfer_memo[0].size() == 64, "lock_hash size must be 32 bytes from sha256" );
	checksum256    lock_hash;
	name           unlocker;
	uint64_t       refund_lock_seconds;

    capi_checksum256 hash;
	memcpy(&hash, from_hex(transfer_memo[0]).data(), 32);
	lock_hash = {hash.hash};
    lock_hash.print();

	unlocker            = name(transfer_memo[1]);
	refund_lock_seconds = atoi(transfer_memo[2].c_str());

	check( is_account(unlocker), "unlocker does not exist" );
	check( refund_lock_seconds >= min_lock_time, "lock time must be >= 60 seconds" );
	check( refund_lock_seconds <= max_lock_time, "lock time must be <= 7*24*60*60 seconds(7 days)" );

	lock( lock_hash, from, unlocker, quantity, refund_lock_seconds);
    
}

ACTION hashtimelock::unlock( string    key,
	                         name      unlocker) {
    require_auth( unlocker );

    checksum256 lock_hash = sha256((const char*)key.data(), key.size());

    hash_time_lock htl;
    htls           htls_table(get_self(), hash_time_lock_scope);
    check( htls_table.get( htl, lock_hash ), "lock does not exist" );
    check( !htl.closed, "lock was closed" );
    check( htl.unlocker == unlocker, "unlocker mismatch" );
    check( current_block_time() <= get_lock_deadline(htl), "lock was expired" );

    htls_table.modify( htl, wasm::no_payer, [&]( auto& s ) {
       s.closed = true;
    });

    wasm::transaction inline_trx( htl.bank,  
                                  name("transfer"), 
                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
                                  std::tuple(get_self(), unlocker, htl.locked_quantity, string("unlocked")));
    
    inline_trx.send();

}

ACTION hashtimelock::refund( string key,
	                         name   locker) {

    require_auth( locker );

    checksum256 lock_hash = sha256((const char*)key.data(), key.size());

    hash_time_lock htl;
    htls           htls_table(get_self(), hash_time_lock_scope);
    check( htls_table.get( htl, lock_hash ), "lock does not exist" );
    check( !htl.closed, "lock was closed" );  
    check( htl.locker == locker, "locker mismatch" );
    check( current_block_time() >  get_lock_deadline(htl), "lock was not expired" );

    htls_table.modify( htl, wasm::no_payer, [&]( auto& s ) {
       s.closed = true;
    });

    wasm::transaction inline_trx( htl.bank,  
                                  name("transfer"), 
                                  std::vector<permission>{{get_self(), name("wasmio.code")}}, 
                                  std::tuple(get_self(), locker, htl.locked_quantity, string("refund")));
    
    inline_trx.send();

}

void hashtimelock::lock( checksum256 lock_hash,
                         name        locker, 
	                     name        unlocker, 
	                     asset       quantity, 
	                     uint64_t    refund_lock_seconds) {
   require_auth( locker );

   hash_time_lock htl;
   htls           htls_table(get_self(), hash_time_lock_scope);
   check( !htls_table.get( htl, lock_hash ), "lock has already exists" );

    htls_table.emplace( get_self(), [&]( auto& s ) {
       s.bank                = get_first_receiver();
       s.lock_hash           = lock_hash;
       s.locker              = locker;
       s.unlocker            = unlocker;
       s.locked_quantity     = quantity;
       s.locked_block_time   = current_block_time();
       s.refund_lock_seconds = refund_lock_seconds;
       s.closed              = false;             
    });

}

uint64_t hashtimelock::get_lock_deadline(const hash_time_lock& htl) {
    return htl.locked_block_time + htl.refund_lock_seconds;
}


extern "C" {
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
       switch( action ) { 
         case wasm::name( "transfer" ).value: 
             wasm::execute_action( wasm::name(receiver), wasm::name(code), &hashtimelock::transfer ); 
             break;
         case wasm::name( "unlock" ).value: 
             wasm::execute_action( wasm::name(receiver), wasm::name(code), &hashtimelock::unlock ); 
             break;
         case wasm::name( "refund" ).value: 
             wasm::execute_action( wasm::name(receiver), wasm::name(code), &hashtimelock::refund ); 
             break;
         default:
             check(false, "action does not exist");
             break;
       }
   }
}



