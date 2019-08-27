#! /bin/bash

binaries=(wasm-ranlib
          wasm-ar
          wasm-cc
          wasm-cpp
          wasm-lld
          wasm2wast
          wast2wasm)

if [ -d "/usr/local/wasm.cdt" ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            pushd /usr/local &> /dev/null
            rm -rf wasm.cdt
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            pushd lib/cmake &> /dev/null
            rm -rf wasm.cdt
            popd &> /dev/null
            break;;
         [Nn]* ) 
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi

if [ -d "/usr/local/wasm.wasmsdk" ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            pushd /usr/local &> /dev/null
            rm -rf wasm.wasmsdk
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            break;;

         [Nn]* ) 
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
