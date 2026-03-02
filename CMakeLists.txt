cmake_minimum_required(VERSION 3.23)
project(planasql)

set(SRC
    ${PROJECT_SOURCE_DIR}/src/main.c
    ${PROJECT_SOURCE_DIR}/src/buffer.c
    ${PROJECT_SOURCE_DIR}/src/compiler.c
)

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
