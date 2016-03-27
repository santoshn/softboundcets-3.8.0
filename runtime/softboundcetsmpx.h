//=== softboundcetsmpx.h - headers for functions introduced by SoftBound+CETS--*- C -*===// 
// Copyright (c) 2015 Santosh Nagarakatte. All rights reserved.

// Developed by: Santosh Nagarakatte, Rutgers University
//               http://www.cs.rutgers.edu/~santosh.nagarakatte/softbound/

// The  SoftBoundCETS project had contributions from:
// Santosh Nagarakatte, Rutgers University,
// Milo M K Martin, University of Pennsylvania,
// Steve Zdancewic, University of Pennsylvania,
// Jianzhou Zhao, University of Pennsylvania


// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal with the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

//   1. Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimers.

//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimers in the
//      documentation and/or other materials provided with the distribution.

//   3. Neither the names of its developers nor the names of its
//      contributors may be used to endorse or promote products
//      derived from this software without specific prior written
//      permission.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// WITH THE SOFTWARE.
//===---------------------------------------------------------------------===//


#ifndef __SOFTBOUNDCETSMPX_H__
#define __SOFTBOUNDCETSMPX_H__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

typedef struct {


  void* base;
  void* bound;
  size_t key;
  void* lock;
  void* ptr;
#define __SOFTBOUNDCETSMPX_METADATA_NUM_FIELDS 4

#define __BASE_INDEX 0
#define __BOUND_INDEX 1
#define __KEY_INDEX 2
#define __LOCK_INDEX 3

} __softboundcetsmpx_trie_entry_t;


#if defined(__APPLE__)
#define SOFTBOUNDCETSMPX_MMAP_FLAGS (MAP_ANON|MAP_NORESERVE|MAP_PRIVATE)
#else
#define SOFTBOUNDCETSMPX_MMAP_FLAGS (MAP_PRIVATE|MAP_ANONYMOUS|MAP_NORESERVE)
#endif


#ifdef __SOFTBOUNDCETSMPX_DEBUG
#undef __SOFTBOUNDCETSMPX_DEBUG
static const int __SOFTBOUNDCETSMPX_DEBUG = 1;
#define __SOFTBOUNDCETSMPX_NORETURN 
#else 
static const int __SOFTBOUNDCETSMPX_DEBUG = 0;
#define __SOFTBOUNDCETSMPX_NORETURN __attribute__((__noreturn__))
#endif

#ifdef __SOFTBOUNDCETSMPX_PREALLOCATE_TRIE
#undef __SOFTBOUNDCETSMPX_PREALLOCATE_TRIE
static const int __SOFTBOUNDCETSMPX_PREALLOCATE_TRIE = 1;
#else
static const int __SOFTBOUNDCETSMPX_PREALLOCATE_TRIE = 0;
#endif

#ifdef __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL 
#define __SOFTBOUNDCETSMPX_FREE_MAP
#endif

#ifdef __SOFTBOUNDCETSMPX_TEMPORAL
#define __SOFTBOUNDCETSMPX_FREE_MAP
#endif

#ifdef __SOFTBOUNDCETSMPX_FREE_MAP
#undef __SOFTBOUNDCETSMPX_FREE_MAP
static const int __SOFTBOUNDCETSMPX_FREE_MAP = 1;
#else 
static const int __SOFTBOUNDCETSMPX_FREE_MAP = 0;
#endif



// check if __WORDSIZE works with clang on both Linux and MacOSX
/* Allocating one million entries for the temporal key */
static const size_t __SOFTBOUNDCETSMPX_N_TEMPORAL_ENTRIES = ((size_t) 64*(size_t) 1024 * (size_t) 1024); 
static const size_t __SOFTBOUNDCETSMPX_LOWER_ZERO_POINTER_BITS = 3;

static const size_t __SOFTBOUNDCETSMPX_N_STACK_TEMPORAL_ENTRIES = ((size_t) 1024 * (size_t) 64);
static const size_t __SOFTBOUNDCETSMPX_N_GLOBAL_LOCK_SIZE = ((size_t) 1024 * (size_t) 32);

