#include <wasm.hpp>

using namespace wasm;
namespace wasm {

CONTRACT dispatch : contract{
   	 public:
   		 using contract::contract;
       uint32_t balance1;
       uint32_t balance2;

      public:

         [[wasm::action]] 
         void hi( name nm );
         
         [[wasm::action]] 
         void prints(uint32_t b1, uint32_t b2);

      public:
          [[wasm::on_notify("57194-2::transfer")]] 
          void on_transfer(regid from, regid to, asset quant, std::string memo);

          [[wasm::on_notify("1-800::transfer1")]] 
          void on_transfer1(regid from, regid to, asset quant, std::string memo);

          [[wasm::on_notify("1-800::transfer2")]] 
          void on_transfer2(regid from, regid to, asset quant, std::string memo);

          [[wasm::on_notify("1-800::transfer3")]] 
          void on_transfer3(regid from, regid to, asset quant, std::string memo);

          [[wasm::on_notify("1-900::transfer4")]] 
          void on_transfer4(regid from, regid to, asset quant, std::string memo);

          [[wasm::on_notify("*::transfer")]] 
          void on_transfer5(regid from, regid to, asset quant, std::string memo);


   };
} /// namespace wasm
