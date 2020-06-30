/**
 * @file bindings/R/print_R.cpp
 * @author Yashwant Singh Parihar
 *
 * Implementation of utility PrintR() function.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#include "print_R.hpp"
#include "strip_type.hpp"
#include <mlpack/prereqs.hpp>
#include <mlpack/core/util/hyphenate_string.hpp>

using namespace mlpack;
using namespace std;

namespace mlpack {
namespace bindings {
namespace r {


/**
 * Print the code for a .R binding for an mlpack program to stdout.
 */
void PrintR(const util::ProgramDoc& programInfo,
             const string& functionName)
{
  // Restore parameters.
  CLI::RestoreSettings(programInfo.programName);

  const map<string, util::ParamData>& parameters = CLI::Parameters();
  typedef map<string, util::ParamData>::const_iterator ParamIter;

  // First, let's get a list of input and output options.  We'll take two passes
  // so that the required input options are the first in the list.
  vector<string> inputOptions, outputOptions;
  for (ParamIter it = parameters.begin(); it != parameters.end(); ++it)
  {
    const util::ParamData& d = it->second;
    if (d.input && d.required)
    {
      // Ignore some parameters.
      if (d.name != "help" && d.name != "info" &&
          d.name != "version")
        inputOptions.push_back(it->first);
    }
    else if (!d.input)
    {
      outputOptions.push_back(it->first);
    }
  }

  for (ParamIter it = parameters.begin(); it != parameters.end(); ++it)
  {
    const util::ParamData& d = it->second;
    if (d.input && !d.required &&
        d.name != "help" && d.name != "info" &&
        d.name != "version")
      inputOptions.push_back(it->first);
  }

  MLPACK_COUT_STREAM << "#' @export" << endl;
  MLPACK_COUT_STREAM << functionName << " <- function(";
  size_t indent = functionName.size() + 13 /* <- function(*/;
  for (size_t i = 0; i < inputOptions.size(); ++i)
  {
    const util::ParamData& d = parameters.at(inputOptions[i]);

    if (i != 0)
      MLPACK_COUT_STREAM << "," << endl << std::string(indent, ' ');

    CLI::GetSingleton().functionMap[d.tname]["PrintInputParam"](d, NULL, NULL);
  }
  MLPACK_COUT_STREAM << ") {" << endl;
  MLPACK_COUT_STREAM << endl;

  // Restore CLI settings.
  MLPACK_COUT_STREAM << "  CLI_RestoreSettings(\"" << CLI::ProgramName()
      << "\")" << endl;
  MLPACK_COUT_STREAM << endl;

  // Handle each input argument's processing before calling mlpackMain().
  for (const string& opt : inputOptions)
  {
    if (opt != "verbose")
    {
      const util::ParamData& d = parameters.at(opt);
      CLI::GetSingleton().functionMap[d.tname]["PrintInputProcessing"](d,
          NULL, NULL);
    }
  }

  // Special handling for verbose output.
  MLPACK_COUT_STREAM << "  if (verbose) {" << endl;
  MLPACK_COUT_STREAM << "    CLI_EnableVerbose()" << endl;
  MLPACK_COUT_STREAM << "  } else {" << endl;
  MLPACK_COUT_STREAM << "    CLI_DisableVerbose()" << endl;
  MLPACK_COUT_STREAM << "  }" << endl;
  MLPACK_COUT_STREAM << endl;

  // Mark output parameters as passed.
  for (const string& opt : outputOptions)
  {
    const util::ParamData& d = parameters.at(opt);
    MLPACK_COUT_STREAM << "  CLI_SetPassed(\"" << d.name << "\")" << endl;
  }
  MLPACK_COUT_STREAM << endl;

  // Call the program.
  MLPACK_COUT_STREAM << "  " << functionName << "_mlpackMain()" << endl;

  for (size_t i = 0; i < outputOptions.size(); ++i)
  {
    const util::ParamData& d = parameters.at(outputOptions[i]);
    CLI::GetSingleton().functionMap[d.tname]["PrintSerializeUtil"](d,
        NULL, NULL);
  }
  MLPACK_COUT_STREAM << endl;

  // Extract the results in order.
  MLPACK_COUT_STREAM << "  out <- list(" << endl;
  string indentStr(4, ' ');
  for (size_t i = 0; i < outputOptions.size(); ++i)
  {
    if (i == 0)
       MLPACK_COUT_STREAM << indentStr;
    const util::ParamData& d = parameters.at(outputOptions[i]);
    CLI::GetSingleton().functionMap[d.tname]["PrintOutputProcessing"](d,
        NULL, NULL);
    // Print newlines if we are returning multiple output options.
    if (i + 1 < outputOptions.size())
      MLPACK_COUT_STREAM << "," << endl << indentStr;
  }
  MLPACK_COUT_STREAM << endl << "  )" << endl << endl;

  // Restore CLI settings.
  MLPACK_COUT_STREAM << "  CLI_ClearSettings()" << endl;
  MLPACK_COUT_STREAM << endl;
  MLPACK_COUT_STREAM << "  return(out)" << endl << "}" << endl;
}

} // namespace r
} // namespace bindings
} // namespace mlpack
