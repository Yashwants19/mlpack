/**
 * @file print_java.cpp
 * @author Vasyl Teliman
 *
 * Implementation of utility PrintJava() function.
 */
#include <iostream>
#include <fstream>
#include <mlpack/core/util/hyphenate_string.hpp>
#include <mlpack/core.hpp>
#include <vector>
#include "print_java.hpp"
#include "get_java_type.hpp"
#include "util.hpp"

namespace mlpack {
namespace bindings {
namespace java {

using namespace std;
using namespace mlpack;

/**
 * Print the code for a .java binding for an mlpack program to a specific file.
 */
void PrintJava(const util::ProgramDoc& programInfo,
    const std::string& methodName, const std::string& methodPath)
{
  string methodFile = methodName + "_main.cpp";
  string className = ToCamelCase(methodName);
  ofstream fout(className + ".java");
  RedirectStream raii(cout, fout);

  CLI::RestoreSettings(programInfo.programName);

  vector<util::ParamData> input, output;

  for (const auto& param : CLI::Parameters())
  {
    const util::ParamData& paramData = param.second;

    if (paramData.input) input.push_back(paramData);
    else output.push_back(paramData);
  }

  cout << "package org.mlpack;" << endl
       << endl
       << "import org.nd4j.linalg.api.ndarray.INDArray;" << endl
       << "import org.bytedeco.javacpp.*;" << endl
       << "import org.bytedeco.javacpp.annotation.*;" << endl
       << "import java.util.*;" << endl
       << endl
       << "/**" << endl
       << " * " << util::HyphenateString(programInfo.shortDocumentation, " * ") << endl
       << " * " << endl
       << " * " << util::HyphenateString(programInfo.documentation(), " * ") << endl
       << " * " << endl
       << " * Program expects the following arguments:" << endl
       << " * <p>" << endl
       << " * <ol>" << endl;

  for (const util::ParamData& param : input)
  {
    if (param.required)
    {
      string desc = util::HyphenateString(param.desc, " *         ");
      string type;
      CLI::GetSingleton().functionMap[param.tname]["GetJavaType"](param, nullptr, (void*) &type);
      cout << " *   <li>" << type << ' ' << param.name << ": " << desc << "</li>" << endl;
    }
  }

  for (const util::ParamData& param : input)
  {
    if (!param.required)
    {
      string desc = util::HyphenateString(param.desc, " *         ");
      string type;
      CLI::GetSingleton().functionMap[param.tname]["GetJavaType"](param, nullptr, (void*) &type);
      cout << " *   <li>" << type << ' ' << param.name << " [optional]: " << desc << "</li>" << endl;
    }
  }

  cout << " * </ol>" << endl
       << " * <p>" << endl
       << " * Output parameters:" << endl
       << " * <ol>" << endl;

  for (const util::ParamData& param : output)
  {
    string desc = util::HyphenateString(param.desc, " *         ");
    string type;
    CLI::GetSingleton().functionMap[param.tname]["GetJavaType"](param, nullptr, (void*) &type);
    cout << " *   <li>" << type << ' ' << param.name << ": " << desc << "</li>" << endl;
  }

  cout << " * </ol>" << endl
       << " */" << endl
       << "@Platform(" << endl
       << "    include = {" << endl
       << "        \"" << methodName << "_main.cpp\"," << endl
       << "        \"cli_util.hpp\"," << endl
       << "        \"deleter.hpp\"" << endl
       << "    }," << endl
       << "    link = \"mlpack\"," << endl
       << "    includepath = \"" << methodPath << "\")" << endl
       << "public class " << className << " {" << endl
       << "  private static final String THIS_NAME = \"" << programInfo.programName << "\";" << endl
       << endl
       << "  public static final class Params {" << endl
       << "    private final Map<String, Object> params = new HashMap<>();" << endl
       << endl
       << "    public Params() {" << endl;

  for (const util::ParamData& param : input)
  {
    cout << "      params.put(\"" << param.name << "\", null);" << endl;
  }

  for (const util::ParamData& param : output)
  {
    cout << "      params.put(\"" << param.name << "\", null);" << endl;
  }

  cout << "    }" << endl
       << endl
       << "    private void checkHasParameter(String name) {" << endl
       << "      if (!params.containsKey(name)) {" << endl
       << "        throw new IllegalArgumentException(" << endl
       << "            THIS_NAME + \" doesn't have \" + name + \" parameter\");" << endl
       << "      }" << endl
       << "    }" << endl
       << endl
       << "    public Params put(String name, Object value) {" << endl
       << "      checkHasParameter(name);" << endl
       << "      params.put(name, value);" << endl
       << "      return this;" << endl
       << "    }" << endl
       << endl
       << "    public <T> T get(String name, Class<T> clazz) {" << endl
       << "      checkHasParameter(name);" << endl
       << endl
       << "      try {" << endl
       << "        return clazz.cast(params.get(name));" << endl
       << "      } catch (ClassCastException e) {" << endl
       << "        throw new IllegalArgumentException(" << endl
       << "            \"Parameter \" + name + \" is not an instance of \" + clazz.getName(), e);" << endl
       << "      }" << endl
       << "    }" << endl
       << "  }" << endl
       << endl;

  PrintModelPointers(input, output);

  cout << "  static {" << endl
       << "    Loader.load();" << endl
       << "  }" << endl
       << endl
       << "  private " << className << "() {" << endl
       << "  }" << endl
       << endl
       << "  private static native void mlpackMain();" << endl
       << endl
       << "  private static void checkHasRequiredParameter(Params params, String name) {" << endl
       << "    if (params.get(name, Object.class) == null) {" << endl
       << "      throw new IllegalArgumentException(\"Missing required parameter \" + name);" << endl
       << "    }" << endl
       << "  }" << endl
       << endl
       << "  public static void run(Params params) {" << endl
       << "    CLI.restoreSettings(THIS_NAME);" << endl
       << endl;

  for (const util::ParamData& param : input)
  {
    CLI::GetSingleton().functionMap[param.tname]["PrintInputParam"](param, nullptr, nullptr);
  }

  for (const util::ParamData& param : output)
  {
    cout << "    CLI.setPassed(\"" << param.name << "\");" << endl;
  }

  cout << endl
       << "    mlpackMain();" << endl
       << endl;

  for (const util::ParamData& param : output)
  {
    CLI::GetSingleton().functionMap[param.tname]["PrintOutputParam"](param, nullptr, nullptr);
  }

  cout << "  }" << endl
       << "}" << endl;
}

}
}
}
