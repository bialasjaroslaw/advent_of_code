cmake_minimum_required(VERSION 3.14)
project(AdventOfCode)

include(FetchContent)
FetchContent_Declare(
  stp
  GIT_REPOSITORY https://github.com/bialasjaroslaw/stp.git
  GIT_TAG        df8359facbeaddee20f8f0061e4a3ca7da626dfa
)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        f5e54359df4c26b6230fc61d38aa294581393084
)
FetchContent_Declare(
  io_device
  GIT_REPOSITORY https://github.com/bialasjaroslaw/io_device.git
  GIT_TAG        4e670ca631c6917dadc1fc933a8d08fb97c46b79
)

FetchContent_MakeAvailable(stp)
FetchContent_MakeAvailable(io_device)
FetchContent_MakeAvailable(fmt)

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
				target_link_libraries(${CPP_TARGET} PUBLIC text_processing io_device fmt::fmt)
				add_dependencies(${YEAR}_${NUMBER} ${CPP_TARGET})
			endif()
		endforeach()
	endforeach()
endforeach()
