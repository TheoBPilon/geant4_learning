cmake_minimum_required(VERSION 3.16)
project(SimplesMC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_MAJOR_VERSION 6)

find_package(Geant4 REQUIRED ui_all vis_all)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL)

find_package(ROOT REQUIRED COMPONENTS Core RIO Hist Tree)

include(${Geant4_USE_FILE})

set(SOURCE_FILES
    main.cpp
    MyDetector.cpp
    PrimaryGenerator.cpp
    MyRunAction.cpp
    MySteppingAction.cpp
    MyActionInitialization.cpp
    MyEventAction.cpp
)

add_executable(simplesMC ${SOURCE_FILES})

target_include_directories(simplesMC PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
# -------------------------
# Linkagem moderna
# -------------------------
target_link_libraries(simplesMC PRIVATE
    ${Geant4_LIBRARIES}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    ROOT::Core
)

# -------------------------
# Instalação
# -------------------------
install(TARGETS simplesMC DESTINATION ${PROJECT_SOURCE_DIR})
