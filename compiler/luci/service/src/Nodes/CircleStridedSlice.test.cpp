/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "luci/Service/CircleNodeClone.h"

#include <gtest/gtest.h>

TEST(CloneNodeTest, clone_StridedSlice)
{
  auto g = loco::make_graph();
  auto node_ss = g->nodes()->create<luci::CircleStridedSlice>();
  node_ss->begin_mask(1);
  node_ss->end_mask(2);
  node_ss->ellipsis_mask(3);
  node_ss->new_axis_mask(4);
  node_ss->shrink_axis_mask(5);

  auto gc = loco::make_graph();
  auto cloned = luci::clone_node(node_ss, gc.get());
  ASSERT_NE(nullptr, cloned);
  ASSERT_EQ(gc.get(), cloned->graph());

  auto cloned_ss = dynamic_cast<luci::CircleStridedSlice *>(cloned);
  ASSERT_NE(nullptr, cloned_ss);
  ASSERT_EQ(node_ss->begin_mask(), cloned_ss->begin_mask());
  ASSERT_EQ(node_ss->end_mask(), cloned_ss->end_mask());
  ASSERT_EQ(node_ss->ellipsis_mask(), cloned_ss->ellipsis_mask());
  ASSERT_EQ(node_ss->new_axis_mask(), cloned_ss->new_axis_mask());
  ASSERT_EQ(node_ss->shrink_axis_mask(), cloned_ss->shrink_axis_mask());
}
