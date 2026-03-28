cmake_minimum_required (VERSION 3.30)
project ("AxonIPCTests")

set(CMAKE_CXX_STANDARD 20)

include(cmake/utils.cmake)
config_compiler_and_linker()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)

FetchContent_MakeAvailable(googletest)

# Add source to this project's executable.
add_executable (${PROJECT_NAME}
	"tests/AxonIPCTests.cpp"
	"tests/AxonIPCSubscriberTests.cpp"
	"tests/BinaryReaderTests.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "mocks"
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	AxonIPC
	gtest_main
    gmock
)
