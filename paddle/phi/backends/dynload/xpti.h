/* Copyright (c) 2023 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */
#pragma once

#ifdef PADDLE_WITH_XPU

#include <xpu/xpti.h>

#include <mutex>  // NOLINT

#include "paddle/phi/backends/dynload/dynamic_loader.h"
#include "paddle/phi/backends/dynload/port.h"

namespace phi {
namespace dynload {

extern std::once_flag xpti_dso_flag;
extern void *xpti_dso_handle;

#define DECLARE_DYNAMIC_LOAD_XPTI_WRAP(__name)                   \
  struct DynLoad__##__name {                                     \
    template <typename... Args>                                  \
    XPTIResult operator()(Args... args) {                        \
      using xptiFunc = decltype(&::__name);                      \
      std::call_once(xpti_dso_flag, []() {                       \
        xpti_dso_handle = phi::dynload::GetXPTIDsoHandle();      \
      });                                                        \
      static void *p_##__name = dlsym(xpti_dso_handle, #__name); \
      return reinterpret_cast<xptiFunc>(p_##__name)(args...);    \
    }                                                            \
  };                                                             \
  extern DynLoad__##__name __name

#define XPTI_ROUTINE_EACH(__macro) \
  __macro(xptiActivityEnable);     \
  __macro(xptiActivityDisable);    \
  __macro(xptiStartTracing);       \
  __macro(xptiStopTracing);        \
  __macro(xptiActivityFlushAll);   \
  __macro(xptiActivityGetNextRecord);

XPTI_ROUTINE_EACH(DECLARE_DYNAMIC_LOAD_XPTI_WRAP);

#undef DECLARE_DYNAMIC_LOAD_XPTI_WRAP
}  // namespace dynload
}  // namespace phi

#endif  // PADDLE_WITH_XPTI
