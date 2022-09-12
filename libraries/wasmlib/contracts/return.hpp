
// fixme:V4 support
namespace wasm {
   extern "C" {              

     __attribute__((wasm_wasm_import))
     void set_return(const void* data, uint32_t data_len);

     __attribute__((wasm_wasm_import))
     uint64_t get_return(const void* data, uint32_t data_len);

   }

  void set_return(const vector<char>& bytes){
      set_return((void*)bytes.data(), bytes.size());
  }

  vector<char> get_return(uint32_t size){
      vector<char> value(size);
      get_return((void*)value.data(), size);
      return value;
  }

} // namespace wasm
