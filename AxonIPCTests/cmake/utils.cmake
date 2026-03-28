
macro(config_compiler_and_linker)
  if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC|Clang")
    # For MSVC and Clang, CMake sets certain flags to defaults we want to
    # override.
    # This replacement code is taken from sample in the CMake Wiki at
    # https://gitlab.kitware.com/cmake/community/wikis/FAQ#dynamic-replace.
    foreach (flag_var
             CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
             CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      if (NOT BUILD_SHARED_LIBS)
        string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")

        # When using Ninja with Clang, static builds pass -D_DLL on Windows.
        # This is incorrect and should not happen, so we fix that here.
        string(REPLACE "-D_DLL" "" ${flag_var} "${${flag_var}}")
      endif()
    endforeach()
  endif()
endmacro()
