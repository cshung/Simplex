set (CMAKE_CXX_STANDARD 11)

add_executable(simplex-tests main.cpp simplex-tests.cpp)

target_link_libraries(simplex-tests
    simplex
    libgtest
    libgmock
)

add_test(NAME simplex-tests
         COMMAND ./simplex-tests)
