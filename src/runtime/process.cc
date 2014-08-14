#include <cassert>
#include <setjmp.h>
#include "../../include/runtime/process.h"
#include "../../include/runtime/sighandler_registrar.h"

#ifndef COREVM_DEFAULT_PROCESS_PAUSE_TIME
  #define COREVM_DEFAULT_PROCESS_PAUSE_TIME 100
#endif


corevm::runtime::process::process():
  sneaker::threading::fixed_time_interval_daemon_service(
    COREVM_DEFAULT_PROCESS_PAUSE_TIME,
    corevm::runtime::process::tick_handler,
    false,
    -1
  )
{
  // Do nothing here.
}

corevm::runtime::process::process(const uint16_t interval):
  sneaker::threading::fixed_time_interval_daemon_service(
    interval,
    corevm::runtime::process::tick_handler,
    false,
    -1
  )
{
  // Do nothing here.
}

corevm::runtime::process::~process()
{
}

const corevm::runtime::instr_addr
corevm::runtime::process::top_addr() const
{
  return _instrs.size() - 1;
}

const corevm::runtime::instr_addr
corevm::runtime::process::current_addr() const
{
  return _pc;
}

uint64_t
corevm::runtime::process::call_stack_size() const
{
  return _call_stack.size();
}

bool
corevm::runtime::process::has_frame() const
{
  return !this->_call_stack.empty();
}

corevm::runtime::frame&
corevm::runtime::process::top_frame() throw(corevm::runtime::frame_not_found_error)
{
  if(_call_stack.empty()) {
    throw corevm::runtime::frame_not_found_error();
  }

  return _call_stack.top();
}

void
corevm::runtime::process::pop_frame() throw(corevm::runtime::frame_not_found_error)
{
  if(_call_stack.empty()) {
    throw corevm::runtime::frame_not_found_error();
  }

  _call_stack.pop();
}

void
corevm::runtime::process::push_frame(corevm::runtime::frame& frame)
{
  _call_stack.push(frame);
}

uint64_t
corevm::runtime::process::stack_size() const
{
  return _dyobj_stack.size();
}

const corevm::dyobj::dyobj_id&
corevm::runtime::process::top_stack() throw(corevm::runtime::object_stack_empty_error)
{
  if(_dyobj_stack.empty()) {
    throw corevm::runtime::object_stack_empty_error();
  }

  return _dyobj_stack.top();
}

void
corevm::runtime::process::push_stack(corevm::dyobj::dyobj_id& id)
{
  _dyobj_stack.push(id);
}

const corevm::dyobj::dyobj_id
corevm::runtime::process::pop_stack() throw(corevm::runtime::object_stack_empty_error)
{
  if(_dyobj_stack.empty()) {
    throw corevm::runtime::object_stack_empty_error();
  }

  corevm::dyobj::dyobj_id id = _dyobj_stack.top();
  _dyobj_stack.pop();
  return id;
}

bool
corevm::runtime::process::has_ntvhndl(corevm::dyobj::ntvhndl_key& key)
{
  return _ntv_handles_pool.find(key) != _ntv_handles_pool.end();
}

corevm::types::native_type_handle&
corevm::runtime::process::get_ntvhndl(corevm::dyobj::ntvhndl_key& key)
  throw(corevm::runtime::native_type_handle_not_found_error)
{
  try {
    return _ntv_handles_pool.at(key);  
  } catch (...)  {// TODO: should catch `std::out_of_range` here.
    throw corevm::runtime::native_type_handle_not_found_error();
  }
}

corevm::dyobj::ntvhndl_key
corevm::runtime::process::insert_ntvhndl(corevm::types::native_type_handle& hndl)
{
  corevm::dyobj::ntvhndl_key key = ++_ntv_handles_incrementor;

  // TODO: not sure if we want to do .insert here or simply use the access operator...
  _ntv_handles_pool.insert(
    std::pair<corevm::dyobj::ntvhndl_key, corevm::types::native_type_handle>(key, hndl)
  );

  return key;
}

void
corevm::runtime::process::erase_ntvhndl(corevm::dyobj::ntvhndl_key& key)
  throw(corevm::runtime::native_type_handle_deletion_error)
{
  size_t res = _ntv_handles_pool.erase(key);

  if(res == 0) {
    throw corevm::runtime::native_type_handle_deletion_error();
  }
}

