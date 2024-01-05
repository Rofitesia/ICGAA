find_package(VTK REQUIRED)
set(exec_name main)

set(source_files 
main.cpp
ConvexHull.cpp)

set(header_files
ConvexHull.h)

add_executable(${exec_name}  ${source_files} ${header_files})
  target_link_libraries(${exec_name} PRIVATE ${VTK_LIBRARIES}
)
vtk_module_autoinit(
  TARGETS ${exec_name}
  MODULES ${VTK_LIBRARIES}
)