// 2^23 entries each will be 8 bytes each 
static const size_t __SOFTBOUNDCETSMPX_TRIE_PRIMARY_TABLE_ENTRIES = ((size_t) 8*(size_t) 1024 * (size_t) 1024);

static const size_t __SOFTBOUNDCETSMPX_SHADOW_STACK_ENTRIES = ((size_t) 128 * (size_t) 32 );

/* 256 Million simultaneous objects */
static const size_t __SOFTBOUNDCETSMPX_N_FREE_MAP_ENTRIES = ((size_t) 32 * (size_t) 1024* (size_t) 1024);
// each secondary entry has 2^ 22 entries 
static const size_t __SOFTBOUNDCETSMPX_TRIE_SECONDARY_TABLE_ENTRIES = ((size_t) 4 * (size_t) 1024 * (size_t) 1024); 


#define __WEAK__ __attribute__((__weak__))
#define __WEAK_INLINE __attribute__((__weak__,__always_inline__)) 

#define __METADATA_INLINE __attribute__((__weak__, __always_inline__))

#define __NO_INLINE __attribute__((__noinline__))

extern __softboundcetsmpx_trie_entry_t** __softboundcetsmpx_trie_primary_table;

extern size_t* __softboundcetsmpx_shadow_stack_ptr;
extern size_t* __softboundcetsmpx_temporal_space_begin;

extern size_t* __softboundcetsmpx_stack_temporal_space_begin;
extern size_t* __softboundcetsmpx_free_map_table;

extern __SOFTBOUNDCETSMPX_NORETURN void __softboundcetsmpx_abort();
extern void __softboundcetsmpx_printf(const char* str, ...);
extern size_t* __softboundcetsmpx_global_lock; 

void* __softboundcetsmpx_safe_calloc(size_t, size_t);
void* __softboundcetsmpx_safe_malloc(size_t);
void __softboundcetsmpx_safe_free(void*);

void * __softboundcetsmpx_safe_mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
__WEAK_INLINE void __softboundcetsmpx_add_to_free_map(size_t ptr_key, void* ptr) ;

/******************************************************************************/

static __attribute__ ((__constructor__)) void __softboundcetsmpx_global_init();

extern __NO_INLINE void __softboundcetsmpx_stub(void);

extern void __softboundcetsmpx_init(void);

void __softboundcetsmpx_global_init()
{
  __softboundcetsmpx_init();
  __softboundcetsmpx_stub();
}


/* Layout of the shadow stack

  1) size of the previous stack frame
  2) size of the current stack frame
  3) base/bound/key/lock of each argument

  Allocation: read the current stack frames size, increment the
  shadow_stack_ptr by current_size + 2, store the previous size into
  the new prev value, calcuate the allocation size and store in the
  new current stack size field; Deallocation: read the previous size,
  and decrement the shadow_stack_ptr */
  
__WEAK_INLINE void __softboundcetsmpx_allocate_shadow_stack_space(int num_pointer_args){
 

  size_t* prev_stack_size_ptr = __softboundcetsmpx_shadow_stack_ptr + 1;
  size_t prev_stack_size = *((size_t*)prev_stack_size_ptr);

  __softboundcetsmpx_shadow_stack_ptr = __softboundcetsmpx_shadow_stack_ptr + prev_stack_size + 2;
  
  *((size_t*) __softboundcetsmpx_shadow_stack_ptr) = prev_stack_size;
  size_t* current_stack_size_ptr = __softboundcetsmpx_shadow_stack_ptr + 1;
  
  ssize_t size = num_pointer_args * __SOFTBOUNDCETSMPX_METADATA_NUM_FIELDS;
  *((size_t*) current_stack_size_ptr) = size;
}
   
__WEAK_INLINE void* __softboundcetsmpx_load_base_shadow_stack(int arg_no){
  assert (arg_no >= 0 );
  size_t count = 2 +  arg_no * __SOFTBOUNDCETSMPX_METADATA_NUM_FIELDS + __BASE_INDEX ;
  size_t* base_ptr = (__softboundcetsmpx_shadow_stack_ptr + count); 
  void* base = *((void**)base_ptr);
  return base;
}

