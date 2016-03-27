///=== SoftBoundCETS/FixByValAttributes.cpp --*- C++ -*=====///
// Copyright (c) 2016 Santosh Nagarakatte. All rights reserved.

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

#include "llvm/Transforms/Instrumentation.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/GetElementPtrTypeIterator.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/STLExtras.h"
#include <algorithm>
#include <cstdarg>


#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/LLVMContext.h"

#include "llvm/IR/Instruction.h"
#include "llvm-c/Target.h"
#include "llvm-c/TargetMachine.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/CFG.h"
#include "llvm/ADT/StringExtras.h"

#include "llvm/Support/Debug.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

#include<queue>



using namespace llvm;

class FixByValAttributes: public ModulePass{

 private:
  bool m_is_64bit;
  bool runOnModule(Module &);
  bool transformFunction(Function*);
  bool checkTypeHasPtrs(Argument*);
  bool checkPtrsInST(StructType*);
  void createGEPStores(Value*, Value*,StructType*, Instruction*, 
                       std::vector<Value*>);

 public:
  static char ID;
 FixByValAttributes(): ModulePass(ID){
  }
  const char* getPassName() const { return "FixByValAttributes";}
  
};


static cl::opt<bool>
fix_all_byval
("fix_all_byval",
 cl::desc("Transform all byval attributes"),
 cl::init(true));

static cl::opt<bool>
fix_specific_byval
("fix_specific_byval",
 cl::desc("Transform only pointer byval attributes"),
 cl::init(false));

char FixByValAttributes:: ID;

INITIALIZE_PASS(FixByValAttributes, "fixbyval", "FixByValAttributes: Helper for the SoftBoundCETS pass", false, false)


ModulePass *
llvm::createFixByValAttributesPass(){
  return new FixByValAttributes();
}



#if 0
static RegisterPass<FixByValAttributes> P ("FixByValAttributes",
                                               "Transform all byval Attributes");
#endif

void 
FixByValAttributes::createGEPStores(Value* result_alloca, 
                                        Value* call_site_arg, 
                                        StructType* struct_type,
                                        Instruction* insert_at, 
                                        std::vector<Value*> indices){


  int i = 0;
  StructType::element_iterator I = struct_type->element_begin();
  
  for(StructType::element_iterator E = struct_type->element_end(); 
      I!=E ; ++I, i++){

    std::vector<Value*> new_indices;
    Type* element_type = *I;
    
    for(size_t i = 0; i< indices.size(); i++){
      new_indices.push_back(indices[i]);
    }

    Constant* loop_index = ConstantInt::get(Type::getInt32Ty(result_alloca->getType()->getContext()), i, false);
    new_indices.push_back(loop_index);

    if(isa<StructType>(element_type)){
      StructType* elem_struct_type = dyn_cast<StructType>(element_type);
      createGEPStores(result_alloca, call_site_arg,  elem_struct_type,insert_at, new_indices);
    }
    else{

      /* FIXME: I need to fix the types */
      GetElementPtrInst* gep_idx_src = GetElementPtrInst::Create(call_site_arg->getType(),
								 call_site_arg, 
								 new_indices, 
								 "", insert_at);

      GetElementPtrInst* gep_idx_dest = GetElementPtrInst::Create(result_alloca->getType(),
								  result_alloca, 
								  new_indices,
								  "", insert_at);

      LoadInst* src_load = new LoadInst(gep_idx_src, "", insert_at);
      new StoreInst(src_load, gep_idx_dest, false, insert_at);
    }
  }

}

bool FixByValAttributes::checkPtrsInST(StructType* struct_type){
  
  StructType::element_iterator I = struct_type->element_begin();
 

  bool ptr_flag = false;
  for(StructType::element_iterator E = struct_type->element_end(); I != E; ++I){
    
    Type* element_type = *I;

    if(isa<StructType>(element_type)){
      StructType* struct_element_type = dyn_cast<StructType>(element_type);
      bool recursive_flag = checkPtrsInST(struct_element_type);
      ptr_flag = ptr_flag | recursive_flag;
    }
    if(isa<PointerType>(element_type)){
      ptr_flag = true;
    }
    if(isa<ArrayType>(element_type)){
      ptr_flag = true;      
    }
  }
  return ptr_flag;
}


bool FixByValAttributes::checkTypeHasPtrs(Argument* ptr_argument){

  if(!ptr_argument->hasByValAttr())
    return false;

  SequentialType* seq_type = dyn_cast<SequentialType>(ptr_argument->getType());
  assert(seq_type && "byval attribute with non-sequential type pointer, not handled?");

  StructType* struct_type = dyn_cast<StructType>(seq_type->getElementType());

  if(struct_type){
    bool has_ptrs = checkPtrsInST(struct_type);
    return has_ptrs;
  }
  else{
    assert(0 && "non-struct byval parameters?");
  }

  // By default we assume any struct can return pointers 
  return true;                                              

}


