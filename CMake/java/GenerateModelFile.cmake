# GenerateModelFile.cmake: 

# This file depends on the following variables being set:
#
#  * PROGRAM_MAIN_FILE: the file containing the mlpackMain() function.
#
include("${SOURCE_DIR}/CMake/StripType.cmake")
strip_type("${PROGRAM_MAIN_FILE}")
if (NOT EXISTS "${MAVEN_BUILD_DIR}/src/main/java/org/mlpack/${MODEL_SAFE_TYPES}Type.java")
  file(WRITE
      "${MAVEN_BUILD_DIR}/src/main/java/org/mlpack/${MODEL_SAFE_TYPES}Type.java"
      "package org.mlpack;\n"
      "\n"
      "import org.bytedeco.javacpp.*;\n"
      "import org.bytedeco.javacpp.annotation.*;\n"
      "\n"
      "@Platform(not = \"\")\n"
      "public class ${MODEL_SAFE_TYPES}Type {\n"
      "  private final Pointer pointer;\n"
      "\n"
      "  ${MODEL_SAFE_TYPES}Type(Pointer pointer) {\n"
      "    this.pointer = pointer;\n"
        }
      "\n"
      "  Pointer getPointer() {\n"
      "    return pointer;\n"
      "  }\n"
      "}")
endif()