__WEAK_INLINE void* __softboundcetsmpx_load_bound_shadow_stack(int arg_no){

  assert (arg_no >= 0 );
  size_t count = 2 + arg_no * __SOFTBOUNDCETSMPX_METADATA_NUM_FIELDS  + __BOUND_INDEX ;
  size_t* bound_ptr = (__softboundcetsmpx_shadow_stack_ptr + count); 

  void* bound = *((void**)bound_ptr);
  return bound;
}

__WEAK_INLINE size_t __softboundcetsmpx_load_key_shadow_stack(int arg_no){

  assert (arg_no >= 0 );
  size_t count = 2 + arg_no * __SOFTBOUNDCETSMPX_METADATA_NUM_FIELDS  + __KEY_INDEX ;
  size_t* key_ptr = (__softboundcetsmpx_shadow_stack_ptr + count); 
  size_t key = *key_ptr;
  return key;
}

__WEAK_INLINE void* __softboundcetsmpx_load_lock_shadow_stack(int arg_no){

  assert (arg_no >= 0 );
  size_t count = 2 + arg_no * __SOFTBOUNDCETSMPX_METADATA_NUM_FIELDS + __LOCK_INDEX;
  size_t* lock_ptr = (__softboundcetsmpx_shadow_stack_ptr + count); 
  void* lock = *((void**)lock_ptr);
  return lock;
}

__WEAK_INLINE void __softboundcetsmpx_store_base_shadow_stack(void* base, int arg_no){
  
  assert(arg_no >= 0);
  size_t count = 2 +  arg_no * __SOFTBOUNDCETSMPX_METADATA_NUM_FIELDS + __BASE_INDEX ;
  void** base_ptr = (void**)(__softboundcetsmpx_shadow_stack_ptr + count); 

  *(base_ptr) = base;

}

__WEAK_INLINE void __softboundcetsmpx_store_bound_shadow_stack(void* bound, int arg_no){

  assert(arg_no >= 0);
  size_t count = 2 +  arg_no * __SOFTBOUNDCETSMPX_METADATA_NUM_FIELDS + __BOUND_INDEX ;
  void** bound_ptr = (void**)(__softboundcetsmpx_shadow_stack_ptr + count); 

  *(bound_ptr) = bound;
}

__WEAK_INLINE void __softboundcetsmpx_store_key_shadow_stack(size_t key, int arg_no){
  assert(arg_no >= 0);
  size_t count = 2 +  arg_no * __SOFTBOUNDCETSMPX_METADATA_NUM_FIELDS + __KEY_INDEX ;
  size_t* key_ptr = (__softboundcetsmpx_shadow_stack_ptr + count); 

  *(key_ptr) = key;

}


__WEAK_INLINE void __softboundcetsmpx_store_lock_shadow_stack(void* lock, int arg_no){
  assert(arg_no >= 0);
  size_t count = 2 +  arg_no * __SOFTBOUNDCETSMPX_METADATA_NUM_FIELDS + __LOCK_INDEX ;
  void** lock_ptr = (void**)(__softboundcetsmpx_shadow_stack_ptr + count); 

  *(lock_ptr) = lock;
}

__WEAK_INLINE void __softboundcetsmpx_deallocate_shadow_stack_space(){

  size_t* reserved_space_ptr = __softboundcetsmpx_shadow_stack_ptr;

  size_t read_value = *((size_t*) reserved_space_ptr);

  assert((read_value >=0 && read_value <= __SOFTBOUNDCETSMPX_SHADOW_STACK_ENTRIES));
                                                
  __softboundcetsmpx_shadow_stack_ptr =  __softboundcetsmpx_shadow_stack_ptr - read_value - 2;
}

__WEAK_INLINE __softboundcetsmpx_trie_entry_t* __softboundcetsmpx_trie_allocate(){
  
  __softboundcetsmpx_trie_entry_t* secondary_entry;
  size_t length = (__SOFTBOUNDCETSMPX_TRIE_SECONDARY_TABLE_ENTRIES) * sizeof(__softboundcetsmpx_trie_entry_t);
  secondary_entry = __softboundcetsmpx_safe_mmap(0, length, PROT_READ| PROT_WRITE, 
					      SOFTBOUNDCETSMPX_MMAP_FLAGS, -1, 0);
  //assert(secondary_entry != (void*)-1); 
  //printf("snd trie table %p %lx\n", secondary_entry, length);
  return secondary_entry;
}


