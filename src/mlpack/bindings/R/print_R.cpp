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
#include <mlpack/bindings/utils/strip_type.hpp>
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

  // Print the documentation.
  cout << "#' @title ";
  cout << util::HyphenateString(programInfo.programName, "#'   ") << endl;
  cout << "#'" << endl;

  // Next print the description.
  cout << "#' @description" << endl;
  cout << "#' ";
  cout << util::HyphenateString(programInfo.documentation(), "#' ") << endl;

  // Next, print information on the input options.
  cout << "#'" << endl;

  for (size_t i = 0; i < inputOptions.size(); ++i)
  {
    const string& opt = inputOptions[i];
    const util::ParamData& d = parameters.at(opt);

    cout << "#' @param ";
    bool out = false;
    CLI::GetSingleton().functionMap[d.tname]["PrintDoc"](d, NULL, (void*) &out);

    cout << endl;
  }

  cout << "#'" << endl;
  cout << "#' @return A list with several components:" << endl;

  for (size_t i = 0; i < outputOptions.size(); ++i)
  {
    const string& opt = outputOptions[i];
    const util::ParamData& d = parameters.at(opt);

    cout << "#' \\item{";

    bool out = true;
    CLI::GetSingleton().functionMap[d.tname]["PrintDoc"](d, NULL, (void*) &out);

    cout << endl;
  }

  cout << "#'" << endl;

  cout << "#' @export" << endl;
  cout << functionName << " <- function(";
  size_t indent = functionName.size() + 13 /* <- function(*/;
  for (size_t i = 0; i < inputOptions.size(); ++i)
  {
    const util::ParamData& d = parameters.at(inputOptions[i]);

    if (i != 0)
      cout << "," << endl << std::string(indent, ' ');

    CLI::GetSingleton().functionMap[d.tname]["PrintInputParam"](d, NULL, NULL);
  }
  cout << ") {" << endl;

  // Restore CLI settings.
  cout << "  # Restore CLI settings." << endl;
  cout << "  CLI_RestoreSettings(\"" << CLI::ProgramName()
      << "\")" << endl;
  cout << endl;

  // Handle each input argument's processing before calling mlpackMain().
  cout << "  # Process each input argument before calling mlpackMain()."
      << endl;
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
  cout << "  if (verbose) {" << endl;
  cout << "    CLI_EnableVerbose()" << endl;
  cout << "  } else {" << endl;
  cout << "    CLI_DisableVerbose()" << endl;
  cout << "  }" << endl;
  cout << endl;

  // Mark output parameters as passed.
  cout << "  # Mark all output options as passed." << endl;
  for (const string& opt : outputOptions)
  {
    const util::ParamData& d = parameters.at(opt);
    cout << "  CLI_SetPassed(\"" << d.name << "\")" << endl;
  }
  cout << endl;

  // Call the program.
  cout << "  # Call the program." << endl;
  cout << "  " << functionName << "_mlpackMain()" << endl << endl;

  // Add ModelType as attr to the model pointer.
  cout << "  # Add ModelType as attribute to the model pointer, if needed."
      << endl;
  for (size_t i = 0; i < outputOptions.size(); ++i)
  {
    const util::ParamData& d = parameters.at(outputOptions[i]);
    CLI::GetSingleton().functionMap[d.tname]["PrintSerializeUtil"](d,
        NULL, NULL);
  }
  cout << endl;

  // Extract the results in order.
  cout << "  # Extract the results in order." << endl;
  cout << "  out <- list(" << endl;
  string indentStr(4, ' ');
  for (size_t i = 0; i < outputOptions.size(); ++i)
  {
    if (i == 0)
       cout << indentStr;
    const util::ParamData& d = parameters.at(outputOptions[i]);
    CLI::GetSingleton().functionMap[d.tname]["PrintOutputProcessing"](d,
        NULL, NULL);
    // Print newlines if we are returning multiple output options.
    if (i + 1 < outputOptions.size())
      cout << "," << endl << indentStr;
  }
  cout << endl << "  )" << endl << endl;

  // Clear the parameters.
  cout << "  # Clear the parameters." << endl;
  cout << "  CLI_ClearSettings()" << endl;
  cout << endl;
  cout << "  return(out)" << endl << "}" << endl;
}

} // namespace r
} // namespace bindings
} // namespace mlpack
