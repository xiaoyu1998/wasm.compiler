#pragma once

#include "../core/datastream.hpp"
#include "../core/name.hpp"
#include "../core/database.h"

#include <vector>
#include <memory>

using namespace std;
using namespace wasm;

namespace wasm {

constexpr static inline name no_payer{};

template<name::raw TableName, typename T, typename PrimaryKeyType>
class table
{
public:
	table( name code, uint64_t scope )
	:_code(code),_scope(scope)
	//:_scope(scope)
	{}

	template<typename Lambda>
	void emplace( name payer, Lambda&& constructor ) {

		T obj;
		constructor(obj);

		//vector<char> key = pack(std::tuple(_code, TableName, _scope, obj.primary_key()));
		vector<char> key = pack(std::tuple(TableName, _scope, obj.primary_key()));
		uint32_t key_len = key.size();

		vector<char> value = pack(obj);
		uint32_t value_len = value.size();  

		db_store(payer.value, key.data(), key_len, value.data(), value_len);

	}

	template<typename Lambda>
	void modify( const T& obj, name payer, Lambda&& updater ) {

		auto& mutableobj = const_cast<T&>(obj); // Do not forget the auto& otherwise it would make a copy and thus not update at all.
		updater( mutableobj );

	    //vector<char> key = pack(std::tuple(_code, TableName, _scope, obj.primary_key()));
	    vector<char> key = pack(std::tuple(TableName, _scope, obj.primary_key()));
		uint32_t key_len = key.size();

		vector<char> value = pack(obj);
		uint32_t value_len = value.size();  

		db_update(payer.value, key.data(), key_len, value.data(), value_len);     
	}

	void erase( const T& obj, name payer) {

	    erase(obj.primary_key(), payer);
	}

	void erase( const PrimaryKeyType& primary, name payer ) {

        //vector<char> key = pack(std::tuple(_code, TableName, _scope, primary));
        vector<char> key = pack(std::tuple(TableName, _scope, primary));
	    uint32_t key_len = key.size();
	    db_remove(payer.value, key.data(), key_len);

	}

	bool get(T& t, const PrimaryKeyType& primary, const char* error_msg = "unable to find key" ) {

        //vector<char> key = pack(std::tuple(_code, TableName, _scope, primary));
        vector<char> key = pack(std::tuple(TableName, _scope, primary));
		uint32_t key_len = key.size();

		auto value_len = db_get(key.data(), key_len, NULL, 0);

		if(value_len == 0){
			return false;
		}

		auto v = (char*)malloc(sizeof(char) * value_len);
		db_get( key.data(), key_len, v, value_len);
		t = unpack<T>(v, value_len);
		free(v);

		return true;
	}

private:
	name     _code;
    uint64_t _scope;

};
}