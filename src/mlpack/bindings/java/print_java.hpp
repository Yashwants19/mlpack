/**
 * @file bindings/java/print_java.hpp
 * @author Vasyl Teliman
 *
 * Definition for PrintJava function.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_BINDINGS_JAVA_PRINT_JAVA_HPP
#define MLPACK_BINDINGS_JAVA_PRINT_JAVA_HPP

namespace mlpack {
namespace bindings {
namespace java {

/**
 * Generate java binding for a library method
 */
void PrintJava(const util::BindingDetails& doc,
               const std::string& methodName,
               const std::string& methodPath);

} // namespace java
} // namespace bindings
} // namespace mlpack

#endif
