add_executable(resolver resolver.cpp)
target_link_libraries(resolver Boost::context Boost::system Threads::Threads)
