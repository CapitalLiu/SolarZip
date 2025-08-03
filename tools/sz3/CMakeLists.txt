set(CMAKE_CXX_STANDARD 17)            # 设置 C++ 标准为 C++17

# 查找 Python 包含路径和链接标志
execute_process(
    COMMAND python3-config --includes
    OUTPUT_VARIABLE PYTHON_INCLUDE_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND python3-config --ldflags
    OUTPUT_VARIABLE PYTHON_LDFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(Python3_LIBRARIES "/usr/lib/x86_64-linux-gnu/libpython3.10.so")

include_directories(/usr/include/python3.10)
include_directories(/root/qcat/build/include)
file(GLOB cpu_source_files "${CMAKE_SOURCE_DIR}/tools/sz3/*.cpp")
foreach (SRCFILE IN LISTS cpu_source_files)
    get_filename_component(EXE ${SRCFILE} NAME_WLE)
    message(STATUS "Target = ${EXE}")
    add_executable(${EXE} ${EXE}.cpp)
    target_link_libraries(${EXE} PRIVATE SZ3 ${Python3_LIBRARIES} /root/qcat/build/lib/libqcat.a)
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${EXE}$<TARGET_FILE_SUFFIX:${EXE}> TYPE BIN)

endforeach ()
install(FILES testfloat_8_8_128.dat DESTINATION ${CMAKE_INSTALL_DATADIR}/SZ3)

#add_subdirectory(demo)
