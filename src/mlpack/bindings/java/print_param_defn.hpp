/**
 * @file bindings/java/print_param_defn.hpp
 * @author Ryan Curtin
 * @author Vasyl Teliman
 *
 * If the type is serializable, we need to define a special utility function to
 * set a IO parameter of that type.
 */
#ifndef MLPACK_BINDINGS_JAVA_PRINT_PARAM_DEFN_HPP
#define MLPACK_BINDINGS_JAVA_PRINT_PARAM_DEFN_HPP

#include <fstream>
#include <mlpack/bindings/util/strip_type.hpp>

namespace mlpack {
namespace bindings {
namespace java {

/**
 * If the type is not serializable, print nothing.
 */
template<typename T>
void PrintParamDefn(
    util::ParamData& /* d */,
    const typename std::enable_if<!arma::is_arma_type<T>::value>::type* = 0,
    const typename std::enable_if<!data::HasSerialize<T>::value>::type* = 0)
{
  // Do nothing.
}

/**
 * Matrices are serializable but here we also print nothing.
 */
template<typename T>
void PrintParamDefn(
    util::ParamData& /* d */,
    const typename std::enable_if<arma::is_arma_type<T>::value>::type* = 0)
{
  // Do nothing.
}

/**
 * For non-matrix serializable types we need to print something.
 */
template<typename T>
void PrintParamDefn(
    util::ParamData& d,
    const typename std::enable_if<!arma::is_arma_type<T>::value>::type* = 0,
    const typename std::enable_if<data::HasSerialize<T>::value>::type* = 0)
{
  std::string ptr = util::StripType(d.cppType) + "Ptr";
  std::string type = util::StripType(d.cppType) + "Type";
  std::cout << "  private static class " << ptr << " extends Pointer {" 
            << std::endl
            << "    private static class MethodDeallocator" << std::endl
            << "        extends " << ptr << " implements Deallocator {"
            << std::endl
            << "      private MethodDeallocator(" << ptr << " p) {" << std::endl
            << "        super(p);" << std::endl
            << "      }" << std::endl
            << std::endl
            << "      @Override" << std::endl
            << "      public void deallocate() {" << std::endl
            << "        delete(this);" << std::endl
            << "      }" << std::endl
            << std::endl
            << "      @Namespace(\"::mlpack::util\")" << std::endl
            << "      @Name(\"Delete<" << d.cppType << ">\")" << std::endl
            << "      private static native void delete(Pointer p);"
            << std::endl
            << "    }" << std::endl
            << std::endl
            << "    private " << ptr << "(Pointer p) {" << std::endl
            << "      super(p);" << std::endl
            << "    }" << std::endl
            << std::endl
            << "    static Pointer create(Pointer p) {" << std::endl
            << "      " << ptr << " result = new " << ptr << "(p);" << std::endl
            << "      result.deallocator(new MethodDeallocator(result));"
            << std::endl
            << "      return result;" << std::endl
            << "    }" << std::endl
            << "  }" << std::endl
            << std::endl
            << "  @Namespace(\"::mlpack::util\")" << std::endl
            << "  @Name(\"GetParam<" << d.cppType << "*>\")" << std::endl
            << "  private static native Pointer get" << ptr << "(String name);"
            << std::endl << std::endl
            << "  @Namespace(\"::mlpack::util\")" << std::endl
            << "  @Name(\"SetParam<" << d.cppType << "*>\")" << std::endl
            << "  private static native void set" << ptr << "(String name, "
            << "@Cast(\"" << d.cppType << "*\") Pointer model);" << std::endl
            << std::endl;
}

/**
 * If the type is serializable, print the definition of a special utility
 * function to set a IO parameter of that type to stdout.
 */
template<typename T>
void PrintParamDefn(util::ParamData& d,
                    const void* /* input */,
                    void* /* output */)
{
  PrintParamDefn<typename std::remove_pointer<T>::type>(d);
}

} // namespace java
} // namespace bindings
} // namespace mlpack

#endif
