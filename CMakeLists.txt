cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
set(ARCH "x86_64")
set(CMAKE_CXX_STANDARD 26)
cmake_policy(SET CMP0100 NEW)

#Base flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wformat-security -Wno-unused -Wno-cast-function-type -Wno-missing-field-initializers")

#Build type specific flags
if(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  set(REL_BUILD TRUE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -Ofast")
  #set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  add_definitions(-DRELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -ggdb3 -g -O0")
    add_definitions(-DDEBUG)
endif()

#Platform-specific flags
if(WIN32 OR WIN64)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  add_definitions(-DWINDOWS)
  if(REL_BUILD)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--high-entropy-va -Wl,--dynamicbase -Wl,--nxcompat")
  endif()
elseif(UNIX)
  add_definitions(-DLINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
elseif(APPLE)
  add_definitions(-DOSX)
endif()

set(SOURCE
        src/main.cc
        src/languages.hh
        src/textFeatures.cc src/textFeatures.hh
        src/textLanguage.hh)
set(LIBS harfbuzz freetype fontconfig icuuc)

project(HarfbuzzTest)
add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} ${LIBS})

#Copy the files the editor needs to run into the build directory
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/fonts" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