__WEAK_INLINE void __softboundcetsmpx_dummy(){
  
  printf("calling abort");

}
__WEAK_INLINE void __softboundcetsmpx_introspect_metadata(void* ptr, 
						       void* base, 
						       void* bound, 
						       int arg_no){
  
  printf("[introspect_metadata]ptr=%p, base=%p, bound=%p, arg_no=%d\n", 
	 ptr, base, bound, arg_no);
}

__METADATA_INLINE 
void __softboundcetsmpx_copy_metadata(void* dest, void* from, 
				   size_t size){
  
  //  printf("dest=%p, from=%p, size=%zx\n", dest, from, size);
  
  size_t dest_ptr = (size_t) dest;
  size_t dest_ptr_end = dest_ptr + size;

  size_t from_ptr = (size_t) from;
  size_t from_ptr_end = from_ptr + size;


  if(from_ptr % 8 != 0){
    //printf("dest=%p, from=%p, size=%zx\n", dest, from, size);
    return;
    //    from_ptr = from_ptr %8;
    //    dest_ptr = dest_ptr %8;

  }

  //  printf("dest=%p, from=%p, size=%zx\n", dest, from, size);
  __softboundcetsmpx_trie_entry_t* trie_secondary_table_dest_begin;
  __softboundcetsmpx_trie_entry_t* trie_secondary_table_from_begin;
  
  size_t dest_primary_index_begin = (dest_ptr >> 25);
  size_t dest_primary_index_end = (dest_ptr_end >> 25);

  size_t from_primary_index_begin = (from_ptr >> 25);
  size_t from_primary_index_end =  (from_ptr_end >> 25);


  if((from_primary_index_begin != from_primary_index_end) || 
     (dest_primary_index_begin != dest_primary_index_end)){

    size_t from_sizet = from_ptr;
    size_t dest_sizet = dest_ptr;

    size_t trie_size = size;
    size_t index = 0;

    for(index=0; index < trie_size; index = index + 8){
      
      size_t temp_from_pindex = (from_sizet + index) >> 25;
      size_t temp_to_pindex = (dest_sizet + index) >> 25;

      size_t dest_secondary_index = (((dest_sizet + index) >> 3) & 0x3fffff);
      size_t from_secondary_index = (((from_sizet + index) >> 3) & 0x3fffff);
      
      __softboundcetsmpx_trie_entry_t* temp_from_strie = __softboundcetsmpx_trie_primary_table[temp_from_pindex];

      if(temp_from_strie == NULL){
        temp_from_strie = __softboundcetsmpx_trie_allocate();
        __softboundcetsmpx_trie_primary_table[temp_from_pindex] = temp_from_strie;
      }
     __softboundcetsmpx_trie_entry_t* temp_to_strie = __softboundcetsmpx_trie_primary_table[temp_to_pindex];

      if(temp_to_strie == NULL){
        temp_to_strie = __softboundcetsmpx_trie_allocate();
        __softboundcetsmpx_trie_primary_table[temp_to_pindex] = temp_to_strie;
      }

      void* dest_entry_ptr = &temp_to_strie[dest_secondary_index];
      void* from_entry_ptr = &temp_from_strie[from_secondary_index];

      memcpy(dest_entry_ptr, from_entry_ptr, sizeof(__softboundcetsmpx_trie_entry_t));
    }    
    return;

  }
    
  trie_secondary_table_dest_begin = __softboundcetsmpx_trie_primary_table[dest_primary_index_begin];
  trie_secondary_table_from_begin = __softboundcetsmpx_trie_primary_table[from_primary_index_begin];
  
  if(trie_secondary_table_from_begin == NULL)
    return;

  if(trie_secondary_table_dest_begin == NULL){
    trie_secondary_table_dest_begin = __softboundcetsmpx_trie_allocate();
    __softboundcetsmpx_trie_primary_table[dest_primary_index_begin] = trie_secondary_table_dest_begin;
  }

  size_t dest_secondary_index = ((dest_ptr>> 3) & 0x3fffff);
  size_t from_secondary_index = ((from_ptr>> 3) & 0x3fffff);
  
  assert(dest_secondary_index < __SOFTBOUNDCETSMPX_TRIE_SECONDARY_TABLE_ENTRIES);
  assert(from_secondary_index < __SOFTBOUNDCETSMPX_TRIE_SECONDARY_TABLE_ENTRIES);

  void* dest_entry_ptr = &trie_secondary_table_dest_begin[dest_secondary_index];
  void* from_entry_ptr = &trie_secondary_table_from_begin[from_secondary_index];
  

  memcpy(dest_entry_ptr, from_entry_ptr, sizeof(__softboundcetsmpx_trie_entry_t)* (size >> 3));

  return;
}

