set(CMAKE_CXX_STANDARD 17)            # 设置 C++ 标准为 C++17

# 添加 qcat 子目录
add_subdirectory(include/qcat)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/tools/sz3/include)
file(GLOB cpu_source_files "${CMAKE_SOURCE_DIR}/tools/sz3/*.cpp")
foreach (SRCFILE IN LISTS cpu_source_files)
    get_filename_component(EXE ${SRCFILE} NAME_WLE)
    message(STATUS "Target = ${EXE}")
    add_executable(${EXE} ${EXE}.cpp)
    target_link_libraries(${EXE} PRIVATE SZ3 Python3::Python qcat)
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${EXE}$<TARGET_FILE_SUFFIX:${EXE}> TYPE BIN)

endforeach ()
install(FILES DESTINATION ${CMAKE_INSTALL_DATADIR}/SZ3)

#add_subdirectory(demo)
