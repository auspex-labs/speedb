//  Copyright (c) Meta Platforms, Inc. and affiliates.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// This file includes the common definitions used in the port/,
// the public API (this directory), and other directories

#pragma once

#include <functional>
#include <memory>
#include <thread>

#include "rocksdb/rocksdb_namespace.h"
namespace ROCKSDB_NAMESPACE {

enum class CpuPriority {
  kIdle = 0,
  kLow = 1,
  kNormal = 2,
  kHigh = 3,
};
namespace port {
class ThreadWithCb {
 public:
  static std::shared_ptr<std::function<void(std::thread::native_handle_type)>>
      on_thread_start_callback;
  template <typename Function, typename... Args>
  ThreadWithCb(Function&& func, Args&&... args) {
    thread_ =
        std::thread(std::forward<Function>(func), std::forward<Args>(args)...);
    if (on_thread_start_callback) {
      on_thread_start_callback->operator()(native_handle());
    }
  }

  ThreadWithCb() {}
  bool joinable() { return thread_.joinable(); }

  void join() { thread_.join(); }

  void detach() { thread_.detach(); }
  std::thread::id get_id() { return thread_.get_id(); }
  std::thread& operator=(std::thread&& __t) {
    thread_ = std::move(__t);
    return thread_;
  }
  std::thread::native_handle_type native_handle() {
    return thread_.native_handle();
  }

 private:
  std::thread thread_;
};
using Thread = ThreadWithCb;
}  // namespace port
}  // namespace ROCKSDB_NAMESPACE
