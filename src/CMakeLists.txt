set(PROJECT_SOURCES
  main.cpp
  session.cpp
  menu.cpp
  deck.cpp
  database.cpp
  )

add_custom_target(run
	COMMAND ${CMAKE_PROJECT_NAME}
	DEPENDS ${CMAKE_PROJECT_NAME}
	WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

set(CMAKE_CXX_STANDARD 17)
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(SQLite3 REQUIRED)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${SQLite3_LIBRARIES} ${CURSES_LIBRARIES} Threads::Threads)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra)
