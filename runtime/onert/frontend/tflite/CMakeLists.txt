if(NOT BUILD_TFLITE_LOADER)
  return()
endif(NOT BUILD_TFLITE_LOADER)

set(TFLITE_LOADER_SOURCES src/tflite_loader.cc)

add_library(tflite_loader STATIC ${TFLITE_LOADER_SOURCES})
set_target_properties(tflite_loader PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(tflite_loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(tflite_loader PRIVATE onert_core)
target_link_libraries(tflite_loader PRIVATE base_loader nnfw_common nnfw_coverage)
