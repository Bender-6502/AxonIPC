cmake_minimum_required (VERSION 3.30)
project ("AxonIPCExample")

set(CMAKE_CXX_STANDARD 20)

include(cmake/utils.cmake)
config_compiler_and_linker()

# Add source to this project's executable.
add_executable (North
		"src/North.cpp"
)

add_executable (South
		"src/South.cpp"
)

target_link_libraries(North PUBLIC
		AxonIPC
)

target_link_libraries(South PUBLIC
		AxonIPC
)