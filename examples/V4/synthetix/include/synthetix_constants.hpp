#pragma once
#include <wasm.hpp>
#include <asset.hpp>
#include <table.hpp>

namespace wayki {

//synthetix
    static constexpr bool     SYNTHETIX_DEBUG           = true; 
    static constexpr uint64_t SYNTHETIX_VERSION         = 100; 
    static constexpr uint64_t SYNTHETIX_SCOPE           = 100; 

    static constexpr int128_t PRECISION_1               = 100000000; 
    static constexpr int128_t PRECISION                 = 8; 
    static constexpr uint64_t MAX_MPAS_SIZE             = 500;
    
    static constexpr uint64_t XUSD                      = SYMBOL_CODE(xUSD);    
    static constexpr uint64_t XETH                      = SYMBOL_CODE(xETH);  
    static constexpr uint64_t XT                        = SYMBOL_CODE(XT);  
    static constexpr name     KEY_SYNTHETIX_MPAS           = "synthetix.mpas"_n; 
    static constexpr name     KEY_INVERSE_KEYS          = "inverse.keys"_n; 

//exchanger
    static constexpr name     KEY_EXCHANGE_ENABLED      = "exenabled"_n; 
    static constexpr name     KEY_WAITING_PERIOD_SECS   = "waitingsecs"_n;  
    static constexpr bool     INIT_EXCHANGE_ENABLED     = true; 
    static constexpr uint64_t INIT_WAITING_PERIOD_SECS  = 3*60; 

//exchange rates
    static constexpr name     KEY_RATE_STALE_PERIOD     = "staleperiod"_n;   
    static constexpr name     KEY_ORACLE                = "oracle"_n;  

//exchange state
    static constexpr name     KEY_MAX_ENTRIES_IN_QUEUE  = "maxentries"_n;  
    static constexpr uint64_t INIT_MAX_ENTRIES_IN_QUEUE = 12; 

//state 
    static constexpr name     KEY_ISSUANCE_RATIO        = "issueratio"_n; 
    static constexpr name     KEY_DEBT_LEDGER_LENGTH    = "debtledgers"_n;
    static constexpr uint64_t MAX_ISSUANCE_RATIO        = std::numeric_limits<uint64_t>::max();
    static constexpr uint64_t INIT_ISSUANCE_RATIO       = MAX_ISSUANCE_RATIO / 5;

//issuer
    static constexpr uint64_t MAX_MINIMUM_STAKING_TIME  = 7*24*60*60; 
    static constexpr uint64_t INIT_MINIMUM_STAKING_TIME = 8*60*60; 
    static constexpr name     KEY_MINIMUM_STAKE_TIME    = "minstaketime"_n; 
    static constexpr name     KEY_LAST_ISSUE_EVENT      = "lastissue"_n; 

//fee pool 
    static constexpr name     KEY_EXCHANGE_FEE_RATE     = "freerate"_n;    
    static constexpr name     KEY_TARGET_THRESHOLD      = "tarthreshold"_n; 
    static constexpr name     KEY_FEE_PERIOD_DURATION   = "feeperioddur"_n; 
    static constexpr uint64_t MAX_EXCHANGE_FEE_RATE     = std::numeric_limits<uint64_t>::max() / 10;  
    static constexpr uint64_t INIT_EXCHANGE_FEE_RATE    = PRECISION_1 / 1000; 
    static constexpr uint64_t INIT_TARGET_THRESHOLD     = PRECISION_1 / 100; 

    static constexpr name     KEY_LAST_FEE_WITHDRAWAL   = "lfwithdrawal"_n; 
    static constexpr name     KEY_RECENT_FEE_PERIODS    = "feeperiods"_n;  
    static constexpr uint64_t POOL_FEE_PERIOD_LENGTH    = 3; 
    static constexpr uint64_t MIN_FEE_PERIOD_DURATION   = 24*60*60;     // one day
    static constexpr uint64_t MAX_FEE_PERIOD_DURATION   = 60*24*60*60;  // 60 days    
    static constexpr uint64_t INIT_RECENT_FEE_PERIODS   = 7*24*60*60;     

//fee pool state
    static constexpr uint64_t ACCOUNT_FEE_PERIOD_LENGTH = 6;     

}