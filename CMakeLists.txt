﻿cmake_minimum_required (VERSION 3.8 )
project ("ChatClient" VERSION 0.0.1 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ORG_NAME "MyOrg")
message("COMPILER${CMAKE_CXX_COMPILER}")
find_package(Qt6 REQUIRED COMPONENTS Core  Qml Quick WebSockets Network Concurrent QuickControls2 LinguistTools)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})

qt_policy(SET QTP0001 NEW)
add_subdirectory (src)
add_subdirectory (lib)
add_subdirectory (tests)
#qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} appp_ru.ts)configure_file(${QM_FILES} ${CMAKE_SOURCE_DIR} COPYONLY)