__WEAK_INLINE void 
__softboundcetsmpx_shrink_bounds(void* new_base, void* new_bound, 
                              void* old_base, void* old_bound, 
                              void** base_alloca, void** bound_alloca) {


  *(base_alloca) = new_base < old_base ? old_base: new_base;
  *(bound_alloca) = new_bound > old_bound? old_bound : new_bound;
}

__WEAK_INLINE void 
__softboundcetsmpx_spatial_call_dereference_check(void* base, void* bound, 
                                               void* ptr) {

  if ((base != bound) && (ptr != base)) {
    if (__SOFTBOUNDCETSMPX_DEBUG) {
      __softboundcetsmpx_printf("In Call Dereference Check, base=%p, bound=%p, ptr=%p\n", base, bound, ptr);
    }
    __softboundcetsmpx_abort();
  }
}

extern void* malloc_address;
__WEAK_INLINE void 
__softboundcetsmpx_spatial_load_dereference_check(void *base, void *bound, 
                                               void *ptr, size_t size_of_type)
{


  if ((ptr < base) || ((void*)((char*) ptr + size_of_type) > bound)) {

    __softboundcetsmpx_printf("In LDC, base=%zx, bound=%zx, ptr=%zx\n",
    			   base, bound, ptr);    
    __softboundcetsmpx_abort();
  }
}



__WEAK_INLINE void 
__softboundcetsmpx_spatial_store_dereference_check(void *base, 
                                                void *bound, 
                                                void *ptr, 
                                                size_t size_of_type)
{
  
  if ((ptr < base) || ((void*)((char*)ptr + size_of_type) > bound)) {
    __softboundcetsmpx_printf("In Store Dereference Check, base=%p, bound=%p, ptr=%p, size_of_type=%zx, ptr+size=%p\n",
                              base, bound, ptr, size_of_type, (char*)ptr+size_of_type); 
    
    __softboundcetsmpx_abort();
  }
}

/* Memcopy check, different variants based on spatial, temporal and
   spatial+temporal modes
*/

__WEAK_INLINE void 
__softboundcetsmpx_memcopy_check(void* dest, void* src, size_t size,
                              void* dest_base, void* dest_bound, 
                              void* src_base, void* src_bound,
                              size_t dest_key, void* dest_lock, 
                              size_t src_key, void* src_lock) {  

  /* printf("dest=%zx, src=%zx, size=%zx, ulong_max=%zx\n",  */
  /*        dest, src, size, ULONG_MAX); */
  if(size >= LONG_MAX)
    __softboundcetsmpx_abort();


  if(dest < dest_base || (char*) dest > ((char*) dest_bound - size) || (size > (size_t) dest_bound))
    __softboundcetsmpx_abort();

  if(src < src_base || (char*) src > ((char*) src_bound - size) || (size > (size_t) dest_bound))
    __softboundcetsmpx_abort();

  if(dest_key != *((size_t*)(dest_lock))){
    __softboundcetsmpx_abort();
  }

  if(src_key != *((size_t*)(src_lock))){
    __softboundcetsmpx_abort();
  }

}

/* Memset check, different variants based on spatial, temporal and
   spatial+temporal modes */

__WEAK_INLINE void 
__softboundcetsmpx_memset_check(void* dest, size_t size,
                             void* dest_base, void* dest_bound, 
                             size_t dest_key, void* dest_lock){

  if(size >= LONG_MAX)
    __softboundcetsmpx_abort();

  if(dest < dest_base || (char*) dest > ((char*) dest_bound - size) || (size > (size_t) dest_bound))
    __softboundcetsmpx_abort();

  if(dest_key != *((size_t*)(dest_lock))){
    __softboundcetsmpx_abort();
  }
}

