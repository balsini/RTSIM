# Include Environment-based settings.
include(${CMAKE_CURRENT_DOURCE_DIR}../common_settings.txt)

# Create the executable.
add_executable(markov markov.cpp)

# Indicate that the executable needs metasim library.
target_link_libraries( markov metasim )
