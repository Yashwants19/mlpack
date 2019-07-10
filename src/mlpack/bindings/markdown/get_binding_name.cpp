/**
 * @file get_binding_name.cpp
 * @author Ryan Curtin
 *
 * Given the name of a binding as it appears in CMake, return the corresponding
 * name of the binding that is generated for a given language.
 */
#include "get_binding_name.hpp"

namespace mlpack {
namespace bindings {
namespace markdown {

std::string GetBindingName(const std::string& language,
                           const std::string& name)
{
  // Unfortunately, every time a new binding is added, this code will need to be
  // modified.
  if (language == "cli")
  {
    // For command-line programs, all bindings have 'mlpack_' prepended to the
    // name.
    return "mlpack_" + name;
  }
  else if (language == "python")
  {
    // For Python bindings, the name is unchanged.
    return name;
  }
  else if (language == "julia")
  {
    // For Julia bindings, the name is unchanged.
    return name;
  }
  else
  {
    throw std::invalid_argument("Don't know how to compute binding name for "
        "language \"" + language + "\"!  Is the language specified in "
        "src/mlpack/bindings/markdown/get_binding_name.cpp?");
  }
}

} // namespace markdown
} // namespace bindings
} // namespace mlpack