/* Metadata store parameterized by the mode of checking */

__METADATA_INLINE void __softboundcetsmpx_metadata_store(void* addr_of_ptr, 
							 void* base, 
							 void* bound, 
							 size_t key, 
							 void* lock,
							 void* stored_ptr) {  
  
  size_t ptr = (size_t) addr_of_ptr;
  size_t primary_index;
  __softboundcetsmpx_trie_entry_t* trie_secondary_table;
  //  __softboundcetsmpx_trie_entry_t** trie_primary_table = __softboundcetsmpx_trie_primary_table;
  
  
  primary_index = (ptr >> 25);
  trie_secondary_table = __softboundcetsmpx_trie_primary_table[primary_index];
 

  if(trie_secondary_table == NULL){
    trie_secondary_table =  __softboundcetsmpx_trie_allocate();
    __softboundcetsmpx_trie_primary_table[primary_index] = trie_secondary_table;
  }    

  
  size_t secondary_index = ((ptr >> 3) & 0x3fffff);
  __softboundcetsmpx_trie_entry_t* entry_ptr =&trie_secondary_table[secondary_index];


  
  entry_ptr->base = base;
  entry_ptr->bound = bound;
  entry_ptr->key = key;
  entry_ptr->lock = lock;
  entry_ptr->ptr = stored_ptr;

  return;
}


__METADATA_INLINE void __softboundcetsmpx_metadata_load(void* addr_of_ptr, void** base, 
                                                     void** bound, size_t* key, void** lock){   


  size_t ptr = (size_t) addr_of_ptr;
  __softboundcetsmpx_trie_entry_t* trie_secondary_table;

  size_t primary_index = ( ptr >> 25);
  trie_secondary_table = __softboundcetsmpx_trie_primary_table[primary_index];


  if(trie_secondary_table == NULL) {  
    *((void**) base) = 0;
    *((void**) bound) = 0;
    *((size_t*) key ) = 0;
    *((size_t*) lock) = 0;        
      return;
  }
  
    /* MAIN SOFTBOUNDCETSMPX LOAD WHICH RUNS ON THE NORMAL MACHINE */
  size_t secondary_index = ((ptr >> 3) & 0x3fffff);
  __softboundcetsmpx_trie_entry_t* entry_ptr = &trie_secondary_table[secondary_index];
   
  void* stored_base = entry_ptr->base;
  void* stored_bound = entry_ptr->bound;
  size_t stored_key = entry_ptr ->key;
  void* stored_lock = entry_ptr->lock;

  void* current_ptr = *((void**)(addr_of_ptr));
  if(current_ptr != entry_ptr->ptr){
    stored_base = NULL;
    size_t unbound_value = 1;
    stored_bound = (void*)(unbound_value << 48);
    stored_key = 1;
    stored_lock = __softboundcetsmpx_global_lock;
  }

  //  printf("base=%p, bound=%p, key=%zx, lock=%p\n", stored_base, stored_bound, stored_key, stored_lock);
  
  *((void**) base) = stored_base;
  *((void**) bound) = stored_bound;
  *((size_t*) key) = stored_key;
  *((void**) lock) = stored_lock;
      

  return;
}
/******************************************************************************/

extern size_t __softboundcetsmpx_key_id_counter;
extern size_t* __softboundcetsmpx_lock_next_location;
extern size_t* __softboundcetsmpx_lock_new_location;

__WEAK_INLINE void 
__softboundcetsmpx_temporal_load_dereference_check(void* pointer_lock, 
                                                size_t key, 
                                                void* base, 
                                                void* bound) {


#if 0  
  /* URGENT: I should think about removing this condition check */
  if(!pointer_lock){
    if(__SOFTBOUNDCETSMPX_DEBUG) {
      __softboundcetsmpx_printf("Temporal lock null\n");
    }
    __softboundcetsmpx_printf("Temporal lock null\n");
    __softboundcetsmpx_abort();
    return;
  }

#endif


  size_t temp = *((size_t*)pointer_lock);
  
  if(temp != key) {
    if(__SOFTBOUNDCETSMPX_DEBUG) {
      __softboundcetsmpx_printf("[TLDC] Key mismatch key = %zx, *lock=%zx\n", 
                             key, temp );
    }
    __softboundcetsmpx_printf("[TLDC] Key mismatch key = %zx, *lock=%zx, next_ptr =%zx\n", 
                           key, temp, __softboundcetsmpx_lock_next_location );
    __softboundcetsmpx_abort();    
  }

}


