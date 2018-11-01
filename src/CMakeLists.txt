add_executable(resolver resolver.cpp)
target_link_libraries(resolver Boost::system Boost::coroutine Boost::context Threads::Threads)
