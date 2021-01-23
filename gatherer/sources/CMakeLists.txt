
find_package(ZeroMQ)
find_package(zlib)
find_package(RapidJSON)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)

file(GLOB SERVER_HEADER_FILES server/*.h)
file(GLOB SERVER_SOURCE_FILES server/*.cpp)

file(GLOB COMMON_HEADER_FILES common/*.h)
file(GLOB COMMON_SOURCE_FILES common/*.cpp)

file(GLOB MSGPARSER_HEADER_FILES msgparser/*.h)
file(GLOB MSGPARSER_SOURCE_FILES msgparser/*.cpp)

include_directories(${CMAKE_CURRENT_LIST_DIR}/server)
include_directories(${CMAKE_CURRENT_LIST_DIR}/common)
include_directories(${ZeroMQ_INCLUDE_DIR})
include_directories(${RAPIDJSON_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SERVER_SOURCE_FILES} ${SERVER_HEADER_FILES}
               ${MSGPARSER_HEADER_FILES} ${MSGPARSER_SOURCE_FILES} ${COMMON_HEADER_FILES} ${COMMON_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${ZeroMQ_LIBRARY} z)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

