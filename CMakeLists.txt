cmake_minimum_required(VERSION 2.82)

file(GLOB_RECURSE SOURCES "src/*.cpp")

project(Box2D-test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pthread -std=c++11 -Wl,-subsystem,windows")

option(BUILD_WINDOWS "Build Windows .exe" ON) # Default
option(BUILD_LINUX "Build Linux executable" OFF)
option(BUILD_VITA "Build Vita .vpk" OFF)

if(BUILD_WINDOWS)
	message(STATUS "Building for Windows")
	add_definitions(-DWindows)

	set(BUILD_WINDOWS_PKGS "C:/Users/Hamme/pkgs" CACHE STRING "Path to the packages, only for Windows")

	set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")
	add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${APP_ICON_RESOURCE_WINDOWS})

	target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
		"${BUILD_WINDOWS_PKGS}/include"
	)

	target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    	"${BUILD_WINDOWS_PKGS}/lib"
	)

	target_link_libraries(${CMAKE_PROJECT_NAME}
		SDL2_image
		SDL2_mixer

		mingw32
		SDL2main

		SDL2
		box2d
	)

	file(COPY src/assets DESTINATION .)

elseif(BUILD_LINUX)
	message(STATUS "Building for Linux")
	add_definitions(-DLinux)

	add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

	target_link_libraries(${CMAKE_PROJECT_NAME}
		SDL2_image
		SDL2_mixer
		SDL2

		box2d
	)

	file(COPY src/assets DESTINATION .)

elseif(BUILD_VITA)
	message(STATUS "Building for Vita")
	add_definitions(-DVita)

	add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

	if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	  if(DEFINED ENV{VITASDK})
		set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
	  else()
		message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
	  endif()
	endif()

	include("${VITASDK}/share/vita.cmake" REQUIRED)

	set(VITA_APPNAME  "Box2D OBJ")
	set(VITA_TITLEID  "HMRL00004")
	set(VITA_VERSION  "01.00")

	set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

	target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
		"$ENV{VITASDK}/arm-vita-eabi/include/"
		"$ENV{VITASDK}/arm-vita-eabi/include/SDL2"
	)

	target_link_libraries(${CMAKE_PROJECT_NAME}
		SDL2_image
		SDL2_mixer
		SDL2

		vorbisfile
		vorbis
		ogg
		mikmod
		modplug
		mpg123
		FLAC
		webp
		jpeg
		png
		z

		SceAudio_stub
		SceAudioIn_stub
		SceCommonDialog_stub
		SceCtrl_stub
		SceDisplay_stub
		SceGxm_stub
		SceHid_stub
		SceSysmodule_stub
		SceTouch_stub
		SceMotion_stub

		box2d
	)

	vita_create_self(${CMAKE_PROJECT_NAME}.self ${CMAKE_PROJECT_NAME})

	vita_create_vpk(${CMAKE_PROJECT_NAME}.vpk ${VITA_TITLEID} ${CMAKE_PROJECT_NAME}.self
		VERSION ${VITA_VERSION}
		NAME ${VITA_APPNAME}

		FILE assets assets
	)

endif()
