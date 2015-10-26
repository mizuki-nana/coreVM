/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/
#include "instr_info.h"

#include <iomanip>

#include <boost/timer/timer.hpp>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

struct instr_measurement
{
  instr_measurement()
    :
    cumulative_wall_time(0u),
    invocation_count(0u)
  {
  }

  boost::timer::nanosecond_type cumulative_wall_time;
  uint64_t invocation_count;
};

// -----------------------------------------------------------------------------

template<typename T, typename ContainerType>
T _sum(const ContainerType& measurements)
{
  T res = 0;

  for (const auto& measurement : measurements)
  {
    res += measurement.cumulative_wall_time;
  }

  return res;
}

// -----------------------------------------------------------------------------

boost::timer::nanosecond_type
_avg(const instr_measurement& measurement)
{
  const auto invocation_count = measurement.invocation_count;

  if (invocation_count == 0)
  {
    return 0;
  }

  return static_cast<boost::timer::nanosecond_type>(
    static_cast<size_t>(measurement.cumulative_wall_time) / invocation_count);
}

// -----------------------------------------------------------------------------

template<typename T>
double _percantage(T n, T total)
{
  return static_cast<double>(n) / static_cast<double>(total) * 100.0;
}

// -----------------------------------------------------------------------------

template<typename ContainerType>
void pretty_print_measurements(const ContainerType& measurements)
{
  static const uint32_t INSTR_NAME_WIDTH = 10u;
  static const uint32_t PERCENTAGE_WIDTH = 6u;
  static const uint32_t INVOCATION_COUNT_WIDTH  = 11u;
  static const uint32_t CUMULATIVE_WALL_TIME_WIDTH = 30u;
  static const uint32_t AVG_CUMULATIVE_WALL_TIME_WIDTH = 24u;
  static const char* BAR = " | ";
  static const char* LINE_SEPARATOR =
    "--------------------------------------------------------------------------"
    "-------------------";

  std::cout << LINE_SEPARATOR << std::endl;
  std::cout << "Instrumental instruction measurements:" << std::endl;
  std::cout << std::endl;

  const auto total_wall_time = _sum<boost::timer::nanosecond_type>(measurements);
  std::cout << "  Total elapsed CPU wall clock time (ns): " << total_wall_time << std::endl;
  std::cout << std::endl;

  std::cout << std::setw(INSTR_NAME_WIDTH) << "Instr" << BAR;
  std::cout << std::setw(PERCENTAGE_WIDTH) << "%" << BAR;
  std::cout << std::setw(INVOCATION_COUNT_WIDTH) << "Invocations" << BAR;
  std::cout << std::setw(CUMULATIVE_WALL_TIME_WIDTH) << "Cumulated wall clock time (ns)" << BAR;
  std::cout << std::setw(AVG_CUMULATIVE_WALL_TIME_WIDTH) << "Avg wall clock time (ns) ";
  std::cout << std::endl;

  for (size_t i = 0; i < corevm::runtime::INSTR_CODE_MAX; ++i)
  {
    const auto& measurement = measurements[i];

    const auto cumulative_wall_time = measurement.cumulative_wall_time;
    const auto cumulative_wall_time_percentage = _percantage(cumulative_wall_time, total_wall_time);
    const auto invocation_count = measurement.invocation_count;
    const auto avg_wall_time = _avg(measurement);

    std::cout << std::setw(INSTR_NAME_WIDTH) << corevm::runtime::instr_set_info::instr_infos[i].name << BAR;
    std::cout << std::setw(PERCENTAGE_WIDTH) << std::setprecision(2) << cumulative_wall_time_percentage << std::fixed << BAR;
    std::cout << std::setw(INVOCATION_COUNT_WIDTH) << invocation_count << BAR;
    std::cout << std::setw(CUMULATIVE_WALL_TIME_WIDTH) << cumulative_wall_time << BAR;
    std::cout << std::setw(AVG_CUMULATIVE_WALL_TIME_WIDTH) << avg_wall_time;
    std::cout << std::endl;
  }

  std::cout << std::endl;
  std::cout << LINE_SEPARATOR << std::endl;
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
