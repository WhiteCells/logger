set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC
    main.cc
)

find_package(spdlog REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/thrid_party/sdplog/include)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)
