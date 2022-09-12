project(synthetix)
cmake_minimum_required(VERSION 3.5)

set(WASM_WASM_OLD_BEHAVIOR "Off")
find_package(wasm.cdt)

add_contract( synthetix synthetix 
	synthetix.cpp 
	synthetix_issuer.cpp 
	synthetix_state.cpp 
	synthetix_token_state.cpp
	synthetix_exchanger.cpp
	synthetix_exchange_rates.cpp 
	synthetix_exchange_state.cpp 
	synthetix_feepool.cpp
	synthetix_feepool_state.cpp
	synthetix_unit_storage.cpp )
target_include_directories( synthetix PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( synthetix ${CMAKE_SOURCE_DIR}/../ricardian )
