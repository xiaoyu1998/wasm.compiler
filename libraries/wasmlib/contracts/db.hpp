#pragma once

#include "../core/datastream.hpp"
#include "../core/name.hpp"
#include "../core/database.h"
#include "../core/wasm_log.hpp"
#include "../core/strings.hpp"

#include <vector>
#include <memory>
#include <boost/hana.hpp>

using namespace std;
using namespace wasm;
namespace hana = boost::hana;

namespace wasm { namespace db {

    #define WASM_DB_TABLE_TYPE_DEFINITION(T) \
    typedef T table_type;

    constexpr static inline regid payer_reserved{};

    //for debug before gettable updated
    constexpr static uint64_t SECONDARY_KEY_PREFIX = 0x000000000000000FULL;
    // constexpr static uint64_t PRIMARY_KEY_PREFIX   = 0x00;
    // constexpr static uint64_t SECONDARY_KEY_PREFIX = 0x01;

    template<class Class, typename Type, Type (Class::*PtrToMemberFunction)()const>
    struct const_mem_fun
    {
        typedef typename std::remove_reference<Type>::type result_type;

        template<typename ChainedPtr>
        auto operator()(const ChainedPtr& x)const -> std::enable_if_t<!std::is_convertible<const ChainedPtr&, const Class&>::value, Type>
        {
            return operator()(*x);
        }

        Type operator()(const Class& x) const
        {
            return (x.*PtrToMemberFunction)();
        }

        Type operator()(const std::reference_wrapper<const Class>& x)const
        {
            return operator()(x.get());
        }

        Type operator()(const std::reference_wrapper<Class>& x)const
        {
            return operator()(x.get());
        }
    };

    template<name::raw IndexName, typename Extractor>
    struct indexed_by {
       enum constants { index_name   = static_cast<uint64_t>(IndexName) };
       typedef Extractor secondary_extractor_type;
    };

    template<name::raw TableName, typename T, typename PrimaryKeyType, typename... Indices>
    struct index_table {

    public:
        template<name::raw IndexName, typename Extractor, uint64_t Number, bool IsConst>
        struct index {
             public:
                typedef Extractor  secondary_extractor_type;
                typedef typename std::decay<decltype( Extractor()(nullptr) )>::type secondary_key_type;

                constexpr static bool validate_index_name( wasm::name n ) {
                   return n.value != 0 && n != wasm::name("primary"); // Primary is a reserve index name.
                }

                static_assert( validate_index_name( wasm::name(IndexName) ), "invalid index name used in index_table" );

                enum constants {
                   table_name   = static_cast<uint64_t>(TableName),
                   index_name   = static_cast<uint64_t>(IndexName),
                   index_number = Number,
                   index_table_name = (static_cast<uint64_t>(TableName) & 0xFFFFFFFFFFFFFFF0ULL)
                                        | (Number & 0x000000000000000FULL) // Assuming no more than 16 secondary indices are allowed
                };

                constexpr static uint64_t name()   { return index_table_name; }
                constexpr static uint64_t number() { return Number; }

                static auto extract_secondary_key(const T& obj) { return secondary_extractor_type()(obj); }
             private:
                friend class index_table;

                index( typename std::conditional<IsConst, const index_table*, index_table*>::type midx )
                :_idx_table(midx){}

                typename std::conditional<IsConst, const index_table*, index_table*>::type _idx_table;
        }; /// struct index_table::index

        template<uint64_t I>
        struct intc { enum e{ value = I }; operator uint64_t()const{ return I; }  };

        static constexpr auto transform_indices( ) {

            typedef decltype( hana::zip_shortest(
                             hana::make_tuple( intc<0>(), intc<1>(), intc<2>(), intc<3>(), intc<4>(), intc<5>(),
                                               intc<6>(), intc<7>(), intc<8>(), intc<9>(), intc<10>(), intc<11>(),
                                               intc<12>(), intc<13>(), intc<14>(), intc<15>() ),
                             hana::tuple<Indices...>() ) ) indices_input_type;

            return hana::transform( indices_input_type(), [&]( auto&& idx ){
                typedef typename std::decay<decltype(hana::at_c<0>(idx))>::type num_type;
                typedef typename std::decay<decltype(hana::at_c<1>(idx))>::type idx_type;
                return hana::make_tuple( hana::type_c<index<wasm::name::raw(static_cast<uint64_t>(idx_type::index_name)),
                                                         typename idx_type::secondary_extractor_type,
                                                         num_type::e::value, false> >,
                                         hana::type_c<index<wasm::name::raw(static_cast<uint64_t>(idx_type::index_name)),
                                                             typename idx_type::secondary_extractor_type,
                                                             num_type::e::value, true> > );

            });
        }

        typedef decltype( index_table::transform_indices() ) indices_type;
        indices_type _indices;

      public:
        typedef T object_type;
        typedef PrimaryKeyType primary_key_type;

      public:
         name::raw table_name() const {
         	return TableName;
         }
    };

