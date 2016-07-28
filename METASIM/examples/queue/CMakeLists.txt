# Include Environment-based settings.
include(${CMAKE_CURRENT_DOURCE_DIR}../common_settings.txt)

# Create the executable.
add_executable(queue queue.cpp queue.hpp)

# Indicate that the executable needs metasim library.
target_link_libraries( queue metasim )
