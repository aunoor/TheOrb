cmake_policy(SET CMP0074 NEW)

find_package(ZeroMQ REQUIRED)
find_package(zlib REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(CURL)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB SOURCE_FILES *.cpp *.h)
file(GLOB COMMON_SOURCE_FILES common/*.cpp common/*.h)
file(GLOB SERVER_SOURCE_FILES server/*.cpp server/*.h)
file(GLOB LOGGER_SOURCE_FILES logger/*.cpp logger/*.h)
file(GLOB MSGPARSER_SOURCE_FILES msgparser/*.cpp msgparser/*.h)
file(GLOB DBMANAGER_SOURCE_FILES dbmanager/*.cpp dbmanager/*.h dbmanager/postgres/*.cpp dbmanager/postgres/*.h)
file(GLOB EDSMFETCHER_SOURCE_FILES edsmfetcher/*.cpp edsmfetcher/*.h)

include_directories(${CMAKE_CURRENT_LIST_DIR}/server)
include_directories(${CMAKE_CURRENT_LIST_DIR}/common)
include_directories(${ZeroMQ_INCLUDE_DIR})
include_directories(${RAPIDJSON_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIRS})
include_directories(${PostgreSQL_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${COMMON_SOURCE_FILES} ${SERVER_SOURCE_FILES} ${MSGPARSER_SOURCE_FILES} ${DBMANAGER_SOURCE_FILES} ${EDSMFETCHER_SOURCE_FILES} ${LOGGER_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${ZeroMQ_LIBRARY} z ${CURL_LIBRARIES} ${PostgreSQL_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

