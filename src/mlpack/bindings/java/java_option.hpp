/**
 * @file bindings/java/java_option.hpp
 * @author Vasyl Teliman
 *
 * The Java option type.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_BINDINGS_JAVA_JAVA_OPTION_HPP
#define MLPACK_BINDINGS_JAVA_JAVA_OPTION_HPP

#include <mlpack/core/util/param_data.hpp>
#include <mlpack/core/util/io.hpp>
#include "print_input_param.hpp"
#include "print_output_param.hpp"
#include "print_param_defn.hpp"

namespace mlpack {
namespace bindings {
namespace java {

// Defined in mlpack_main.hpp.
extern std::string programName;

/**
 * The Java option class.
 */
template<typename T>
class JavaOption
{
 public:
  /**
   * Construct a ROption object.  When constructed, it will register itself
   * with IO. The testName parameter is not used and added for compatibility
   * reasons.
   *
   * @param defaultValue Default value this parameter will be initialized to
   *      (for flags, this should be false, for instance).
   * @param identifier The name of the option (no dashes in front; for --help,
   *      we would pass "help").
   * @param description A short string describing the option.
   * @param alias Short name of the parameter. "" for no alias.
   * @param cppName Name of the C++ type of this parameter (i.e. "int").
   * @param required Whether or not the option is required at runtime.
   * @param input Whether or not the option is an input option.
   * @param noTranspose If the parameter is a matrix and this is true, then the
   *      matrix will not be transposed on loading.
   * @param * (testName) Is not used and added for compatibility reasons.
   */
  JavaOption(const T defaultValue,
             const std::string& identifier,
             const std::string& description,
             const std::string& alias,
             const std::string& cppName,
             const bool required = false,
             const bool input = true,
             const bool noTranspose = false,
             const std::string& /*testName*/ = "")
  {
    // Create the ParamData object to give to IO.
    util::ParamData data;

    data.desc = description;
    data.name = identifier;
    data.tname = TYPENAME(T);
    data.alias = alias[0];
    data.wasPassed = false;
    data.noTranspose = noTranspose;
    data.required = required;
    data.input = input;
    data.loaded = false;
    // Only "verbose" and "copy_all_inputs" will be persistent.
    // copy_all_inputs is not available in Java
    if (identifier == "verbose" /*|| identifier == "copy_all_inputs"*/)
      data.persistent = true;
    else
      data.persistent = false;
    data.cppType = cppName;

    // Every parameter we'll get from Python will have the correct type.
    data.value = boost::any(defaultValue);

    // Restore the parameters for this program.
    if (identifier != "verbose" /*&& identifier != "copy_all_inputs"*/)
      IO::RestoreSettings(programName, false);

    // Set the function pointers that we'll need.  All of these function
    // pointers will be used by both the program that generates the pyx, and
    // also the binding itself.  (The binding itself will only use GetParam,
    // GetPrintableParam, and GetRawParam.)

    IO::GetSingleton().functionMap[data.tname]["PrintParamDefn"] =
        &PrintParamDefn<T>;
    IO::GetSingleton().functionMap[data.tname]["PrintInputParam"] =
        &PrintInputParam<T>;
    IO::GetSingleton().functionMap[data.tname]["PrintOutputParam"] =
        &PrintOutputParam<T>;
    IO::GetSingleton().functionMap[data.tname]["GetJavaType"] =
        &GetJavaType<T>;

    // Add the ParamData object, then store.  This is necessary because we may
    // import more than one .so that uses IO, so we have to keep the options
    // separate.  programName is a global variable from mlpack_main.hpp.
    IO::Add(std::move(data));
    if (identifier != "verbose" /*&& identifier != "copy_all_inputs"*/)
      IO::StoreSettings(programName);
    IO::ClearSettings();
  }
};

} // namespace java
} // namespace bindings
} // namespace mlpack

#endif
