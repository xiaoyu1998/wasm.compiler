#include <wasm.hpp>
#include <asset.hpp>
#include <table.hpp>
#include <string>
using namespace wasm;
using namespace std;

static constexpr int64_t  min_lock_time = 60;
static constexpr int64_t  max_lock_time = 7*24*60*60;
static constexpr int64_t  hash_time_lock_scope = 100;

CONTRACT hashtimelock : public contract {
   public:
      using contract::contract;
      ACTION transfer( name from, name to, asset quantity, string memo );
      ACTION unlock  ( string key, name unlocker);
      ACTION refund  ( string key, name locker  );

  private:
     TABLE  hash_time_lock {
        name           bank;
        checksum256    lock_hash;
        name           locker;
        name           unlocker;
        asset          locked_quantity;
        uint64_t       locked_block_time;
        uint64_t       refund_lock_seconds;
        bool           closed;

        checksum256 primary_key()const { return lock_hash; }
     };

     uint64_t get_lock_deadline(const hash_time_lock& htl);
     void     lock(checksum256 lock_hash, name locker, name unlocker, asset quantity, uint64_t refund_lock_seconds);

     typedef wasm::table< "htls"_n, hash_time_lock, checksum256 > htls;  	


};
