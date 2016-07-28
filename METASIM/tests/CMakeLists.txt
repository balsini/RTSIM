# Add include directory
include_directories(.)
include_directories(../src/)

# Environment-based settings.
if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
elseif(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
elseif(WIN32)

endif()

# Create the executable.
add_executable(test_metasim myentity.cpp TestEntityOrder.cpp TestEntitySameName.cpp TestParseUtil.cpp TestTick.cpp)

# Indicate that rtlib need metasim library.
target_link_libraries(test_metasim metasim)