bool FixByValAttributes:: transformFunction(Function* func){
  
  bool byval_arg = false;
  for(Function::arg_iterator ib = func->arg_begin(), ie = func->arg_end(); 
      ib != ie; ++ib){
    Argument* ptr_argument = dyn_cast<Argument>(ib);

    if(ptr_argument->hasByValAttr()){
      if(checkTypeHasPtrs(ptr_argument)){
        byval_arg = true;
      }      
    }    
  }
  if(!byval_arg)
    return false;

  Type* ret_type = func->getReturnType();
  const FunctionType* fty = func->getFunctionType();
  std::vector<Type*> params;
  
  SmallVector<AttributeSet, 8> param_attrs_vec;

  const AttributeSet& pal = func->getAttributes();

  if(pal.hasAttributes(AttributeSet::ReturnIndex))
    param_attrs_vec.push_back(AttributeSet::get(func->getContext(), pal.getRetAttributes()));

  int arg_index = 1;
  for(Function::arg_iterator i = func->arg_begin(), e = func->arg_end();
      i != e; ++i, ++arg_index){
    Argument* arg = dyn_cast<Argument>(i);
 
    params.push_back(i->getType());
    AttributeSet attrs = pal.getParamAttributes(arg_index);
    if(attrs.hasAttributes(arg_index)){
      if(arg->hasByValAttr()){

      }
      else{
	AttrBuilder B(attrs, arg_index);
	param_attrs_vec.push_back(AttributeSet::get(func->getContext(), params.size(), B));
      }
      
    }

#if 0
    if((Attributes attrs = pal.getParamAttributes(arg_index)) && !(arg->hasByValAttr())){
      param_attrs_vec.push_back(AttributeWithIndex::get(params.size(), attrs));
      //param_attrs_vec.push_back(AttributeWithIndex::get(arg_index, attrs));
    }
#endif
  }
  
  FunctionType* nfty = FunctionType::get(ret_type, params, fty->isVarArg());
  Function* new_func = Function::Create(nfty, func->getLinkage(), func->getName()+ ".sb");
  //  new_func->copyAttributesFrom(func);
  new_func->setAttributes(AttributeSet::get(func->getContext(), param_attrs_vec));
                          
  SmallVector<Value*, 16> call_args;      
  new_func->getBasicBlockList().splice(new_func->begin(), func->getBasicBlockList());  
  
  func->getParent()->getFunctionList().insert(func->getIterator(), new_func);

  Function::arg_iterator arg_i2 = new_func->arg_begin();      
  for(Function::arg_iterator arg_i = func->arg_begin(), arg_e = func->arg_end(); 
      arg_i != arg_e; ++arg_i) {
    
    arg_i->replaceAllUsesWith(&*arg_i2);
    arg_i2->takeName(&*arg_i);        
    ++arg_i2;
    arg_index++;
  }

  bool change_call = true;
  while(change_call) {
    change_call = false;
    
    for(Value::use_iterator ui = func->use_begin(), ue = func->use_end(); 
        ui != ue;) {
      Use &U = *ui;
      
      User* user_call = U.getUser();
      assert(user_call && "user null?");

      CallSite cs(user_call);
      Instruction* call = cs.getInstruction();
      if(!call){
        assert(0 && "Byval attribute and indirect call?");
      }

      CallInst* call_inst = dyn_cast<CallInst>(call);
      assert(call_inst && "call inst null?");

      SmallVector<Value*, 16> call_args;
      CallSite::arg_iterator arg_i = cs.arg_begin();

      for(Function::arg_iterator ib = func->arg_begin(), ie = func->arg_end(); 
          ib != ie; ++ib) {

        Value* call_site_arg = dyn_cast<Value>(arg_i);
        ++arg_i;
                
        Argument* ptr_argument = dyn_cast<Argument>(ib);
        if(!(ptr_argument->hasByValAttr() && 
             checkTypeHasPtrs(ptr_argument))){
          call_args.push_back(call_site_arg);
        }
        else{
          /* byval pointer */

          SequentialType* seq_type = dyn_cast<SequentialType>(call_site_arg->getType());
          assert(seq_type && "byval attribute with non-seq type?");

          StructType* struct_type = dyn_cast<StructType>(seq_type->getElementType());
          assert(struct_type && "non-struct byval parameters?");

          
          AllocaInst* byval_alloca = new AllocaInst(struct_type, "", call);
          /* introduce stores, call_site_arg to byval_alloca */
          
          // introduce an alloca of the pointer type of byval
          // introduce stores
                   
          std::vector<Value*> indices;
          Constant* start_index = ConstantInt::get(Type::getInt64Ty(byval_alloca->getType()->getContext()),
                                         0, false);
          indices.push_back(start_index);

          createGEPStores(byval_alloca, call_site_arg, struct_type, call, indices);
          call_args.push_back(byval_alloca);

        }      
      }

      CallInst* result_call = CallInst::Create(new_func,
                                               call_args, "", call);
      call->replaceAllUsesWith(result_call);
      call->eraseFromParent();
      change_call = true;
      break;
    }
  }
  
  func->eraseFromParent();
  return true;
    
}



bool FixByValAttributes::runOnModule(Module & module) {
  
  
  int LongSize = module.getDataLayout().getPointerSizeInBits();
  
  if (LongSize == 64) {
    m_is_64bit = true;
  } else {
    m_is_64bit = false;
  }

  bool change = true;

  while (change){
    change = false;
    for(Module::iterator ff_begin = module.begin(), ff_end = module.end();
        ff_begin != ff_end; ++ff_begin){
      Function* func_ptr = dyn_cast<Function>(ff_begin);
      assert(func_ptr && "Not a function?");
      change = transformFunction(func_ptr);
      if(change)
        break;
    }
  }
  
  return true;
}
