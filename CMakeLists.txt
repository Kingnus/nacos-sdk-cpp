cmake_minimum_required(VERSION 3.4.1)

project(nacos-cli)

#add_definitions(-DNACOS_AUTH)
#add_definitions(-DNACOS_SPAS)

include_directories (
.
include
)


aux_source_directory( src SRCS_DIRS)
aux_source_directory( src/config SRCS_DIRS)
aux_source_directory( src/constant SRCS_DIRS)
aux_source_directory( src/crypto SRCS_DIRS)
aux_source_directory( src/crypto/base64 SRCS_DIRS)
aux_source_directory( src/crypto/hmac_sha1 SRCS_DIRS)
aux_source_directory( src/crypto/hmac_sha1/hmac SRCS_DIRS)
aux_source_directory( src/crypto/hmac_sha1/sha SRCS_DIRS)
aux_source_directory( src/crypto/md5 SRCS_DIRS)
aux_source_directory( src/factory SRCS_DIRS)
aux_source_directory( src/http SRCS_DIRS)
aux_source_directory( src/http/delegate SRCS_DIRS)
aux_source_directory( src/init SRCS_DIRS)
aux_source_directory( src/json SRCS_DIRS)
aux_source_directory( src/json/rapidjson SRCS_DIRS)
aux_source_directory( src/json/rapidjson/error SRCS_DIRS)
aux_source_directory( src/json/rapidjson/internal SRCS_DIRS)
aux_source_directory( src/json/rapidjson/msinttypes SRCS_DIRS)
aux_source_directory( src/listen SRCS_DIRS)
aux_source_directory( src/log SRCS_DIRS)
aux_source_directory( src/naming SRCS_DIRS)
aux_source_directory( src/naming/beat SRCS_DIRS)
aux_source_directory( src/naming/cache SRCS_DIRS)
aux_source_directory( src/naming/selectors SRCS_DIRS)
aux_source_directory( src/naming/subscribe SRCS_DIRS)
aux_source_directory( src/security SRCS_DIRS)
aux_source_directory( src/server SRCS_DIRS)
aux_source_directory( src/thread SRCS_DIRS)
aux_source_directory( src/utils SRCS_DIRS)
aux_source_directory( test SRCS_DIRS)
aux_source_directory( test/testcase SRCS_DIRS)

aux_source_directory(test TEST_SRCS_DIRS)
aux_source_directory(test/testcase TEST_SRCS_DIRS)

add_executable(nacos-cli.out ${TEST_SRCS_DIRS} ${SRCS_DIRS})
add_library(nacos-cli SHARED ${SRCS_DIRS})
add_library(nacos-cli-static STATIC ${SRCS_DIRS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
target_link_libraries(nacos-cli PRIVATE Threads::Threads curl z)
target_link_libraries(nacos-cli-static PRIVATE Threads::Threads curl z)
target_link_libraries(nacos-cli.out PRIVATE Threads::Threads curl z)
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)

install(TARGETS nacos-cli
		LIBRARY DESTINATION lib)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
		DESTINATION include/nacos
		FILES_MATCHING PATTERN "*.h*")


#install(
#		EXPORT ${PROJECT_NAME}Targets
#		FILE ${PROJECT_NAME}Targets.cmake
#		DESTINATION lib/cmake/mylib
#)

set(INCLUDE_DIRS include)
set(LIBRARIES nacos-cli)
set(LIB_DIR lib)
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
		${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
		VERSION 1
		COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
		${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
		${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
		INSTALL_DESTINATION lib/cmake/mylib
		PATH_VARS INCLUDE_DIRS LIBRARIES LIB_DIR
		INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
)

install(
		FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
		DESTINATION lib/cmake/mylib
)
