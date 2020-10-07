/**
 * @file bindings/java/java_util.hpp
 * @author Vasyl Teliman
 *
 * Implementation of various utilities used to generate Java bindings.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#include <unordered_set>
#include <mlpack/core/util/io.hpp>
#include <cctype>
#include "java_util.hpp"

namespace mlpack {
namespace bindings {
namespace java {

/**
 * Generate bindings for model parameters
 */
void PrintModelPointers(std::vector<util::ParamData>& in,
                        std::vector<util::ParamData>& out)
{
  std::unordered_set<std::string> types;

  for (util::ParamData& param : in)
  {
    if (types.count(param.cppType) == 0)
    {
      types.insert(param.cppType);
      IO::GetSingleton().functionMap[param.tname]["PrintParamDefn"](param,
          nullptr, nullptr);
    }
  }

  for (util::ParamData& param : out)
  {
    if (types.count(param.cppType) == 0)
    {
      types.insert(param.cppType);
      IO::GetSingleton().functionMap[param.tname]["PrintParamDefn"](param,
          nullptr, nullptr);
    }
  }
}

} // namespace java
} // namespace bindings
} // namespace mlpack
