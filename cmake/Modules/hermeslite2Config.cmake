INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_HERMESLITE2 hermeslite2)

FIND_PATH(
    HERMESLITE2_INCLUDE_DIRS
    NAMES hermeslite2/api.h
    HINTS $ENV{HERMESLITE2_DIR}/include
        ${PC_HERMESLITE2_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    HERMESLITE2_LIBRARIES
    NAMES gnuradio-hermeslite2
    HINTS $ENV{HERMESLITE2_DIR}/lib
        ${PC_HERMESLITE2_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/hermeslite2Target.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HERMESLITE2 DEFAULT_MSG HERMESLITE2_LIBRARIES HERMESLITE2_INCLUDE_DIRS)
MARK_AS_ADVANCED(HERMESLITE2_LIBRARIES HERMESLITE2_INCLUDE_DIRS)
