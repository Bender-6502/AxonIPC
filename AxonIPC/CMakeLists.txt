cmake_minimum_required (VERSION 3.30)
project ("AxonIPC")

set(CMAKE_CXX_STANDARD 20)

include(cmake/utils.cmake)
config_compiler_and_linker()

add_library(${PROJECT_NAME} STATIC 
    "src/BinaryReader.cpp"
    "src/BinaryWriter.cpp"
    "src/Context.cpp"
    "src/Dispatcher.cpp"
    "src/AxonIPCSubscriber.cpp"
    "src/AxonIPCPublisher.cpp"
    "src/Path.cpp"
    "src/Platform.cpp"
    "src/Publisher.cpp"
    "src/Socket.cpp"
    "src/Subscriber.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    "."
    PRIVATE
    "AxonIPC"
)
