#pragma once
#include <vector>
#include "../core/regid.hpp"

namespace wasm {


   /**
    *  Returns back the list of active producer names.
    *
    *  @ingroup producer_schedule
    */
   std::vector<regid> get_active_producers();

} /// namespace wasm
