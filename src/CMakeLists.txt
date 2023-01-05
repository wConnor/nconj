set(PROJECT_SOURCES
  main.cpp
  session.cpp
  menu.cpp
  deck.cpp
  )

add_custom_target(run
	COMMAND ${CMAKE_PROJECT_NAME}
	DEPENDS ${CMAKE_PROJECT_NAME}
	WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${CURSES_LIBRARIES} Threads::Threads)
