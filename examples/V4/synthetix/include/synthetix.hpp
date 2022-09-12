#include <wasm.hpp>
#include <asset.hpp>
#include <table.hpp>
#include <string>
#include <map>

#include <synthetix_entities.hpp>

using namespace wasm;
using namespace std;
using namespace wasm::db;


#define TO_ASSET( amount, currency_key ) \
    asset(amount, symbol(symbol_code(currency_key), PRECISION))


CONTRACT synthetix : public contract {
    
    using contract::contract;

    public:
        //synthetix
        void     debt_balance_of_and_total_debt(regid issuer, uint64_t currency_key, uint64_t& debt_balance, uint64_t& total_system_debt);
        uint64_t collateralisation_ratio(regid issuer);
        uint64_t max_issuable_mpas(regid issuer);
        void     remaining_issuable_mpas(regid issuer, uint64_t& max_issuable, uint64_t& already_issued, uint64_t& total_system_debt);
        regid    get_mpa_issuer_bank(uint64_t currency_key);
        bool     existing_mpa(uint64_t currency_key);

        //synthetix state
        void     init_synthetix_state();
        uint64_t debt_ledger_length();
        void     issuance_data(regid issuer, uint64_t& initial_debt_owner_ship, uint64_t& debt_entry_index);
        uint64_t issuance_ratio();

        //exchanger
        void init_synthetix_exchanger();
        void settlement_owing(uint64_t account, uint64_t currency_key, uint64_t& reclaim_amount, uint64_t& rebat_amount);
        void calculate_amount_after_settlement(
            regid     from,
            uint64_t  currency_key,
            uint64_t  amount,
            uint64_t  refunded,
            uint64_t& amount_after_settlement);
        void calculate_exchange_amount_minus_fees(
            uint64_t  source_currency_key,
            uint64_t  destination_currency_key,
            uint64_t  destination_amount,
            uint64_t& amount_recived,
            uint64_t& fee);
         void reclaim(regid from, uint64_t currency_key, uint64_t amount);
         void refund(regid from, uint64_t currency_key, uint64_t amount);
         void get_round_ids_at_period_end(
             const exchange_entry_t &e, 
             uint64_t& src_round_id_at_period_end, 
             uint64_t& dest_round_id_at_period_end);

        //exchange rates
        void     init_synthetix_exchange_rates();
        uint64_t get_last_round_id_before_elapsed_secs(uint64_t currency_key, uint64_t starting_round_id, uint64_t starting_times_stamp, uint64_t time_diff);
        uint64_t effective_value_at_round(uint64_t source_currency_key, uint64_t source_amount, uint64_t destination_currency_key, uint64_t round_id_for_src, uint64_t round_id_for_dest);
        uint64_t effective_value(uint64_t source_currency_key, uint64_t source_amount, uint64_t destination_currency_key);
        uint64_t get_rate(uint64_t currency_key);
        uint64_t get_current_round_id(uint64_t currency_key);

        //exchange state
        void init_synthetix_exchange_state();
        void append_exchange_entry(
            uint64_t owner,
            uint64_t source_currency_key,     
            uint64_t source_amount,
            uint64_t destination_currency_key,
            uint64_t amount_recived,
            uint64_t exchange_fee_rate,
            uint64_t time_tamp,
            uint64_t round_id_for_src,
            uint64_t round_id_for_dest);
        uint64_t get_max_time_stamp(regid account, uint64_t currency_key);
        uint64_t get_max_entries_in_queue(); 
        void     remove_entries(uint64_t from, uint64_t currency_key);

        //feepool
        void init_synthetix_feepool();
        void record_fee_paid( uint64_t amount);
        void close_current_fee_period();//fixme:should be as an action
        void feepool_append_account_issuance_record( regid account, uint64_t debt_ratio, uint64_t debt_entry_index);  
        bool is_fees_claimable(regid account);   

        //feepool state
        void get_accounts_debt_entry(uint64_t account, uint64_t index, uint64_t& debt_percentage, uint64_t& debt_entry_index);
        void applicable_issuance_data(uint64_t account, uint64_t closing_debt_index, uint64_t& owner_ship_percentage, uint64_t& debt_entry_index);
        void feepool_state_append_account_issuance_record( uint64_t account, uint64_t debt_ratio, uint64_t debt_entry_index, uint64_t current_period_start_debt_index);

        //issuer
        void init_synthetix_issuer();


        //xt token state
        void init_synthetix_token_state();
        void notify_bank_to_burn_mpa( regid from, asset quantity);
        void notify_bank_to_issue_mpa( regid to, asset quantity);
        void issue_mpa( regid owner, asset quantity);
        void burn_mpa( regid owner, asset quantity);

    private:
        //synthetix
        bool     _available_currency_keys(map<uint64_t, symbol_code>& currency_keys);
        bool     _totoal_issue_mpas(bool exclude_ether_collateral, asset& total_synthetix_debt);
        uint64_t _available_mpa_count();
        uint64_t _debt_balance_of(regid issuer,uint64_t currency_key);
        uint64_t _collateral(regid account);

        //exchanger
        bool     _get_exchange_enabled();
        uint64_t _max_secs_left_in_waiting_period(regid account, uint64_t currency_key);  
        uint64_t _fee_rate_for_exchange(uint64_t source_currency_key, uint64_t destination_currency_key);
        void     _append_exchange(
            regid       destination_address,
            symbol_code source_currency_key,
            asset       amount,
            symbol_code destination_currency_key,
            asset       amount_received);
        void     _internal_settle(regid from, uint64_t currency_key, uint64_t reclaimed, uint64_t refunded);
        uint64_t _get_waiting_period_secs();
        uint64_t _secs_left_in_waiting_period_for_exchange(uint64_t time_stamp);
        void     _remit_fee(uint64_t fee, uint64_t currency_key);
        void     _set_last_fee_withdrawal(uint64_t claiming_address, uint64_t fee_period_id);
        uint64_t _get_last_fee_withdrawal(uint64_t claiming_address);
        uint64_t _get_fee_period_duration();
        uint64_t get_exchange_fee_rate();
        uint64_t _get_target_threshold();

        //exchange rates
        bool     _get_rate_and_timestamp_at_round(uint64_t currency_key, uint64_t round_id, uint64_t& rate, uint64_t& time);
        uint64_t _rate_for_currency(uint64_t currency_key);
        void     _rates_for_currencies(const vector<uint64_t>& currency_keys, map<uint64_t, uint64_t>& rates);
        void     _rates_and_stale_for_currencies(const vector<uint64_t>& currency_keys, map<uint64_t, uint64_t>& rates, bool& any_rate_stale);
        bool     _rate_is_stale(uint64_t currency_key);
        bool     _any_rate_is_stale(const vector<uint64_t>& currency_keys);
        void     _rate_not_stale(uint64_t currency_key);
        void     _set_rate(uint64_t currency_key, uint64_t new_rate, uint64_t time_sent);
        bool     _get_rate_and_updated_time(uint64_t currency_key, rate_and_updated_time_t& rate_and_updated_time);  
        uint64_t _last_rate_update_times(uint64_t currency_key);   
        void     _internal_update_rates(const std::vector<symbol_code>& currency_keys, const std::vector<uint64_t>& new_rates, uint64_t time_sent);                
        void     _last_rate_update_times_for_currencies(const std::vector<symbol_code>& currency_keys, std::vector<uint64_t> last_update_times);

        regid    _get_oracle();
        uint64_t _get_rate_stale_period();

        //feepool
        uint64_t _fees_by_period(regid account, vector<uint64_t> &user_fees, vector<uint64_t> &user_rewards);
        void     _claim_fees(regid claiming_address);
        void     _fee_and_rewards_from_period(
            uint64_t  period, 
            uint64_t  owner_ship_percentage, 
            uint64_t  debt_entry_index, 
            uint64_t& fees_from_period, 
            uint64_t& rewards_from_period);
        uint64_t _effective_debt_ratio_for_period(uint64_t closing_debt_index, uint64_t owner_ship_percentage, uint64_t debt_entry_index);
        void     _fees_available(regid account, uint64_t& available_fees, uint64_t& available_rewards);
        uint64_t _record_fee_payment(uint64_t xUSD_amount);

        fee_period_t         _get_recent_fee_period(uint64_t index);
        void                 _set_recent_fee_period(uint64_t index, const fee_period_t& fee_period);
        recent_fee_periods_t _get_recent_fee_periods();
        void                 _set_recent_fee_periods(const recent_fee_periods_t& fee_periods);

        //issuer
        void     _internal_Issue_mpas(regid from, uint64_t amount, uint64_t existing_debt, uint64_t total_system_debt);
        void     _internal_burn_mpas(regid from, uint64_t amount, uint64_t existing_debt, uint64_t total_system_debt);
        void     _add_to_debt_register(regid from, uint64_t amount, uint64_t existing_debt, uint64_t total_debt_issused);
        void     _remove_from_debt_register(regid from, uint64_t amount, uint64_t existing_debt, uint64_t total_debt_issused);
        void     _append_account_issuance_record(regid from);
        void     _burnmpas(regid from, uint64_t amount); 
        void     _burn_mpa_to_target(regid from);
        bool     _can_burn_mpas(regid account);
        void     _set_last_issue_event(regid account);
        uint64_t _last_issue_event(regid account);

        //state
        void     _set_current_issuance_data(regid account, uint64_t initial_debt_owner_ship);
        void     _append_debt_ledger_value(uint64_t value);
        uint64_t _last_debt_ledger_entry();
        uint64_t _debt_ledger(uint64_t index);

        //xt token state
        void     _transfer_normal( regid from, regid to, asset quantity, string  memo );
        void     _sub_balance( regid owner, asset value );
        void     _add_balance( regid owner, asset value, regid ram_payer );
        uint64_t _token_state_balance_of( regid owner, uint64_t symbol_code );

        //storage
        uint64_t get_uint_value(const name& key);
        void     set_uint_value(const name& key, uint64_t value);
        void     delete_uint_value(const name& key); 

        uint64_t get_u128_value(const uint128_t& key);
        void     set_u128_value(const uint128_t& key, uint64_t value);
        void     delete_u128_value(const uint128_t& key); 

    public:
        //synthetix
        ACTION initsynthetix();
        ACTION addmpa(mpa_t mpa);
        ACTION delmpa(symbol_code curreny_key);
        ACTION totalmpas();

        //exchanger
        ACTION setexenabled(bool exchange_enabled);
        ACTION setwaitsecs(uint64_t waiting_period_secs);
        ACTION exchange(
            regid       from,
            symbol_code source_currency_key,     
            uint64_t    source_amount,
            symbol_code destination_currency_key,
            regid       destination_address);   
        ACTION settle(regid from, symbol_code currency_key);

        //exchange state
        ACTION setmaxqueue(uint64_t max_entries_in_queue);

        //feelpool
        ACTION setfeerate(uint64_t exchange_fee_rate);
        ACTION setfeeperiod(uint64_t fee_period_duration);
        ACTION setthreshold(uint64_t target_threshold_percent) ;
        ACTION claimfees(regid claiming_address);
        ACTION appendreward( uint64_t amount);

        //exchange rates
        ACTION setoracle(regid oracle);
        ACTION setperiod(uint64_t period);
        ACTION updaterates(const std::vector<symbol_code>& currency_keys, const std::vector<uint64_t>& new_rates);

        //issuer
        ACTION setstaketime(uint64_t seconds);
        ACTION issuempas(regid from, uint64_t amount);
        ACTION issuemaxmpas(regid from );
        ACTION burnmpas(regid from, asset quantity);
        ACTION burnmpas2t(regid from);

        //synthetix state
        ACTION setissuratio(uint64_t issuance_ratio);  

        //xt token
        ACTION create( regid  issuer, asset  maximum_supply );
        ACTION issue( regid to, asset quantity, string memo );
        ACTION transfer(regid from, regid to, asset quantity, string  memo ); 

};
