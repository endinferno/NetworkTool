cmake_minimum_required(VERSION 3.5)

macro(FIND_DEPENDENCIES)
	file(GLOB FILE_NAMES *)
	foreach(LIB_NAMES ${FILE_NAMES})
		if (IS_DIRECTORY ${LIB_NAMES})
			add_subdirectory(${LIB_NAMES})
		endif()
	endforeach()
endmacro()

FIND_DEPENDENCIES()
