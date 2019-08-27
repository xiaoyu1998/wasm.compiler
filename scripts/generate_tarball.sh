#! /bin/bash

NAME=$1
CDT_PREFIX=${PREFIX}/${SUBPREFIX}
mkdir -p ${PREFIX}/bin/
mkdir -p ${PREFIX}/lib/cmake/${PROJECT}
mkdir -p ${CDT_PREFIX}/bin 
mkdir -p ${CDT_PREFIX}/include
mkdir -p ${CDT_PREFIX}/lib/cmake/${PROJECT}
mkdir -p ${CDT_PREFIX}/cmake
mkdir -p ${CDT_PREFIX}/scripts
mkdir -p ${CDT_PREFIX}/licenses

#echo "${PREFIX} ** ${SUBPREFIX} ** ${CDT_PREFIX}"

# install binaries 
cp -R ${BUILD_DIR}/bin/* ${CDT_PREFIX}/bin 
cp -R ${BUILD_DIR}/licenses/* ${CDT_PREFIX}/licenses

# install cmake modules
sed "s/_PREFIX_/\/${SPREFIX}/g" ${BUILD_DIR}/modules/WasmCDTMacrosPackage.cmake &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/WasmCDTMacros.cmake
sed "s/_PREFIX_/\/${SPREFIX}/g" ${BUILD_DIR}/modules/WasmWasmToolchainPackage.cmake &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/WasmWasmToolchain.cmake
sed "s/_PREFIX_/\/${SPREFIX}\/${SSUBPREFIX}/g" ${BUILD_DIR}/modules/${PROJECT}-config.cmake.package &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake

# install scripts
cp -R ${BUILD_DIR}/scripts/* ${CDT_PREFIX}/scripts 

# install misc.
cp ${BUILD_DIR}/wasm.imports ${CDT_PREFIX}

# install wasm includes
cp -R ${BUILD_DIR}/include/* ${CDT_PREFIX}/include

# install wasm libs
cp ${BUILD_DIR}/lib/*.a ${CDT_PREFIX}/lib

# make symlinks
pushd ${PREFIX}/lib/cmake/${PROJECT} &> /dev/null
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake ${PROJECT}-config.cmake
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/WasmWasmToolchain.cmake WasmWasmToolchain.cmake
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/WasmCDTMacros.cmake WasmCDTMacros.cmake
popd &> /dev/null

create_symlink() {
   pushd ${PREFIX}/bin &> /dev/null
   ln -sf ../${SUBPREFIX}/bin/$1 $2
   popd &> /dev/null
}

create_symlink "wasm-cc wasm-cc"
create_symlink "wasm-cpp wasm-cpp"
create_symlink "wasm-lld wasm-lld"
create_symlink "wasm2wast wasm2wast"
create_symlink "wast2wasm wast2wasm"

# create_symlink "eosio-cc eosio-cc"
# create_symlink "eosio-cpp eosio-cpp"
# create_symlink "eosio-ld eosio-ld"
# create_symlink "eosio-pp eosio-pp"
# create_symlink "eosio-init eosio-init"
# create_symlink "eosio-abigen eosio-abigen"
# create_symlink "eosio-wasm2wast eosio-wasm2wast"
# create_symlink "eosio-wast2wasm eosio-wast2wasm"

tar -cvzf $NAME ./${PREFIX}/*
rm -r ${PREFIX}
