# Add current directory.
include_directories(.)

# Environment-dependable settings.
if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
	set(METASIM_LIB "SHARED")
	
elseif(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
	set(METASIM_LIB "SHARED")
	
elseif(WIN32)
	set(METASIM_LIB "STATIC")

endif()

# Create a library called "metasim" which includes the source files.
add_library(metasim ${METASIM_LIB} basestat.cpp entity.cpp genericvar.cpp
  regvar.cpp strtoken.cpp trace.cpp debugstream.cpp event.cpp
  randomvar.cpp simul.cpp tick.cpp)
  
# Export.
export(TARGETS metasim FILE "./metasimConfig.cmake")
export(PACKAGE metasim)
  