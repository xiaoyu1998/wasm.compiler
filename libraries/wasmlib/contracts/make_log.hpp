
// fixme:V4 support
namespace wasm {
   extern "C" {              
     __attribute__((wasm_wasm_import))
     void make_log( uint64_t payer, uint64_t receiver, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len );
   }

  void make_log(regid owner, const string& topic, const vector<char>& value){
      make_log(0, owner.value, topic.data(), topic.size(), value.data(), value.size());
  }


} // namespace wasm
