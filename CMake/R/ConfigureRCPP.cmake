# ConfigureRCPP.cmake: generate an mlpack .cpp file for a R binding given
# input arguments.
#
# This file depends on the following variables being set:
#
#  * PROGRAM_NAME: name of the binding
#  * PROGRAM_MAIN_FILE: the file containing the mlpackMain() function.
#  * R_CPP_IN: path of the r_method.cpp.in file.
#  * R_CPP_OUT: name of the output .cpp file.
file(READ "${PROGRAM_MAIN_FILE}" MAIN_FILE)

# Grab all "PARAM_MODEL_IN(Model,", "PARAM_MODEL_IN_REQ(Model,",
# "PARAM_MODEL_OUT(Model,".
string(REGEX MATCHALL "PARAM_MODEL_IN\\([A-Za-z_<>]*," MODELS_IN
    "${MAIN_FILE}")
string(REGEX MATCHALL "PARAM_MODEL_IN_REQ\\([A-Za-z_<>]*," MODELS_IN_REQ
    "${MAIN_FILE}")
string(REGEX MATCHALL "PARAM_MODEL_OUT\\([A-Za-z_]*," MODELS_OUT "${MAIN_FILE}")

string(REGEX REPLACE "PARAM_MODEL_IN\\(" "" MODELS_IN_STRIP1 "${MODELS_IN}")
string(REGEX REPLACE "," "" MODELS_IN_STRIP2 "${MODELS_IN_STRIP1}")
string(REGEX REPLACE "[<>,]" "" MODELS_IN_SAFE_STRIP2 "${MODELS_IN_STRIP1}")

string(REGEX REPLACE "PARAM_MODEL_IN_REQ\\(" "" MODELS_IN_REQ_STRIP1
    "${MODELS_IN_REQ}")
string(REGEX REPLACE "," "" MODELS_IN_REQ_STRIP2 "${MODELS_IN_REQ_STRIP1}")
string(REGEX REPLACE "[<>,]" "" MODELS_IN_REQ_SAFE_STRIP2
    "${MODELS_IN_REQ_STRIP1}")

string(REGEX REPLACE "PARAM_MODEL_OUT\\(" "" MODELS_OUT_STRIP1 "${MODELS_OUT}")
string(REGEX REPLACE "," "" MODELS_OUT_STRIP2 "${MODELS_OUT_STRIP1}")
string(REGEX REPLACE "[<>,]" "" MODELS_OUT_SAFE_STRIP2 "${MODELS_OUT_STRIP1}")

set(MODEL_TYPES ${MODELS_IN_STRIP2} ${MODELS_IN_REQ_STRIP2}
    ${MODELS_OUT_STRIP2})
set(MODEL_SAFE_TYPES ${MODELS_IN_SAFE_STRIP2} ${MODELS_IN_REQ_SAFE_STRIP2}
    ${MODELS_OUT_SAFE_STRIP2})
if (MODEL_TYPES)
  list(REMOVE_DUPLICATES MODEL_TYPES)
endif ()
if (MODEL_SAFE_TYPES)
  list(REMOVE_DUPLICATES MODEL_SAFE_TYPES)
endif ()

file(READ "${MODEL_FILE}" MODEL_FILE_TYPE)
if (NOT (MODEL_FILE_TYPE MATCHES "\"${MODEL_SAFE_TYPES}\""))
  file(APPEND "${MODEL_FILE}" "\"${MODEL_SAFE_TYPES}\"\n")
  # Now, generate the implementation of the functions we need.
  set(MODEL_PTR_IMPLS "")
  list(LENGTH MODEL_TYPES NUM_MODEL_TYPES)
  # Append content to the list.
  if (${NUM_MODEL_TYPES} GREATER 0)
    math(EXPR LOOP_MAX "${NUM_MODEL_TYPES}-1")
    foreach (INDEX RANGE ${LOOP_MAX})
      list(GET MODEL_TYPES ${INDEX} MODEL_TYPE)
      list(GET MODEL_SAFE_TYPES ${INDEX} MODEL_SAFE_TYPE)

      # Define typedef for the model.
      set(MODEL_PTR_TYPEDEF "${MODEL_PTR_TYPEDEF}Rcpp::XPtr<${MODEL_TYPE}>")

      # Generate the implementation.
      set(MODEL_PTR_IMPLS "${MODEL_PTR_IMPLS}
// Get the pointer to a ${MODEL_TYPE} parameter.
// [[Rcpp::export]]
SEXP CLI_GetParam${MODEL_SAFE_TYPE}Ptr(const std::string& paramName)
{
  return std::move((${MODEL_PTR_TYPEDEF}) CLI::GetParam<${MODEL_TYPE}*>(paramName));
}

// Set the pointer to a ${MODEL_TYPE} parameter.
// [[Rcpp::export]]
void CLI_SetParam${MODEL_SAFE_TYPE}Ptr(const std::string& paramName, SEXP ptr)
{
  CLI::GetParam<${MODEL_TYPE}*>(paramName) =  Rcpp::as<${MODEL_PTR_TYPEDEF}>(ptr);
  CLI::SetPassed(paramName);
}

// Serialize a ${MODEL_TYPE} pointer.
// [[Rcpp::export]]
Rcpp::RawVector Serialize${MODEL_SAFE_TYPE}Ptr(SEXP ptr)
{
  std::ostringstream oss;
  {
    boost::archive::binary_oarchive oa(oss);
    oa << boost::serialization::make_nvp(\"${MODEL_SAFE_TYPE}\",
          *Rcpp::as<${MODEL_PTR_TYPEDEF}>(ptr));
  }

  Rcpp::RawVector raw_vec(oss.str().size());

  // Copy the string buffer so we can return one that won't get deallocated when
  // we exit this function.
  memcpy(&raw_vec[0], oss.str().c_str(), oss.str().size());
  raw_vec.attr(\"type\") = \"${MODEL_SAFE_TYPE}\";
  return raw_vec;
}

// Deserialize a ${MODEL_TYPE} pointer.
// [[Rcpp::export]]
SEXP Deserialize${MODEL_SAFE_TYPE}Ptr(Rcpp::RawVector str)
{
  ${MODEL_TYPE}* ptr = new ${MODEL_TYPE}();

  std::istringstream iss(std::string((char *) &str[0], str.size()));
  {
    boost::archive::binary_iarchive ia(iss);
    ia >> boost::serialization::make_nvp(\"${MODEL_SAFE_TYPE}\", *ptr);
  }

  // R will be responsible for freeing this.
  return std::move((${MODEL_PTR_TYPEDEF})ptr);
}
")
    endforeach ()
  endif()
endif()

# Now configure the files.
configure_file("${R_CPP_IN}" "${R_CPP_OUT}")
