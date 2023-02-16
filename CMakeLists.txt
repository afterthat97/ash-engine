cmake_minimum_required(VERSION 3.16)

project(ash-engine)

find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL Network)

add_definitions(-DAPP_VERSION="1.0")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SRC src/*.cpp)
set(RESOURCE resources.qrc)

list(APPEND INCLUDE_DIRS 
	 ${CMAKE_SOURCE_DIR}
	 ${CMAKE_SOURCE_DIR}/src
	 ${CMAKE_SOURCE_DIR}/include
	 ${CMAKE_SOURCE_DIR}/include/Core
	 ${CMAKE_SOURCE_DIR}/include/OpenGL
	 ${CMAKE_SOURCE_DIR}/include/UI
	)

list(APPEND LIBS Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::Network assimp OpenGL)

add_executable(${PROJECT_NAME} ${SRC} ${RESOURCE})

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

# moc
file(GLOB_RECURSE HEADERS_TO_MOC ${CMAKE_SOURCE_DIR}/include/*.h)
qt5_wrap_cpp(ash-engine-moc ${HEADERS_TO_MOC} TARGET ${PROJECT_NAME} OPTIONS --no-notes)
target_sources(${PROJECT_NAME} PRIVATE ${ash-engine-moc})

# install
# install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION include)
# install(DIRECTORY ${CMAKE_SOURCE_DIR}/include LIBRARY DESTINATION include)
