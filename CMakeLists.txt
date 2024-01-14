cmake_minimum_required(VERSION 3.14)
project(AdventOfCode)


include(Packages.cmake)
add_dependency(
  NAME stp
  REPO https://github.com/bialasjaroslaw/stp.git
  HASH 6199d4a85db7c6e521d3fd58c47984760ef85f44
)
set(IO_DEV_ENABLE_TESTS OFF)
add_dependency(
  NAME io_device
  REPO https://github.com/bialasjaroslaw/io_dev.git
  HASH 02433eb95a3fa2135aaadb2bd99553533e0aacd3
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
  NAME hash
  REPO https://github.com/bialasjaroslaw/hash.git
  HASH e7cde339889e1ae375417d047397ecbf9632b71d
)

use_dependency(stp)
use_dependency(io_device)
use_dependency(fmt)
use_dependency(range-v3)
use_dependency(hash)

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
				target_link_libraries(${CPP_TARGET} PUBLIC text_processing io_dev hash fmt::fmt range-v3::range-v3)
				add_dependencies(${YEAR}_${NUMBER} ${CPP_TARGET})
			endif()
		endforeach()
	endforeach()
endforeach()
