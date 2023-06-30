cmake_minimum_required(VERSION 3.25)
project(boost-beast-deadlock)

set(CMAKE_CXX_STANDARD 20)

add_executable(boost_beast_deadlock main.cpp)

find_package(Boost REQUIRED COMPONENTS system coroutine)
find_package(OpenSSL REQUIRED)
target_include_directories(
  boost_beast_deadlock
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(boost_beast_deadlock PUBLIC OpenSSL::SSL)

option(BOOST_ASIO_ENABLE_HANDLER_TRACKING "Enable boost asio handler tracking"
       ON)
if(BOOST_ASIO_ENABLE_HANDLER_TRACKING)
  add_compile_definitions(BOOST_ASIO_ENABLE_HANDLER_TRACKING)
endif()
