cmake_minimum_required(VERSION 2.8.12)
project(radioparadise-player)

find_package(Qt5 REQUIRED COMPONENTS Gui Qml Quick)

qt5_add_resources(resources resources.qrc)

add_executable(radioparadise-player
	main.cpp
	${resources}
)

target_link_libraries(radioparadise-player
	Qt5::Gui
	Qt5::Qml
	Qt5::Quick
)
