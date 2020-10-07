/**
 * @file bindings/java/java_util.hpp
 * @author Vasyl Teliman
 *
 * Various utilities used to generate Java bindings.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_BINDINGS_JAVA_JAVA_UTIL_HPP
#define MLPACK_BINDINGS_JAVA_JAVA_UTIL_HPP

#include <ios>
#include <streambuf>
#include <string>
#include <vector>
#include <mlpack/core/util/param_data.hpp>

namespace mlpack {
namespace bindings {
namespace java {

/**
 * Concise way to redirect streams in C++ using RAII
 */
class RedirectStream
{
 public:
  RedirectStream(std::ios& from, const std::ios& to) : from(from),
      buffer(from.rdbuf(to.rdbuf()))
  {}

  ~RedirectStream()
  {
    from.rdbuf(buffer);
  }

 private:
  std::ios& from;
  std::streambuf* buffer;
};

/**
 * Generate bindings for model parameters
 */
void PrintModelPointers(std::vector<util::ParamData>& in,
                        std::vector<util::ParamData>& out);

} // namespace java
} // namespace bindings
} // namespace mlpack

#endif
