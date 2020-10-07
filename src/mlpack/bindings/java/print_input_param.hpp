/**
 * @file bindings/java/print_input_param.hpp
 * @author Vasyl Teliman
 *
 * Print Java code to handle input arguments.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_BINDINGS_JAVA_PRINT_INPUT_PARAM_HPP
#define MLPACK_BINDINGS_JAVA_PRINT_INPUT_PARAM_HPP

namespace mlpack {
namespace bindings {
namespace java {

/**
 * Print the input processing (basically calling IO::GetParam<>()) for a
 * non-serializable type.
 */
template<typename T>
void PrintInputParam(
    util::ParamData& d,
    const typename std::enable_if<!data::HasSerialize<T>::value>::type* = 0,
    const typename std::enable_if<!std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type* = 0);

/**
 * Print the input processing for an Armadillo type.
 */
template<typename T>
void PrintInputParam(
    util::ParamData& d,
    const typename std::enable_if<arma::is_arma_type<T>::value>::type* = 0,
    const typename std::enable_if<!std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type* = 0);

/**
 * Print the input processing for a serializable type.
 */
template<typename T>
void PrintInputParam(
    util::ParamData& d,
    const typename std::enable_if<!arma::is_arma_type<T>::value>::type* = 0,
    const typename std::enable_if<data::HasSerialize<T>::value>::type* = 0,
    const typename std::enable_if<!std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type* = 0);

/**
 * Print the input processing (basically calling IO::GetParam<>()) for a
 * matrix with DatasetInfo type.
 */
template<typename T>
void PrintInputParam(
    util::ParamData& d,
    const typename std::enable_if<std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type* = 0);

/**
 * Print the input processing (basically calling IO::GetParam<>()) for a type.
 */
template<typename T>
void PrintInputParam(util::ParamData& d,
                     const void* /* input */,
                     void* /* output */)
{
  // Call out to the right overload.
  PrintInputParam<typename std::remove_pointer<T>::type>(d);
}

} // namespace java
} // namespace bindings
} // namespace mlpack

#include "print_input_param_impl.hpp"

#endif
