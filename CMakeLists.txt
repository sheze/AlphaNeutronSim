# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(AlphaNSimulation)

# Find Geant4 package, activating all available UI and Vis drivers by default
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})

# Locate sources and headers for this project
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

# Add the excutable, and link it to the Geant4 libraries
add_executable(AlphaN AlphaN.cpp ${sources} ${headers})
target_link_libraries(AlphaN ${Geant4_LIBRARIES})

# Copy all scripts to the build directory, i.e. the directory in which we
# build. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(AlphaN_SCRIPTS
  input.mac
  vis.mac
  )

foreach(_script ${AlphaN_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

# Add program to this project targets
add_custom_target(AlphaNSimulation DEPENDS AlphaN)