__WEAK_INLINE void 
__softboundcetsmpx_temporal_store_dereference_check(void* pointer_lock, 
                                                 size_t key, 
                                                 void* base, 
                                                 void* bound) {
#if 0
  if(!pointer_lock){
    __softboundcetsmpx_printf("lock null?");
    __softboundcetsmpx_abort();    
  }
#endif
  
  size_t temp = *((size_t*)pointer_lock);
  
  if(temp != key) {

    if(__SOFTBOUNDCETSMPX_DEBUG) {
      __softboundcetsmpx_printf("[TSDC] Key mismatch, key = %zx, *lock=%zx\n", 
                             key, temp );
    }
    __softboundcetsmpx_printf("[TSDC] Key mismatch, key = %zx, *lock=%zx\n", 
                           key, temp );    
    __softboundcetsmpx_abort();    
  }
}


__WEAK_INLINE void __softboundcetsmpx_stack_memory_deallocation(size_t ptr_key){

  
#ifndef __SOFTBOUNDCETSMPX_CONSTANT_STACK_KEY_LOCK

  __softboundcetsmpx_stack_temporal_space_begin--;
  *(__softboundcetsmpx_stack_temporal_space_begin) = 0;

#endif

  return;

}

__WEAK_INLINE void 
__softboundcetsmpx_memory_deallocation(void* ptr_lock, size_t ptr_key) {

  
  if(__SOFTBOUNDCETSMPX_DEBUG){
    __softboundcetsmpx_printf("[Hdealloc] pointer_lock = %p, *pointer_lock=%zx\n", 
                           ptr_lock, *((size_t*) ptr_lock));
  }

#if 0
  if(!ptr_lock)
    return;
#endif
  
  *((size_t*)ptr_lock) = 0;
  *((void**) ptr_lock) = __softboundcetsmpx_lock_next_location;
  __softboundcetsmpx_lock_next_location = ptr_lock;

}

__WEAK_INLINE void*  __softboundcetsmpx_allocate_lock_location() {
  
  void* temp= NULL;
  if(__softboundcetsmpx_lock_next_location == NULL) {
    if(__SOFTBOUNDCETSMPX_DEBUG) {
      __softboundcetsmpx_printf("[lock_allocate] new_lock_location=%p\n", 
                             __softboundcetsmpx_lock_new_location);
      
      if(__softboundcetsmpx_lock_new_location  > 
         __softboundcetsmpx_temporal_space_begin + __SOFTBOUNDCETSMPX_N_TEMPORAL_ENTRIES){
        __softboundcetsmpx_printf("[lock_allocate] out of temporal free entries \n");
        __softboundcetsmpx_abort();
      }
    }

    return __softboundcetsmpx_lock_new_location++;
  }
  else{

    temp = __softboundcetsmpx_lock_next_location;
    if(__SOFTBOUNDCETSMPX_DEBUG){
      __softboundcetsmpx_printf("[lock_allocate] next_lock_location=%p\n", temp);
    }

    __softboundcetsmpx_lock_next_location = *((void**)__softboundcetsmpx_lock_next_location);
    return temp;
  }
}


__WEAK_INLINE void 
__softboundcetsmpx_stack_memory_allocation(void** ptr_lock, size_t* ptr_key) {


#ifdef __SOFTBOUNDCETSMPX_CONSTANT_STACK_KEY_LOCK
  *((size_t*) ptr_key) = 1;
  *((size_t**) ptr_lock) = __softboundcetsmpx_global_lock;
#else
  size_t temp_id = __softboundcetsmpx_key_id_counter++;
  *((size_t**) ptr_lock) = (size_t*)__softboundcetsmpx_stack_temporal_space_begin++;
  *((size_t*)ptr_key) = temp_id;
  **((size_t**)ptr_lock) = temp_id;  
#endif

}

