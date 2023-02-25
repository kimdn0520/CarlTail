# Generated by Boost 1.80.0

# address-model=32

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  _BOOST_SKIPPED("boost_context-vc142-mt-gd-x32-1_80.lib" "32 bit, need 64")
  return()
endif()

# layout=versioned

# toolset=vc142

if(Boost_COMPILER)
  if(NOT Boost_COMPILER STREQUAL "vc142" AND NOT Boost_COMPILER STREQUAL "-vc142")
    _BOOST_SKIPPED("boost_context-vc142-mt-gd-x32-1_80.lib" "vc142, Boost_COMPILER=${Boost_COMPILER}")
    return()
  endif()
else()
  if(BOOST_DETECTED_TOOLSET AND NOT BOOST_DETECTED_TOOLSET STREQUAL "vc142")
    _BOOST_SKIPPED("boost_context-vc142-mt-gd-x32-1_80.lib" "vc142, detected ${BOOST_DETECTED_TOOLSET}, set Boost_COMPILER to override")
    return()
  endif()
endif()

# link=shared

if(DEFINED Boost_USE_STATIC_LIBS)
  if(Boost_USE_STATIC_LIBS)
    _BOOST_SKIPPED("boost_context-vc142-mt-gd-x32-1_80.lib" "shared, Boost_USE_STATIC_LIBS=${Boost_USE_STATIC_LIBS}")
    return()
  endif()
else()
  if(WIN32 AND NOT _BOOST_SINGLE_VARIANT)
    _BOOST_SKIPPED("boost_context-vc142-mt-gd-x32-1_80.lib" "shared, default on Windows is static, set Boost_USE_STATIC_LIBS=OFF to override")
    return()
  endif()
endif()

# runtime-link=shared

if(Boost_USE_STATIC_RUNTIME)
  _BOOST_SKIPPED("boost_context-vc142-mt-gd-x32-1_80.lib" "shared runtime, Boost_USE_STATIC_RUNTIME=${Boost_USE_STATIC_RUNTIME}")
  return()
endif()

# runtime-debugging=on

if(NOT "${Boost_USE_DEBUG_RUNTIME}" STREQUAL "" AND NOT Boost_USE_DEBUG_RUNTIME)
  _BOOST_SKIPPED("boost_context-vc142-mt-gd-x32-1_80.lib" "debug runtime, Boost_USE_DEBUG_RUNTIME=${Boost_USE_DEBUG_RUNTIME}")
  return()
endif()

# threading=multi

if(DEFINED Boost_USE_MULTITHREADED AND NOT Boost_USE_MULTITHREADED)
  _BOOST_SKIPPED("boost_context-vc142-mt-gd-x32-1_80.lib" "multithreaded, Boost_USE_MULTITHREADED=${Boost_USE_MULTITHREADED}")
  return()
endif()

# variant=debug

if(NOT "${Boost_USE_DEBUG_LIBS}" STREQUAL "" AND NOT Boost_USE_DEBUG_LIBS)
  _BOOST_SKIPPED("boost_context-vc142-mt-gd-x32-1_80.lib" "debug, Boost_USE_DEBUG_LIBS=${Boost_USE_DEBUG_LIBS}")
  return()
endif()

if(Boost_VERBOSE OR Boost_DEBUG)
  message(STATUS "  [x] boost_context-vc142-mt-gd-x32-1_80.lib")
endif()

# Create imported target Boost::context

if(NOT TARGET Boost::context)
  add_library(Boost::context SHARED IMPORTED)

  set_target_properties(Boost::context PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_BOOST_INCLUDEDIR}"
    INTERFACE_COMPILE_DEFINITIONS "BOOST_CONTEXT_NO_LIB"
  )
endif()

# Target file name: boost_context-vc142-mt-gd-x32-1_80.lib

get_target_property(__boost_imploc Boost::context IMPORTED_IMPLIB_DEBUG)
if(__boost_imploc)
  message(SEND_ERROR "Target Boost::context already has an imported location '${__boost_imploc}', which is being overwritten with '${_BOOST_LIBDIR}/boost_context-vc142-mt-gd-x32-1_80.lib'")
endif()
unset(__boost_imploc)

set_property(TARGET Boost::context APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)

set_target_properties(Boost::context PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG CXX
  IMPORTED_IMPLIB_DEBUG "${_BOOST_LIBDIR}/boost_context-vc142-mt-gd-x32-1_80.lib"
  )

set_target_properties(Boost::context PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_BOOST_LIBDIR}/boost_context-vc142-mt-gd-x32-1_80.dll"
  )

set_property(TARGET Boost::context APPEND
  PROPERTY INTERFACE_COMPILE_DEFINITIONS "BOOST_CONTEXT_DYN_LINK"
  )

list(APPEND _BOOST_CONTEXT_DEPS headers)
