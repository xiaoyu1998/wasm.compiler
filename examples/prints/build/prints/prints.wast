(module
  (type (;0;) (func))
  (type (;1;) (func (param i32)))
  (type (;2;) (func (param i32 i32 i32) (result i32)))
  (type (;3;) (func (param i32 i32)))
  (type (;4;) (func (result i32)))
  (type (;5;) (func (param i32 i32) (result i32)))
  (type (;6;) (func (param i32) (result i32)))
  (type (;7;) (func (param i64 i64 i64)))
  (type (;8;) (func (param i64 i64 i32) (result i32)))
  (import "env" "memcpy" (func (;0;) (type 2)))
  (import "env" "printhex" (func (;1;) (type 3)))
  (import "env" "abort" (func (;2;) (type 0)))
  (import "env" "wasm_assert" (func (;3;) (type 3)))
  (import "env" "memset" (func (;4;) (type 2)))
  (import "env" "action_data_size" (func (;5;) (type 4)))
  (import "env" "read_action_data" (func (;6;) (type 5)))
  (func (;7;) (type 0)
    call 10)
  (func (;8;) (type 6) (param i32) (result i32)
    (local i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          block  ;; label = @4
            get_local 0
            i32.eqz
            br_if 0 (;@4;)
            i32.const 0
            i32.const 0
            i32.load offset=8204
            get_local 0
            i32.const 16
            i32.shr_u
            tee_local 1
            i32.add
            tee_local 2
            i32.store offset=8204
            i32.const 0
            i32.const 0
            i32.load offset=8196
            tee_local 3
            get_local 0
            i32.add
            i32.const 7
            i32.add
            i32.const -8
            i32.and
            tee_local 0
            i32.store offset=8196
            get_local 2
            i32.const 16
            i32.shl
            get_local 0
            i32.le_u
            br_if 1 (;@3;)
            get_local 1
            memory.grow
            i32.const -1
            i32.eq
            br_if 2 (;@2;)
            br 3 (;@1;)
          end
          i32.const 0
          return
        end
        i32.const 0
        get_local 2
        i32.const 1
        i32.add
        i32.store offset=8204
        get_local 1
        i32.const 1
        i32.add
        memory.grow
        i32.const -1
        i32.ne
        br_if 1 (;@1;)
      end
      i32.const 0
      i32.const 8220
      call 3
      get_local 3
      return
    end
    get_local 3)
  (func (;9;) (type 1) (param i32))
  (func (;10;) (type 0)
    (local i32)
    get_global 0
    i32.const 16
    i32.sub
    tee_local 0
    i32.const 0
    i32.store offset=12
    i32.const 0
    get_local 0
    i32.load offset=12
    i32.load
    i32.const 7
    i32.add
    i32.const -8
    i32.and
    tee_local 0
    i32.store offset=8196
    i32.const 0
    get_local 0
    i32.store offset=8192
    i32.const 0
    memory.size
    i32.store offset=8204)
  (func (;11;) (type 4) (result i32)
    i32.const 8208)
  (func (;12;) (type 5) (param i32 i32) (result i32)
    (local i32 i32 i32 i32 i32 i32)
    i32.const 0
    set_local 2
    block  ;; label = @1
      block  ;; label = @2
        i32.const 0
        get_local 0
        i32.sub
        tee_local 3
        get_local 0
        i32.and
        get_local 0
        i32.ne
        br_if 0 (;@2;)
        get_local 0
        i32.const 16
        i32.gt_u
        br_if 1 (;@1;)
        get_local 1
        call 8
        return
      end
      call 11
      i32.const 22
      i32.store
      i32.const 0
      return
    end
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          get_local 0
          i32.const -1
          i32.add
          tee_local 4
          get_local 1
          i32.add
          call 8
          tee_local 0
          i32.eqz
          br_if 0 (;@3;)
          get_local 0
          get_local 4
          get_local 0
          i32.add
          get_local 3
          i32.and
          tee_local 2
          i32.eq
          br_if 1 (;@2;)
          get_local 0
          i32.const -4
          i32.add
          tee_local 3
          i32.load
          tee_local 4
          i32.const 7
          i32.and
          tee_local 1
          i32.eqz
          br_if 2 (;@1;)
          get_local 0
          get_local 4
          i32.const -8
          i32.and
          i32.add
          tee_local 4
          i32.const -8
          i32.add
          tee_local 5
          i32.load
          set_local 6
          get_local 3
          get_local 1
          get_local 2
          get_local 0
          i32.sub
          tee_local 7
          i32.or
          i32.store
          get_local 2
          i32.const -4
          i32.add
          get_local 4
          get_local 2
          i32.sub
          tee_local 3
          get_local 1
          i32.or
          i32.store
          get_local 2
          i32.const -8
          i32.add
          get_local 6
          i32.const 7
          i32.and
          tee_local 1
          get_local 7
          i32.or
          i32.store
          get_local 5
          get_local 1
          get_local 3
          i32.or
          i32.store
          get_local 0
          call 9
        end
        get_local 2
        return
      end
      get_local 0
      return
    end
    get_local 2
    i32.const -8
    i32.add
    get_local 0
    i32.const -8
    i32.add
    i32.load
    get_local 2
    get_local 0
    i32.sub
    tee_local 0
    i32.add
    i32.store
    get_local 2
    i32.const -4
    i32.add
    get_local 3
    i32.load
    get_local 0
    i32.sub
    i32.store
    get_local 2)
  (func (;13;) (type 2) (param i32 i32 i32) (result i32)
    (local i32)
    i32.const 22
    set_local 3
    block  ;; label = @1
      block  ;; label = @2
        get_local 1
        i32.const 4
        i32.lt_u
        br_if 0 (;@2;)
        get_local 1
        get_local 2
        call 12
        tee_local 1
        i32.eqz
        br_if 1 (;@1;)
        get_local 0
        get_local 1
        i32.store
        i32.const 0
        set_local 3
      end
      get_local 3
      return
    end
    call 11
    i32.load)
  (func (;14;) (type 6) (param i32) (result i32)
    (local i32 i32)
    block  ;; label = @1
      get_local 0
      i32.const 1
      get_local 0
      select
      tee_local 1
      call 8
      tee_local 0
      br_if 0 (;@1;)
      loop  ;; label = @2
        i32.const 0
        set_local 0
        i32.const 0
        i32.load offset=8216
        tee_local 2
        i32.eqz
        br_if 1 (;@1;)
        get_local 2
        call_indirect (type 0)
        get_local 1
        call 8
        tee_local 0
        i32.eqz
        br_if 0 (;@2;)
      end
    end
    get_local 0)
  (func (;15;) (type 6) (param i32) (result i32)
    get_local 0
    call 14)
  (func (;16;) (type 1) (param i32)
    block  ;; label = @1
      get_local 0
      i32.eqz
      br_if 0 (;@1;)
      get_local 0
      call 9
    end)
  (func (;17;) (type 1) (param i32)
    get_local 0
    call 16)
  (func (;18;) (type 5) (param i32 i32) (result i32)
    (local i32 i32)
    get_global 0
    i32.const 16
    i32.sub
    tee_local 2
    set_global 0
    block  ;; label = @1
      get_local 2
      i32.const 12
      i32.add
      get_local 1
      i32.const 4
      get_local 1
      i32.const 4
      i32.gt_u
      select
      tee_local 1
      get_local 0
      i32.const 1
      get_local 0
      select
      tee_local 3
      call 13
      i32.eqz
      br_if 0 (;@1;)
      block  ;; label = @2
        loop  ;; label = @3
          i32.const 0
          i32.load offset=8216
          tee_local 0
          i32.eqz
          br_if 1 (;@2;)
          get_local 0
          call_indirect (type 0)
          get_local 2
          i32.const 12
          i32.add
          get_local 1
          get_local 3
          call 13
          br_if 0 (;@3;)
          br 2 (;@1;)
        end
      end
      get_local 2
      i32.const 0
      i32.store offset=12
    end
    get_local 2
    i32.load offset=12
    set_local 0
    get_local 2
    i32.const 16
    i32.add
    set_global 0
    get_local 0)
  (func (;19;) (type 5) (param i32 i32) (result i32)
    get_local 0
    get_local 1
    call 18)
  (func (;20;) (type 3) (param i32 i32)
    block  ;; label = @1
      get_local 0
      i32.eqz
      br_if 0 (;@1;)
      get_local 0
      call 9
    end)
  (func (;21;) (type 3) (param i32 i32)
    get_local 0
    get_local 1
    call 20)
  (func (;22;) (type 1) (param i32))
  (func (;23;) (type 1) (param i32)
    call 2
    unreachable)
  (func (;24;) (type 1) (param i32)
    (local i32 i32)
    get_global 0
    i32.const 32
    i32.sub
    tee_local 1
    set_global 0
    get_local 1
    i64.const 1234605616436508552
    i64.store offset=24
    get_local 1
    i32.const 0
    i32.store offset=16
    get_local 1
    i64.const 0
    i64.store offset=8
    get_local 1
    i32.const 8
    i32.add
    i32.const 8
    call 25
    get_local 1
    i32.load offset=8
    get_local 1
    i32.const 24
    i32.add
    i32.const 8
    call 0
    drop
    get_local 1
    i32.load offset=8
    tee_local 2
    get_local 1
    i32.load offset=12
    get_local 2
    i32.sub
    call 1
    block  ;; label = @1
      get_local 1
      i32.load offset=8
      tee_local 2
      i32.eqz
      br_if 0 (;@1;)
      get_local 1
      get_local 2
      i32.store offset=12
      get_local 2
      call 16
    end
    get_local 1
    i32.const 32
    i32.add
    set_global 0)
  (func (;25;) (type 3) (param i32 i32)
    (local i32 i32 i32 i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          block  ;; label = @4
            block  ;; label = @5
              get_local 0
              i32.load offset=8
              tee_local 2
              get_local 0
              i32.load offset=4
              tee_local 3
              i32.sub
              get_local 1
              i32.ge_u
              br_if 0 (;@5;)
              get_local 3
              get_local 0
              i32.load
              tee_local 4
              i32.sub
              tee_local 5
              get_local 1
              i32.add
              tee_local 6
              i32.const -1
              i32.le_s
              br_if 2 (;@3;)
              i32.const 2147483647
              set_local 7
              block  ;; label = @6
                get_local 2
                get_local 4
                i32.sub
                tee_local 2
                i32.const 1073741822
                i32.gt_u
                br_if 0 (;@6;)
                get_local 6
                get_local 2
                i32.const 1
                i32.shl
                tee_local 2
                get_local 2
                get_local 6
                i32.lt_u
                select
                tee_local 7
                i32.eqz
                br_if 2 (;@4;)
              end
              get_local 7
              call 14
              set_local 2
              br 3 (;@2;)
            end
            get_local 0
            i32.const 4
            i32.add
            set_local 0
            loop  ;; label = @5
              get_local 3
              i32.const 0
              i32.store8
              get_local 0
              get_local 0
              i32.load
              i32.const 1
              i32.add
              tee_local 3
              i32.store
              get_local 1
              i32.const -1
              i32.add
              tee_local 1
              br_if 0 (;@5;)
              br 4 (;@1;)
            end
          end
          i32.const 0
          set_local 7
          i32.const 0
          set_local 2
          br 1 (;@2;)
        end
        get_local 0
        call 23
        unreachable
      end
      get_local 2
      get_local 7
      i32.add
      set_local 7
      get_local 3
      get_local 1
      i32.add
      get_local 4
      i32.sub
      set_local 4
      get_local 2
      get_local 5
      i32.add
      tee_local 5
      set_local 3
      loop  ;; label = @2
        get_local 3
        i32.const 0
        i32.store8
        get_local 3
        i32.const 1
        i32.add
        set_local 3
        get_local 1
        i32.const -1
        i32.add
        tee_local 1
        br_if 0 (;@2;)
      end
      get_local 2
      get_local 4
      i32.add
      set_local 4
      get_local 5
      get_local 0
      i32.const 4
      i32.add
      tee_local 6
      i32.load
      get_local 0
      i32.load
      tee_local 1
      i32.sub
      tee_local 3
      i32.sub
      set_local 2
      block  ;; label = @2
        get_local 3
        i32.const 1
        i32.lt_s
        br_if 0 (;@2;)
        get_local 2
        get_local 1
        get_local 3
        call 0
        drop
        get_local 0
        i32.load
        set_local 1
      end
      get_local 0
      get_local 2
      i32.store
      get_local 6
      get_local 4
      i32.store
      get_local 0
      i32.const 8
      i32.add
      get_local 7
      i32.store
      get_local 1
      i32.eqz
      br_if 0 (;@1;)
      get_local 1
      call 16
      return
    end)
  (func (;26;) (type 7) (param i64 i64 i64)
    (local i32)
    get_global 0
    i32.const 16
    i32.sub
    tee_local 3
    set_global 0
    call 7
    block  ;; label = @1
      get_local 1
      get_local 0
      i64.ne
      br_if 0 (;@1;)
      get_local 2
      i64.const -5918507477291130368
      i64.ne
      br_if 0 (;@1;)
      get_local 3
      i32.const 0
      i32.store offset=12
      get_local 3
      i32.const 1
      i32.store offset=8
      get_local 3
      get_local 3
      i64.load offset=8
      i64.store
      get_local 1
      get_local 1
      get_local 3
      call 27
      drop
    end
    i32.const 0
    call 22
    get_local 3
    i32.const 16
    i32.add
    set_global 0)
  (func (;27;) (type 8) (param i64 i64 i32) (result i32)
    (local i32 i32 i32 i32 i32)
    get_global 0
    i32.const 32
    i32.sub
    tee_local 3
    set_local 4
    get_local 3
    set_global 0
    get_local 2
    i32.load offset=4
    set_local 5
    get_local 2
    i32.load
    set_local 6
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          block  ;; label = @4
            call 5
            tee_local 7
            i32.eqz
            br_if 0 (;@4;)
            get_local 7
            i32.const 513
            i32.lt_u
            br_if 1 (;@3;)
            get_local 7
            call 8
            set_local 2
            br 2 (;@2;)
          end
          i32.const 0
          set_local 2
          br 2 (;@1;)
        end
        get_local 3
        get_local 7
        i32.const 15
        i32.add
        i32.const -16
        i32.and
        i32.sub
        tee_local 2
        set_global 0
      end
      get_local 2
      get_local 7
      call 6
      drop
    end
    get_local 4
    i32.const 20
    i32.add
    get_local 2
    i32.store
    get_local 4
    i32.const 24
    i32.add
    get_local 2
    get_local 7
    i32.add
    i32.store
    get_local 4
    get_local 1
    i64.store offset=8
    get_local 4
    get_local 0
    i64.store
    get_local 4
    get_local 2
    i32.store offset=16
    get_local 4
    get_local 5
    i32.const 1
    i32.shr_s
    i32.add
    set_local 3
    block  ;; label = @1
      get_local 5
      i32.const 1
      i32.and
      i32.eqz
      br_if 0 (;@1;)
      get_local 3
      i32.load
      get_local 6
      i32.add
      i32.load
      set_local 6
    end
    get_local 3
    get_local 6
    call_indirect (type 1)
    block  ;; label = @1
      get_local 7
      i32.const 513
      i32.lt_u
      br_if 0 (;@1;)
      get_local 2
      call 9
    end
    get_local 4
    i32.const 32
    i32.add
    set_global 0
    i32.const 1)
  (table (;0;) 2 2 anyfunc)
  (memory (;0;) 1)
  (global (;0;) (mut i32) (i32.const 8192))
  (global (;1;) i32 (i32.const 8245))
  (global (;2;) i32 (i32.const 8245))
  (export "apply" (func 26))
  (elem (i32.const 1) 24)
  (data (i32.const 8220) "failed to allocate pages\00")
  (data (i32.const 0) "8 \00\00"))
