/**
 * @file bindings/R/print_input_param.hpp
 * @author Yashwant Singh Parihar
 *
 * Print the declaration of an input parameter as part of a line in a R
 * function definition.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_BINDINGS_R_PRINT_INPUT_PARAM_HPP
#define MLPACK_BINDINGS_R_PRINT_INPUT_PARAM_HPP

namespace mlpack {
namespace bindings {
namespace r {

/**
 * Print the declaration of an input parameter as part of a line in a R
 * function definition.  This doesn't include any commas or anything.
 */
template<typename T>
void PrintInputParam(const util::ParamData& d,
                     const void* /* input */,
                     void* /* output */)
{
  std::cout << d.name;
  if (std::is_same<T, bool>::value)
    std::cout << "=FALSE";
  else if (!d.required)
    std::cout << "=NA";
}

} // namespace r
} // namespace bindings
} // namespace mlpack

#endif
