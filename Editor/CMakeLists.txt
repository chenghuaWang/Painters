project(p_editor CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE p_editor_include ./inlcude/*.h)
file(GLOB_RECURSE p_editor_src ./src/*.cpp)
file(GLOB_RECURSE p_editor_dep *.h)

# include self.
include_directories(${p_core_include_dir})
include_directories(${p_core_include_dir}/include)
include_directories(${p_mWidget_include_dir})

include_directories(./include)

add_library(p_editor STATIC ${p_editor_include} ${p_editor_src} ${p_editor_dep})
target_link_libraries(p_editor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets p_core p_m_widget)
