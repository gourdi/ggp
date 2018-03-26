if (MSVC)
  add_definitions(/std:c++latest -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
endif()

macro(get_sources_from_root_directory root sources)
  file(GLOB_RECURSE all_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${root}/*.h" "${root}/*.cpp")
  set(${sources} ${all_files})
endmacro()

macro(group_sources)
  foreach(f ${ARGN})
    get_filename_component(dir ${f} DIRECTORY)
    string(REPLACE "/" "\\\\" dir ${dir})
 	  source_group(${dir} FILES ${f})
  endforeach()
endmacro()

macro(get_subdirectories dir subdirectories)
  set(all_dirs ${all_dirs} "${CMAKE_CURRENT_SOURCE_DIR}/${dir}") # Add the root directory to the output list too.
  file(GLOB_RECURSE all_files LIST_DIRECTORIES true "${dir}/*")
  foreach(f ${all_files})
    if (IS_DIRECTORY ${f})
      set(all_dirs ${all_dirs} ${f})
    endif()
  endforeach()
  set(${subdirectories} ${all_dirs})
endmacro()