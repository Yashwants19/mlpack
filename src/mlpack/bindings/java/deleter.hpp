/**
 * @file bindings/java/deleter.hpp
 * @author Vasyl Teliman
 *
 * Utility functions used to clear data.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_BINDINGS_JAVA_DELETER_HPP
#define MLPACK_BINDINGS_JAVA_DELETER_HPP

#include <type_traits>

namespace mlpack {
namespace util {

/**
 * Delete data for a pointer to a non-array type.
 * It is assumed that data was allocated using new keyword
 */
template <typename T>
typename std::enable_if<!std::is_array<T>::value>::type Delete(void* p)
{
  delete static_cast<T*>(p);
}

/**
 * Delete data for a pointer to an array type.
 * Data is assumed to be allocated using new[] keyword
 */
template <typename T>
typename std::enable_if<std::is_array<T>::value>::type Delete(void* p)
{
  using U = typename std::remove_all_extents<T>::type;
  delete[] static_cast<U*>(p);
}

} // namespace util
} // namespace mlpack

#endif
