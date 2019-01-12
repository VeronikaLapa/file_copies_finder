CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(finder_tests
        LANGUAGES CXX)


add_definitions(-DGTEST_LANGUAGE_CXX11)

find_package(Threads REQUIRED)
set (CMAKE_CXX_FLAGS "-std=c++11")
if ($ENV{GOOGLETEST_DIR})
    SET(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
else ()
    message(WARNING "Using googletest src dir specified at Qt Creator wizard")
    SET(GOOGLETEST_DIR "D:/googletest-master")
endif ()
if (EXISTS ${GOOGLETEST_DIR})
    SET(GTestSrc ${GOOGLETEST_DIR}/googletest)
    SET(GMockSrc ${GOOGLETEST_DIR}/googlemock)
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif ()


include_directories(${GTestSrc} ${GTestSrc}/include ${GMockSrc} ${GMockSrc}/include)

add_executable(${PROJECT_NAME} main.cpp tst_finder.h
               ${GTestSrc}/src/gtest-all.cc
               ${GMockSrc}/src/gmock-all.cc)
add_test(${PROJECT_NAME} COMMAND ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

