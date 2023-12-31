file(GLOB_RECURSE PYTHON_BINDING_LIB_SRC ${CMAKE_SOURCE_DIR}/src/python_bindings/*.cpp)

add_library(py SHARED ${PYTHON_BINDING_LIB_SRC})
set_target_properties(py PROPERTIES OUTPUT_NAME "hal_py")
add_library(hal::py ALIAS py)
target_include_directories(py
                           PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
                           $<BUILD_INTERFACE:${Python3_INCLUDE_DIRS}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                           PRIVATE
                           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/python_bindings/bindings>
                           )
target_compile_options(py
                       PUBLIC ${COMPILE_OPTIONS_PUBLIC}
                       PRIVATE ${COMPILE_OPTIONS_PRIVATE}
                       INTERFACE ${COMPILE_OPTIONS_INTERFACE})
#Set shared library suffix for MacOS; Used for boost_python support
set_target_properties(py PROPERTIES PREFIX "")
if(APPLE AND CMAKE_HOST_APPLE)
    set_target_properties(py PROPERTIES SUFFIX ".so")
endif(APPLE AND CMAKE_HOST_APPLE)
target_link_libraries(py PUBLIC PRIVATE ${Python3_LIBRARIES} pybind11::module hal::netlist hal::utilities hal::plugin_system)
install(TARGETS py
        EXPORT hal
        LIBRARY DESTINATION ${LIBRARY_INSTALL_DIRECTORY}
        PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        OWNER_EXECUTE
        GROUP_READ
        GROUP_EXECUTE
        WORLD_READ
        WORLD_EXECUTE
        INCLUDES DESTINATION ${INCLUDE_INSTALL_DIRECTORY})
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_sanitizers(py)
endif()

# file(GLOB_RECURSE PYTHON_PLUGIN_LIB_HDR ${CMAKE_SOURCE_DIR}/include/python_binding/*.h)

# add_library(hal_python SHARED ${PYTHON_PLUGIN_LIB_HDR} ${PYTHON_PLUGIN_LIB_source})
# target_include_directories(hal_python
#                            PUBLIC
#                            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
#                            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
#                            $<BUILD_INTERFACE:${Python3_INCLUDE_DIRS}>
#                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
#                            )
# target_compile_options(hal_python
#                        PUBLIC ${COMPILE_OPTIONS_PUBLIC}
#                        PRIVATE ${COMPILE_OPTIONS_PRIVATE}
#                        INTERFACE ${COMPILE_OPTIONS_INTERFACE})
# set_target_properties(hal_python PROPERTIES DEFINE_SYMBOL API_PYTHON_PLUGIN)
# target_link_libraries(hal_python PUBLIC hal::core hal::netlist PRIVATE ${Python3_LIBRARIES})
# install(TARGETS hal_python
#         LIBRARY DESTINATION ${LIBRARY_INSTALL_DIRECTORY}
#         PERMISSIONS
#         OWNER_READ
#         OWNER_WRITE
#         OWNER_EXECUTE
#         GROUP_READ
#         GROUP_EXECUTE
#         WORLD_READ
#         WORLD_EXECUTE
#         INCLUDES DESTINATION ${INCLUDE_INSTALL_DIRECTORY})
# if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#     add_sanitizers(hal_python)
# endif()
