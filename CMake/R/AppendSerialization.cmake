# AppendSerialization.cmake: append imports for serialization and
# deserialization for mlpack model types to the existing list of serialization
# and deserialization imports.

# This function depends on the following variables being set:
#
#  * PROGRAM_MAIN_FILE: the file containing the mlpackMain() function.
#  * SERIALIZATION_FILE: file to append types to
#
# We need to parse the main file and find any PARAM_MODEL_* lines.
function(append_serialization SERIALIZATION_FILE PROGRAM_MAIN_FILE)
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

  list(LENGTH MODEL_TYPES NUM_MODEL_TYPES)
  if (${NUM_MODEL_TYPES} GREATER 0)
    math(EXPR LOOP_MAX "${NUM_MODEL_TYPES}-1")
    foreach (INDEX RANGE ${LOOP_MAX})
      list(GET MODEL_TYPES ${INDEX} MODEL_TYPE)
      list(GET MODEL_SAFE_TYPES ${INDEX} MODEL_SAFE_TYPE)

      # See if the model type already exists.
      file(READ "${SERIALIZATION_FILE}" SERIALIZATION_FILE_CONTENTS)
      string(FIND
          "${SERIALIZATION_FILE_CONTENTS}"
          "\"${MODEL_SAFE_TYPE}\" = Serialize${MODEL_SAFE_TYPE}Ptr,"
          FIND_OUT)

      # If it doesn't exist, append it.
      if (${FIND_OUT} EQUAL -1)
        # Now append the type to the list of types, and define any serialization
        # function.
        file(APPEND
            "${SERIALIZATION_FILE}"
            "      \"${MODEL_SAFE_TYPE}\" = Serialize${MODEL_SAFE_TYPE}Ptr,\n")
    endif()
    endforeach ()
  endif()
endfunction()

function(append_unserialization SERIALIZATION_FILE PROGRAM_MAIN_FILE)
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

  list(LENGTH MODEL_TYPES NUM_MODEL_TYPES)
  if (${NUM_MODEL_TYPES} GREATER 0)
    math(EXPR LOOP_MAX "${NUM_MODEL_TYPES}-1")
    foreach (INDEX RANGE ${LOOP_MAX})
      list(GET MODEL_TYPES ${INDEX} MODEL_TYPE)
      list(GET MODEL_SAFE_TYPES ${INDEX} MODEL_SAFE_TYPE)
      # See if the model type already exists.
      file(READ "${SERIALIZATION_FILE}" SERIALIZATION_FILE_CONTENTS)
      string(FIND
          "${SERIALIZATION_FILE_CONTENTS}"
          "\"${MODEL_SAFE_TYPE}\" = Deserialize${MODEL_SAFE_TYPE}Ptr,"
          FIND_OUT)

      # If it doesn't exist, append it.
      if (${FIND_OUT} EQUAL -1)
        # Now append the type to the list of types, and define any deserialization
        # function.
        file(APPEND
            "${SERIALIZATION_FILE}"
            "      \"${MODEL_SAFE_TYPE}\" = Deserialize${MODEL_SAFE_TYPE}Ptr,\n")
    endif()
    endforeach ()
  endif()
endfunction()
