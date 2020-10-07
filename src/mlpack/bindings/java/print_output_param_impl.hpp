/**
 * @file bindings/java/print_output_param_impl.hpp
 * @author VasylTeliman
 *
 * Print Java code to handle output arguments.
 */
#ifndef MLPACK_BINDINGS_JAVA_PRINT_OUTPUT_PARAM_IMPL_HPP
#define MLPACK_BINDINGS_JAVA_PRINT_OUTPUT_PARAM_IMPL_HPP

#include "print_output_param.hpp"
#include "get_java_type.hpp"
#include <mlpack/bindings/util/strip_type.hpp>

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
    const typename std::enable_if<!data::HasSerialize<T>::value>::type*,
    const typename std::enable_if<!std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type*)
{
  std::string type;
  if (std::is_same<T, std::vector<std::string>>::value)
    type = "VecString";
  else if (std::is_same<T, std::vector<int>>::value)
    type = "VecInt";
  else
    type = GetJavaType<T>(d);

  std::cout << "    params.put(\"" << d.name << "\", IO.get"
            << type << "Param(\"" << d.name << "\"));" << std::endl;
}

/**
 * Print the output processing for an Armadillo type.
 */
template<typename T>
void PrintOutputParam(
    util::ParamData& d,
    const typename std::enable_if<arma::is_arma_type<T>::value>::type*,
    const typename std::enable_if<!std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type*)
{
  std::string uChar = (std::is_same<typename T::elem_type, size_t>::value) ?
      "U" : "";
  std::string matTypeSuffix = "";
  std::string extra = "";
  if (T::is_row)
  {
    matTypeSuffix = "Row";
  }
  else if (T::is_col)
  {
    matTypeSuffix = "Col";
  }
  else
  {
    matTypeSuffix = "Mat";
  }

  std::cout << "    params.put(\"" << d.name << "\", IO.get" << uChar
            << matTypeSuffix << "Param(\"" << d.name << "\"));" << std::endl;
}

/**
 * Print the output processing for a serializable type.
 */
template<typename T>
void PrintOutputParam(
    util::ParamData& d,
    const typename std::enable_if<!arma::is_arma_type<T>::value>::type*,
    const typename std::enable_if<data::HasSerialize<T>::value>::type*,
    const typename std::enable_if<!std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type*)
{
  std::string type = util::StripType(d.cppType);

  std::cout << "    params.put(\"" << d.name << "\", new " << type << "Type("
            << std::endl
            << "        " << type << "Ptr.create(get" << type << "Ptr(\""
            << d.name << "\"))));" << std::endl;
}

/**
 * Print the output processing for a mat/DatasetInfo tuple type.
 */
template<typename T>
void PrintOutputParam(
    util::ParamData& d,
    const typename std::enable_if<std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>::value>::type*)
{
  std::cout << "    params.put(\"" << d.name << "\", IO.getMatWithInfoParam(\""
            << d.name << "\"));" << std::endl;
}

} // namespace java
} // namespace bindings
} // namespace mlpack

#endif
