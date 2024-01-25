cmake_minimum_required(VERSION 3.14)
project(AdventOfCode)

include(Packages.cmake)
add_dependency(
  NAME nlohmann-json # TODO package_name for cmake is different than vcpkg
  CMAKE_PKG nlohmann_json
  URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.zip
)

add_dependency(
  NAME fmt
  URL https://github.com/fmtlib/fmt/releases/download/10.2.1/fmt-10.2.1.zip
)

add_dependency(
  NAME range-v3
  URL https://github.com/ericniebler/range-v3/archive/refs/tags/0.12.0.zip
)

add_dependency(
  NAME _project_options 
  URL https://github.com/aminya/project_options/archive/refs/tags/v0.33.0.zip
)

set(TEXT_PROCESSING_ENABLE_TESTS OFF)
add_dependency(
  NAME stp 
  REPO https://github.com/bialasjaroslaw/stp.git
  HASH cff5d5a945d9551c88a4d167e073cf4faf12ca3e
)
set(IO_DEV_ENABLE_TESTS OFF)
add_dependency(
  NAME io_device
  REPO https://github.com/bialasjaroslaw/io_dev.git
  HASH c93ea6422fa4339bb0a10af16244896fd04a0eaa
)
add_dependency(
  NAME hash
  REPO https://github.com/bialasjaroslaw/hash.git
  HASH 274ebe6f16209da59c230904f328572d6ca902b9
)

use_dependency(nlohmann-json)
use_dependency(fmt)
use_dependency(range-v3)
use_dependency(stp)
use_dependency(hash)
use_dependency(io_device)
use_dependency(_project_options)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

foreach(YEAR RANGE 2015 2023)
	add_custom_target(${YEAR})
	foreach(NUMBER RANGE 1 25)
		add_custom_target(${YEAR}_${NUMBER})
		add_dependencies(${YEAR} ${YEAR}_${NUMBER})
		foreach(VARIANT RANGE 1 2)
			set(CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${YEAR}/c++/${NUMBER}_${VARIANT}.cpp")
			set(CPP_TARGET "${YEAR}_${NUMBER}_${VARIANT}")
			if(EXISTS ${CPP_FILE})
				# message(STATUS "Add target ${CPP_TARGET}")
				add_executable(${CPP_TARGET} ${CPP_FILE})
				target_link_libraries(${CPP_TARGET} 
					PUBLIC 
						fmt::fmt
						range-v3::range-v3
						nlohmann_json::nlohmann_json
						text_processing
						io_dev
						hash
				)
				add_dependencies(${YEAR}_${NUMBER} ${CPP_TARGET})
			endif()
		endforeach()
	endforeach()
endforeach()