    template<typename ObjectType>
    bool get(ObjectType& object) {  

        typename ObjectType::table_type table;

        //vector<char> key = pack(std::tuple(PRIMARY_KEY_PREFIX, table.table_name(), object.primary_key()));
        vector<char> key = pack(std::tuple(table.table_name(), object.primary_key()));
		uint32_t key_len = key.size();

		auto value_len = db_get(key.data(), key_len, NULL, 0);
		if(value_len == 0){ return false; }

		auto v = (char*)malloc(sizeof(char) * value_len);
		db_get( key.data(), key_len, v, value_len);
		object = unpack<ObjectType>(v, value_len);
		free(v);
		return true;
    }

    template<typename ObjectType, typename PrimaryKeyType>
    bool get(ObjectType& object, const PrimaryKeyType& primary) {  

        typename ObjectType::table_type table;

        //vector<char> key = pack(std::tuple(PRIMARY_KEY_PREFIX, table.table_name(), primary));
        vector<char> key = pack(std::tuple(table.table_name(), primary));
        uint32_t key_len = key.size();

        auto value_len = db_get(key.data(), key_len, NULL, 0);
        if(value_len == 0){ return false; }

        auto v = (char*)malloc(sizeof(char) * value_len);
        db_get( key.data(), key_len, v, value_len);
        object = unpack<ObjectType>(v, value_len);
        free(v);
        return true;
    }

    template<typename ObjectType>
    void set(const ObjectType& object) {

        typename ObjectType::table_type table;

        //vector<char> key   = pack(std::tuple(PRIMARY_KEY_PREFIX,table.table_name(), object.primary_key()));
        vector<char> key   = pack(std::tuple(table.table_name(), object.primary_key()));
		vector<char> value = pack(object);

        ObjectType old_object;
        bool has_old_object   = wasm::db::get(old_object, object.primary_key());

        hana::for_each( table._indices, [&]( auto& idx ) {
            typedef typename decltype(+hana::at_c<0>(idx))::type index_type;
            if(has_old_object && index_type::extract_secondary_key(old_object) != index_type::extract_secondary_key(object))
            {
                auto old_secondary = pack(std::tuple(SECONDARY_KEY_PREFIX, index_type::name(), index_type::extract_secondary_key(old_object)));
                db_remove(wasm::db::payer_reserved.value, old_secondary.data(), old_secondary.size());
            }

            auto secondary = pack(std::tuple(SECONDARY_KEY_PREFIX, index_type::name(), index_type::extract_secondary_key(object)));
            auto primary   = pack(object.primary_key());

            db_store(payer_reserved.value, secondary.data(), secondary.size(), primary.data(), primary.size());
        }); 


        db_store(payer_reserved.value, key.data(), key.size(), value.data(), value.size());      

    }

    template<typename ObjectType>
    void del(const ObjectType& object) {

        typename ObjectType::table_type table;

        //vector<char> key = pack(std::tuple(PRIMARY_KEY_PREFIX, table.table_name(), object.primary_key()));
        vector<char> key = pack(std::tuple( table.table_name(), object.primary_key()));

        // bool has_object = wasm::db::get(object);
        // if(has_object){
            hana::for_each( table._indices, [&]( auto& idx ) {
                typedef typename decltype(+hana::at_c<0>(idx))::type index_type;
                vector<char> secondary = pack(std::tuple(SECONDARY_KEY_PREFIX, index_type::name(), index_type::extract_secondary_key(object)));
                db_remove(payer_reserved.value, secondary.data(), secondary.size());
            }); 
            db_remove(payer_reserved.value, key.data(), key.size());
        //}

    }

    template<typename ObjectType>
    bool get(uint64_t by_index, ObjectType& object, const char* error_msg = "unable to find key" ) {

         //WASM_LOG_FPRINT(true, "by_index:%", wasm::name(by_index))
        typedef typename ObjectType::table_type table_type;
        typename table_type::primary_key_type primary;
        table_type table;

        bool has_primary = false;
        hana::for_each( table._indices, [&]( auto& idx ) {
           typedef typename decltype(+hana::at_c<0>(idx))::type index_type;
           if(index_type::index_name == by_index)
           {
               //auto secondary   = pack(std::tuple(SECONDARY_KEY_PREFIX, table.table_name(), object.scope(), index_type::name(), index_type::extract_secondary_key(object)));
               auto secondary   = pack(std::tuple(SECONDARY_KEY_PREFIX, index_type::name(), index_type::extract_secondary_key(object)));
               auto primary_len = db_get(secondary.data(), secondary.size(), NULL, 0);

               //WASM_LOG_FPRINT(true, "by_index:% secondary_key:%", wasm::name(by_index),  index_type::extract_secondary_key(object))

               if(primary_len == 0) {return;}
               
               auto v = (char*)malloc(sizeof(char) * primary_len);
               db_get( secondary.data(), secondary.size(), v, primary_len);
               primary = unpack<typename table_type::primary_key_type>(v, primary_len);
               free(v);  

               //WASM_LOG_FPRINT(true, "by_index:% secondary_key:% primary:%", wasm::name(by_index),  index_type::extract_secondary_key(object), primary)
               has_primary = true;
               return;
            }

        });

        if(!has_primary) return false;

        return get(object, primary);

    }


}}//db//wasm

  