/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include <vector>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace record_minmax
{

/**
 * @brief  getNthPercentile calculates the n-th percentile of input vector (0.0 <= n <= 100.0)
 *         linear interpolation is used when the desired percentile lies between two data points
 */
float getNthPercentile(std::vector<float> &vector, float percentile)
{
  if (percentile < 0 || percentile > 100)
    throw std::runtime_error("Percentile must be ranged from 0 to 100");

  if (percentile == 0.0)
    return vector.front();

  if (percentile == 100.0)
    return vector.back();

  if (vector.empty())
    throw std::runtime_error("Percentile must take a non-empty vector as an argument");

  if (vector.size() == 1)
    return vector[0];

  std::vector<float> copy;
  copy.assign(vector.begin(), vector.end());
  std::sort(copy.begin(), copy.end());

  int index = static_cast<int>(std::floor((copy.size() - 1) * percentile / 100.0));

  float percent_i = static_cast<float>(index) / static_cast<float>(copy.size() - 1);
  float fraction =
      (percentile / 100.0 - percent_i) / ((index + 1.0) / (copy.size() - 1.0) - percent_i);
  float res = copy[index] + fraction * (copy[index + 1] - copy[index]);
  return res;
}

/**
 * @brief  getMovingAverage calculates the weighted moving average of input vector
 *         The initial value is the first element of the vector
 */
float getMovingAverage(std::vector<float> &vector, float alpha)
{
  assert(!vector.empty());
  assert(alpha >= 0.0 && alpha <= 1.0);

  float curr_avg = vector[0];
  for (size_t i = 1; i < vector.size(); i++)
  {
    curr_avg = curr_avg * alpha + vector[i] * (1.0 - alpha);
  }
  return curr_avg;
}

} // namespace record_minmax
