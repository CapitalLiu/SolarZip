add_library (qcat  
  src/ByteToolkit.c
  src/DynamicByteArray.c
  src/DynamicDoubleArray.c
  src/DynamicFloatArray.c
  src/DynamicIntArray.c
  src/Huffman.c
  src/qcat_compressionAnalysis.c
  src/qcat_dataAnalysis.c
  src/qcat_gnuplot.c
  src/qcat_hashtable.c
  src/qcat_ssim.c
  src/rw.c
  src/sz_dummy_compression.c
  src/sz_utility.c
  )
target_include_directories(qcat PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/qcat/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/qcat>
  )
# set(ZSTD_LIBRARY "/usr/lib/x86_64-linux-gnu/libzstd.a")
  
target_link_libraries(qcat PUBLIC m)

# target_link_libraries(qcat PUBLIC ${ZSTD_LIBRARY})
# endif()


install (TARGETS qcat EXPORT QcatTargets)
install (DIRECTORY ./include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/qcat)
install(EXPORT QcatTargets NAMESPACE Qcat:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Qcat/)
include(CMakePackageConfigHelpers)
#configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/QcatConfig.cmake.in
#  "${CMAKE_CURRENT_BINARY_DIR}/QcatConfig.cmake"
#  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Qcat
#)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/QcatConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/QcatConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/QcatConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Qcat
)
