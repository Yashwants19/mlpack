# CamelCase.cmake: Given an snake_case like, e.g., "logistic_regression", return
# CamelCase(e.g. "LogisticRegression") that can be used in bindings.

# This function depends on the following variables being set:
#
#  * PROGRAM_NAME: Program Name.
#
function(camel_case PROGRAM_NAME)
  string(LENGTH ${PROGRAM_NAME} NUM_MODEL_CHAR)
  if (${NUM_MODEL_CHAR} GREATER 0)
    math(EXPR LAST_CHAR_INDEX "${NUM_MODEL_CHAR}-2")
    string(SUBSTRING ${PROGRAM_NAME} "0" "1" MODEL_CHAR)
    string(TOUPPER ${MODEL_CHAR} MODEL_CHAR)
    string(APPEND CAMEL_CASE_PROGRAM_NAME ${MODEL_CHAR})
    foreach (INDEX0 RANGE ${LAST_CHAR_INDEX})
      math(EXPR INDEX0 "${INDEX0}+1") 
      math(EXPR INDEX1 "${INDEX0}+1")
      math(EXPR INDEX2 "${INDEX0}-1")
      string(SUBSTRING "${PROGRAM_NAME}" "${INDEX0}" "1" MODEL_CHAR1)
      string(SUBSTRING "${PROGRAM_NAME}" "${INDEX1}" "1" MODEL_CHAR2)
      string(SUBSTRING "${PROGRAM_NAME}" "${INDEX2}" "1" MODEL_CHAR3)
      if ("${MODEL_CHAR1}" MATCHES "_")
         string(TOUPPER ${MODEL_CHAR2} MODEL_CHAR2)
         string(APPEND CAMEL_CASE_PROGRAM_NAME ${MODEL_CHAR2})
         set(INDEX0 ${INDEX1})
      elseif ("${MODEL_CHAR3}" MATCHES "_")
         continue()
      else()
         string(APPEND CAMEL_CASE_PROGRAM_NAME ${MODEL_CHAR1})
      endif()
     endforeach()
  endif()
  set(CAMEL_CASE_PROGRAM_NAME ${CAMEL_CASE_PROGRAM_NAME} PARENT_SCOPE)
endfunction()
