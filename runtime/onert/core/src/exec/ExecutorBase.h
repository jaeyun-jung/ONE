/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ONERT_EXEC_EXECUTOR_BASE_H__
#define __ONERT_EXEC_EXECUTOR_BASE_H__

#include "IPermuteFunction.h"
#include "exec/IExecutor.h"
#include "exec/ExecTime.h"
#include "exec/ExecutionObservee.h"
#include "exec/IFunction.h"
#include "exec/IODescription.h"
#include "ir/Graph.h"
#include "ir/Index.h"
#include "ir/LowerInfoMap.h"
#include "ir/OperationIndexMap.h"
#include "compiler/LoweredGraph.h"
#include "compiler/TensorRegistries.h"
#include "backend/ITensor.h"

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

namespace onert
{
namespace exec
{

class ExecutorBase : public IExecutor
{
public:
  /**
   * @brief Construct a new ExecutorBase object
   * @param graph Graph object
   * @param tensor_builders Tensor builders that are currently used
   */
  ExecutorBase(std::unique_ptr<compiler::LoweredGraph> &&lowered_graph,
               const std::vector<backend::ITensor *> &input_tensors,
               const std::vector<backend::ITensor *> &output_tensors,
               const compiler::TensorRegistries &tensor_regs);

  virtual ~ExecutorBase() = default;

  const ir::Graph &graph() final { return _graph; }

  /**
   * @brief Execute without IODescription
   *
   * @param src_tensor Tensor list that will be copied to input tensors of this
   * @param pre_fn The permutation function that copy from src_tensor to input tensors of this
   */
  void execute(const std::vector<backend::ITensor *> &src_tensors,
               const std::shared_ptr<IPermuteFunction> &pre_fn);

  void execute(const IODescription &desc) final;

  // Used only in Dataflow and Parallel Executors
  void setIndexedRanks(std::shared_ptr<ir::OperationIndexMap<int64_t>> ranks) final
  {
    _indexed_ranks = std::move(ranks);
  };

  virtual void executeImpl(void) = 0;

  void addObserver(std::unique_ptr<IExecutionObserver> ref) { _subject.add(std::move(ref)); };

  const std::vector<backend::ITensor *> &getInputTensors() const { return _input_tensors; }

  const std::vector<backend::ITensor *> &getOutputTensors() const { return _output_tensors; }

protected:
  /**
   * @brief Returns @c true if any input tensor is dynamic; @c false if all are static tensors
   */
  bool hasDynamicInput();

protected:
  ExecutionObservee _subject;
  std::shared_ptr<ir::OperationIndexMap<int64_t>> _indexed_ranks;
  std::unique_ptr<compiler::LoweredGraph> _lowered_graph;
  const ir::Graph &_graph;
  std::vector<backend::ITensor *> _input_tensors;
  std::vector<backend::ITensor *> _output_tensors;
  std::mutex _mutex;

private:
  void handleDynamicInputTensor(ir::IOIndex input_index, const IODescription &desc);
};

} // namespace exec
} // namespace onert

#endif // __ONERT_EXEC_EXECUTOR_BASE_H__
