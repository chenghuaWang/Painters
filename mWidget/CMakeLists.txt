project(p_m_widget CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE p_m_widget_include ./inlcude/*.h)
file(GLOB_RECURSE p_m_widget_src ./src/*.cpp)
file(GLOB_RECURSE p_m_widget_dep *.h)

# include self.
include_directories(${p_core_include_dir})
include_directories(${p_core_include_dir}/include)
include_directories(${p_mWidget_include_dir})

add_library(p_m_widget STATIC ${p_m_widget_include} ${p_m_widget_src} ${p_m_widget_dep})
target_link_libraries(p_m_widget PRIVATE Qt${QT_VERSION_MAJOR}::Widgets p_core)