const corevm::runtime::instr_handler*
corevm::runtime::process::get_instr_handler(corevm::runtime::instr_code code)
{
  corevm::runtime::instr_info instr_info = _instr_handler_meta.find(code);
  return instr_info.handler;
}

void
corevm::runtime::process::pause_exec()
{
  _pause_exec = true;
}

void
corevm::runtime::process::resume_exec()
{
  _pause_exec = false;
}

bool
corevm::runtime::process::can_execute()
{
  return _pc < _instrs.size();
}

bool
corevm::runtime::process::start()
{
  bool res = sneaker::threading::fixed_time_interval_daemon_service::start();

  while(can_execute()) {
    while(_pause_exec) {}

    corevm::runtime::instr instr = static_cast<corevm::runtime::instr>(_instrs[_pc]);

    corevm::runtime::instr_handler* handler =
      const_cast<corevm::runtime::instr_handler*>(this->get_instr_handler(instr.code));

    sigsetjmp(corevm::runtime::sighandler_registrar::get_sigjmp_env(), 1);

    if(!corevm::runtime::sighandler_registrar::sig_raised) {
      handler->execute(instr, *this);
    } else {
      if(!can_execute()) {
        break;
      }
    }
    corevm::runtime::sighandler_registrar::sig_raised = false;

    ++_pc;
  }

  return res;
}

void
corevm::runtime::process::maybe_gc()
{
  if(!_should_gc()) {
    return;
  }

  this->pause_exec();

  // GC...
  corevm::gc::garbage_collector<garbage_collection_scheme> garbage_collector(_dynamic_object_heap);
  garbage_collector.gc();

  this->resume_exec();
}

void
corevm::runtime::process::set_pc(const corevm::runtime::instr_addr addr)
  throw(corevm::runtime::invalid_instr_addr_error)
{
  if(addr >= _instrs.size()) {
    throw corevm::runtime::invalid_instr_addr_error();
  }

  _pc = addr;

  while(this->has_frame()) {
    corevm::runtime::frame& frame = this->top_frame();

    if(! (frame.get_start_addr() > addr) ) {
      break;
    }

    this->pop_frame();
  }
}

void
corevm::runtime::process::append_instrs(const std::vector<corevm::runtime::instr>& instrs)
{
  _instrs.insert(_instrs.end(), instrs.begin(), instrs.end());
}

void
corevm::runtime::process::append_instr_block(const corevm::runtime::instr_block& block)
{
  _instr_blocks.push_back(block);
}

bool
corevm::runtime::process::_should_gc()
{
  size_t flag_size = sizeof(_gc_flag) * 8;

  for(int i = 0; i < flag_size; ++i) {
    bool bit_set = is_bit_set(_gc_flag, i);

    if(!bit_set) {
      continue;
    }

    corevm::runtime::gc_rule_meta::gc_bitfields bit = static_cast<corevm::runtime::gc_rule_meta::gc_bitfields>(i);
    corevm::runtime::gc_rule* gc_rule = _gc_rule_meta.get_gc_rule(bit);

    if(gc_rule && gc_rule->should_gc(const_cast<const corevm::runtime::process&>(*this))) {
      return true;
    }
  }

  return false;
}

void
corevm::runtime::process::tick_handler(void* arg)
{
  assert(arg);
  corevm::runtime::process* process = static_cast<corevm::runtime::process*>(arg);
  process->maybe_gc();
}

void
corevm::runtime::process::set_sig_instr_block(
  sig_atomic_t sig, corevm::runtime::instr_block& block)
{
  _sig_instr_map.insert({sig, block});
}

void
corevm::runtime::process::_insert_instr_block(corevm::runtime::instr_block& block)
{
  std::vector<corevm::runtime::instr>::iterator pos = _instrs.begin() + _pc + 1;
  _instrs.insert(pos, block.begin(), block.end()); 
}

void
corevm::runtime::process::handle_signal(
  sig_atomic_t sig, corevm::runtime::sighandler* handler)
{
  auto itr = _sig_instr_map.find(sig);

  if(itr != _sig_instr_map.end()) {
    corevm::runtime::instr_block block = itr->second;
    this->pause_exec();
    this->_insert_instr_block(block);
    this->resume_exec();

  } else if(handler != nullptr) {
    handler->handle_signal(sig, *this);
  }
}
