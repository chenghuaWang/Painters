project(p_core CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE p_core_include ./inlcude/*.h)
file(GLOB_RECURSE p_core_src ./src/*.cpp)
file(GLOB_RECURSE p_core_dep *.h)

add_library(p_core STATIC ${p_core_include} ${p_core_src} ${p_core_dep})
target_link_libraries(p_core PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
