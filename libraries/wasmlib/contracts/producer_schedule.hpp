#pragma once
#include <vector>
#include "../core/name.hpp"

namespace wasm {


   /**
    *  Returns back the list of active producer names.
    *
    *  @ingroup producer_schedule
    */
   std::vector<name> get_active_producers();

} /// namespace wasm
