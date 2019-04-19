find_package(Boost REQUIRED)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})
find_package(Threads)

### CppReact
add_library(CppReact
	src/react/src/engine/PulsecountEngine.cpp
	src/react/src/engine/SubtreeEngine.cpp
	src/react/src/engine/ToposortEngine.cpp
	src/react/src/logging/EventLog.cpp
	src/react/src/logging/EventRecords.cpp)
target_link_libraries(CppReact tbb)


## plays
add_executable(source_sink src/source_sink.cpp)
target_link_libraries(source_sink CppReact ${CMAKE_THREAD_LIBS_INIT})

add_executable(reactive_value src/reactive_value.cpp)
target_link_libraries(reactive_value CppReact ${CMAKE_THREAD_LIBS_INIT})