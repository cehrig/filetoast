cmake_minimum_required(VERSION 3.3)
project(filetoast)

set(FILETOAST_REQUIRED_LIBS pthread crypto config curl m)

foreach(REQUIRED_LIB ${FILETOAST_REQUIRED_LIBS})
    SET(LIBTEMP_FOUND LIBTEMP_FOUND-NOTFOUND)
    find_library(LIBTEMP_FOUND ${REQUIRED_LIB})

    if(LIBTEMP_FOUND)
        MESSAGE("found ${REQUIRED_LIB}")
    else()
        MESSAGE(FATAL_ERROR "could not find lib${REQUIRED_LIB}")
    endif()
endforeach(REQUIRED_LIB)

set(SOURCE_FILES src/main.c src/log.c src/supervisor.c src/job.c src/worker.c src/ssl.c src/reverse.c)

add_compile_options(-Wall -g)

add_executable(filetoast ${SOURCE_FILES})
target_link_libraries(filetoast pthread)
target_link_libraries(filetoast crypto)
target_link_libraries(filetoast curl)
target_link_libraries(filetoast config)
target_link_libraries(filetoast m)
set_target_properties(filetoast PROPERTIES RUNTIME_OUTPUT_DIRECTORY /usr/bin)
#set_target_properties(filetoast PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)