// Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once
#include "paddle/fluid/framework/new_executor/interpreter_base_impl.h"

DECLARE_bool(new_executor_use_local_scope);

namespace ir {
class Program;
}  // namespace ir

namespace ir {
class Program;
}  // namespace ir

namespace paddle {
namespace framework {

class InterpreterBaseImpl;

class InterpreterCore {
  using ExecutionConfig = interpreter::ExecutionConfig;
  using HookFunc = std::function<void(OperatorBase*, Scope*)>;

 public:
  InterpreterCore(const platform::Place& place,
                  const BlockDesc& block,
                  Scope* scope,
                  const ExecutionConfig& execution_config = ExecutionConfig());
  // This constructor is for New IR.
  InterpreterCore(const platform::Place& place,
                  std::unique_ptr<::ir::Program> ir_prog,
                  Scope* scope,
                  const ExecutionConfig& execution_config = ExecutionConfig());
  ~InterpreterCore();

  const InterpreterBaseImpl* Impl() const { return impl_.get(); }

  paddle::framework::FetchList Run(
      const std::vector<std::string>& feed_names,
      const std::vector<phi::DenseTensor>& feed_tensors);

  paddle::framework::FetchList Run(const std::vector<std::string>& feed_names,
                                   bool need_fetch = true);

  void ShareWorkQueueFrom(std::shared_ptr<InterpreterCore> src);

  void SetCopyProgram(std::shared_ptr<ProgramDesc> prog);

  void SetSkipGcVars(const std::set<std::string>& skip_gc_vars);

  const std::set<std::string>& JitInputVars() const;

  void SetJitInputVars(const std::set<std::string>& jit_input_vars);

  const VariableScope* GetVariableScope() const;

  void reset_scope(Scope* new_scope);

  const platform::Place& GetPlace() const;

  void SetOutputHooks(const std::vector<HookFunc>& hookfuncs);

 private:
  DISABLE_COPY_AND_ASSIGN(InterpreterCore);

  std::unique_ptr<InterpreterBaseImpl> impl_;
};

}  // namespace framework
}  // namespace paddle
