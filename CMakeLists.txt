cmake_minimum_required(VERSION 3.3)
project(filetoast)

set(SOURCE_FILES src/main.c src/main.h src/log.c src/log.c src/log.h src/supervisor.c src/supervisor.h src/job.c src/job.h src/worker.c src/worker.h)

add_compile_options(-Wall -g)

add_executable(filetoast ${SOURCE_FILES})
target_link_libraries(filetoast pthread)
target_link_libraries(filetoast curl)
target_link_libraries(filetoast config)
set_target_properties(filetoast PROPERTIES RUNTIME_OUTPUT_DIRECTORY /usr/bin)
#set_target_properties(filetoast PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)