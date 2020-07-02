# FindAutotools.cmake
#
# Based on Lars's implementation:
# https://github.com/lbaehren/CMakeModules/blob/master/FindAutotools.cmake
if (AUTOTOOLS_FOUND)
  return()
endif()

# Set root directory for Autotools.
if (NOT AUTOTOOLS_ROOT_DIR)
  set (AUTOTOOLS_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
endif ()

# Find the Autotools.
find_program (AUTORECONF_EXECUTABLE autoreconf
  HINTS ${AUTOTOOLS_ROOT_DIR} ${CMAKE_INSTALL_PREFIX}
  PATH_SUFFIXES bin
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args (Autotools DEFAULT_MSG AUTORECONF_EXECUTABLE)

# Mark advanced variables
mark_as_advanced (
  AUTOTOOLS_ROOT_DIR
  AUTORECONF_EXECUTABLE
  AUTOSCAN_EXECUTABLE
)
