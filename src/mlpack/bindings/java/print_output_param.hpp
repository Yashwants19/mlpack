/**
 * @file bindings/java/print_output_param.hpp
 * @author Vasyl Teliman
 *
 * Print Java code to handle output arguments.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_BINDINGS_JAVA_PRINT_OUTPUT_PARAM_HPP
#define MLPACK_BINDINGS_JAVA_PRINT_OUTPUT_PARAM_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace bindings {
namespace java {

/**
 * Print the output processing (basically calling IO::GetParam<>()) for a
 * non-serializable type.
 */
template<typename T>
void PrintOutputParam(
    util::ParamData& d,
    const typename std::enable_if<!data::HasSerialize<T>::value>::type* = 0,
    const typename std::enable_if<!std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type* = 0);

/**
 * Print the output processing for an Armadillo type.
 */
template<typename T>
void PrintOutputParam(
    util::ParamData& d,
    const typename std::enable_if<arma::is_arma_type<T>::value>::type* = 0,
    const typename std::enable_if<!std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type* = 0);

/**
 * Print the output processing for a serializable type.
 */
template<typename T>
void PrintOutputParam(
    util::ParamData& d,
    const typename std::enable_if<!arma::is_arma_type<T>::value>::type* = 0,
    const typename std::enable_if<data::HasSerialize<T>::value>::type* = 0,
    const typename std::enable_if<!std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type* = 0);

/**
 * Print the output processing for a mat/DatasetInfo tuple type.
 */
template<typename T>
void PrintOutputParam(
    util::ParamData& d,
    const typename std::enable_if<std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type* = 0);

/**
 * Print the output processing (basically calling IO::GetParam<>()) for a type.
 */
template<typename T>
void PrintOutputParam(util::ParamData& d,
                      const void* /* input */,
                      void* /* output */)
{
  // Call out to the right overload.
  PrintOutputParam<typename std::remove_pointer<T>::type>(d);
}

} // namespace java
} // namespace bindings
} // namespace mlpack

#include "print_output_param_impl.hpp"

#endif
