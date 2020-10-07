/**
 * @file bindings/java/io_util.hpp
 * @author Vasyl Teliman
 *
 * Utility functions for Java bindings.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_BINDINGS_JAVA_IO_UTIL_HPP
#define MLPACK_BINDINGS_JAVA_IO_UTIL_HPP

#include <mlpack/core/util/io.hpp>

namespace mlpack {
namespace util {

/**
 * Sets matrix data.
 */
template <typename T>
void SetMatParam(const char* name, T* data, size_t rows, size_t columns)
{
  arma::Mat<T> m(data, rows, columns, false, true);
  IO::GetParam<arma::Mat<T>>(name) = std::move(m);
}

/**
 * Sets row data
 */
template <typename T>
void SetRowParam(const char* name, T* data, size_t length)
{
  arma::Row<T> m(data, length, false, true);
  IO::GetParam<arma::Row<T>>(name) = std::move(m);
}

/**
 * Sets column data
 */
template <typename T>
void SetColParam(const char* name, T* data, size_t length)
{
  arma::Col<T> m(data, length, false, true);
  IO::GetParam<arma::Col<T>>(name) = std::move(m);
}

/**
 * Sets other kinds of parameters (strings, integers...)
 */
template <typename T>
void SetParam(const char* name, T value)
{
  IO::GetParam<T>(name) = std::move(value);
}

/**
 * Sets data for matrix with categorical values
 */
void SetMatWithInfoParam(
    const char* name, double* data, bool* info, size_t rows, size_t columns)
{
  data::DatasetInfo d(rows);
  for (size_t i = 0; i < d.Dimensionality(); ++i)
  {
    d.Type(i) = (info[i]) ? data::Datatype::categorical :
        data::Datatype::numeric;
  }

  arma::mat m(data, rows, columns, false, true);
  std::get<0>(IO::GetParam<std::tuple<data::DatasetInfo, arma::mat>>(
      name)) = std::move(d);
  std::get<1>(IO::GetParam<std::tuple<data::DatasetInfo, arma::mat>>(
      name)) = std::move(m);
}

/**
 * Gets data for simple parameters (strings, integers...)
 */
template <typename T>
T GetParam(const char* name)
{
  return std::move(IO::GetParam<T>(name));
}

/**
 * Gets data for matrix parameter
 */
template <typename T>
typename T::elem_type* GetArmaParamData(const char* name)
{
  T& param = IO::GetParam<T>(name);

  // copy inlined data
  if (param.mem && param.n_elem <= arma::arma_config::mat_prealloc)
  {
    using R = typename T::elem_type;
    R* result = new R[param.n_elem];
    arma::arrayops::copy(result, param.mem, param.n_elem);
    return result;
  }

  arma::access::rw(param.mem_state) = 1;
  return param.memptr();
}

/**
 * Get number of rows in a matrix or a vector
 */
template <typename T>
size_t GetArmaParamRows(const char* name)
{
  return IO::GetParam<T>(name).n_rows;
}

/**
 * Get number of columns in a matrix or a vector
 */
template <typename T>
size_t GetArmaParamColumns(const char* name)
{
  return IO::GetParam<T>(name).n_cols;
}

/**
 * Get number of elements in a matrix or a vector
 */
template <typename T>
size_t GetArmaParamLength(const char* name)
{
  return IO::GetParam<T>(name).n_elem;
}

/**
 * Sets vector elements
 */
template <typename T>
void SetVecElement(const char* name, int i, T element)
{
  IO::GetParam<std::vector<T>>(name)[i] = std::move(element);
}

/**
 * Resizes vector appropriately
 */
template <typename T>
void SetVecSize(const char* name, int size)
{
  IO::GetParam<std::vector<T>>(name).resize(size);
}

/**
 * Gets vector size
 */
template <typename T>
int GetVecSize(const char* name)
{
  return static_cast<int>(IO::GetParam<std::vector<T>>(name).size());
}

/**
 * Gets vector element
 */
template <typename T>
T GetVecElement(const char* name, int i)
{
  return std::move(IO::GetParam<std::vector<T>>(name)[i]);
}

/**
 * Set an argument as passed to the IO object.
 */
void SetPassed(const char* name)
{
  IO::SetPassed(name);
}

/**
 * Restores saved settings for a program with specific name
 */
void RestoreSettings(const char* name)
{
  IO::RestoreSettings(name);
}

/**
 * Get pointer to the data of a categorical matrix.
 * Calling function is expected to own the memory
 */
double* GetMatWithInfoParamData(const char* name)
{
  arma::mat& param = std::get<1>(
      IO::GetParam<std::tuple<data::DatasetInfo, arma::mat>>(name));

  if (param.mem && param.n_elem <= arma::arma_config::mat_prealloc)
  {
    double* result = new double[param.n_elem];
    arma::arrayops::copy(result, param.mem, param.n_elem);
    return result;
  }

  arma::access::rw(param.mem_state) = 1;
  return param.memptr();
}

/**
 * Get number of columns in a categorical matrix
 */
size_t GetMatWithInfoParamCols(const char* name)
{
  return std::get<1>(
      IO::GetParam<std::tuple<data::DatasetInfo, arma::mat>>(name)).n_cols;
}

/**
 * Get number of rows in a categorical matrix
 */
size_t GetMatWithInfoParamRows(const char* name)
{
  return std::get<1>(
      IO::GetParam<std::tuple<data::DatasetInfo, arma::mat>>(name)).n_rows;
}

/**
 * Get number of elements in a categorical matrix
 */
size_t GetMatWithInfoParamLength(const char* name)
{
  return std::get<1>(
      IO::GetParam<std::tuple<data::DatasetInfo, arma::mat>>(name)).n_elem;
}

/**
 * Get pointer to the info of a categorical matrix.
 * Calling function is expected to own the memory
 */
bool* GetMatWithInfoParamInfo(const char* name)
{
  const data::DatasetInfo& info = std::get<0>(
      IO::GetParam<std::tuple<data::DatasetInfo, arma::mat>>(name));

  size_t n = info.Dimensionality();
  bool* result = new bool[n];

  for (size_t i = 0; i < n; ++i)
  {
    result[i] = info.Type(i) == data::Datatype::categorical;
  }

  return result;
}

/**
 * Enable verbose output.
 */
void EnableVerbose()
{
  Log::Info.ignoreInput = false;
}

/**
 * Disable verbose output.
 */
void DisableVerbose()
{
  Log::Info.ignoreInput = true;
}

} // namespace util
} // namespace mlpack

#endif
