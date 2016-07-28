# Include Environment-based settings.
include(${CMAKE_CURRENT_DOURCE_DIR}../common_settings.txt)

# Create the executable.
add_executable(eth example.cpp link.cpp	link.hpp message.cpp message.hpp netinterface.cpp
	netinterface.hpp node.cpp node.hpp)

# Indicate that the executable needs metasim library.
target_link_libraries( eth metasim )