__WEAK_INLINE void 
__softboundcetsmpx_memory_allocation(void* ptr, void** ptr_lock, size_t* ptr_key){

  size_t temp_id = __softboundcetsmpx_key_id_counter++;

  *((size_t**) ptr_lock) = (size_t*)__softboundcetsmpx_allocate_lock_location();  
  *((size_t*) ptr_key) = temp_id;
  **((size_t**) ptr_lock) = temp_id;

  __softboundcetsmpx_add_to_free_map(temp_id, ptr);
  //  printf("memory allocation ptr=%zx, ptr_key=%zx\n", ptr, temp_id);
  if(__SOFTBOUNDCETSMPX_DEBUG) {    
    __softboundcetsmpx_printf("[mem_alloc] lock = %p, ptr_key = %p, key = %zx\n", 
                           ptr_lock, ptr_key, temp_id);
  }
}


__WEAK_INLINE void* __softboundcetsmpx_get_global_lock(){  
  return __softboundcetsmpx_global_lock;
}

__WEAK_INLINE void __softboundcetsmpx_add_to_free_map(size_t ptr_key, void* ptr) {

  if(!__SOFTBOUNDCETSMPX_FREE_MAP)
    return;

  assert(ptr!= NULL);

  size_t counter  = 0;
  while(1){
    size_t index = (ptr_key + counter) % __SOFTBOUNDCETSMPX_N_FREE_MAP_ENTRIES;
    size_t* entry_ptr = &__softboundcetsmpx_free_map_table[index];
    size_t tag = *entry_ptr;

    if(tag == 0 || tag == 2) {
      //      printf("entry_ptr=%zx, ptr=%zx, key=%zx\n", entry_ptr, ptr, ptr_key);
      *entry_ptr = (size_t)(ptr);
      return;
    }
    if(counter >= (__SOFTBOUNDCETSMPX_N_FREE_MAP_ENTRIES)) {
#ifndef __NOSIM_CHECKS
      __softboundcetsmpx_abort();
#else
      break;
#endif
    }
    counter++;
  }
  return;
}


__WEAK_INLINE void __softboundcetsmpx_check_remove_from_free_map(size_t ptr_key, void* ptr) {

  if(! __SOFTBOUNDCETSMPX_FREE_MAP){
    return;
  }
  size_t counter = 0;
  while(1) {
    size_t index = (ptr_key + counter) % __SOFTBOUNDCETSMPX_N_FREE_MAP_ENTRIES;
    size_t* entry_ptr = &__softboundcetsmpx_free_map_table[index];
    size_t tag = *entry_ptr;

    if(tag == 0) {
#ifndef __NOSIM_CHECKS      
      //      printf("free map does not have the key\n");
      __softboundcetsmpx_abort();
#else
      break;
#endif
    }

    if(tag == (size_t) ptr) {      
      *entry_ptr = 2;
      return;
    }

    if(counter >= __SOFTBOUNDCETSMPX_N_FREE_MAP_ENTRIES) {
      //      printf("free map out of entries\n");
#ifndef __NOSIM_CHECKS
      printf("free map out of entries\n");
      __softboundcetsmpx_abort();
#else
      break;
#endif
    }
    counter++;
  }
  return;
}

 __METADATA_INLINE void __softboundcetsmpx_metadata_load_vector(void* addr_of_ptr, 
							     void** base, 
							     void** bound, 
							     size_t* key, 
							     void** lock, 
							     int index){

   size_t val = index * 8;
   size_t addr = (size_t) addr_of_ptr;
   addr = addr + val;

   __softboundcetsmpx_metadata_load((void*) addr, base, bound, key, lock);   

 }

 __METADATA_INLINE void __softboundcetsmpx_metadata_store_vector(void* addr_of_ptr, 
								 void* base, 
								 void* bound, 
								 size_t key, 
								 void* lock,
								 void* ptr, 
								 int index){
   size_t val = index * 8;
   size_t addr = (size_t) addr_of_ptr;
   addr = addr + val;

   __softboundcetsmpx_metadata_store((void*)addr, base, bound, key, lock, ptr);
   
 }


#endif

