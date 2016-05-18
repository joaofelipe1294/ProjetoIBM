
cmake_minimum_required(VERSION 2.8)
 
project(ProjetoIBM)
 
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
 
add_executable(ProjetoIBM ProjetoIBM.cxx)
target_link_libraries(ProjetoIBM ${ITK_LIBRARIES})
