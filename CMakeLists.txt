
# Versão mínima do CMake
cmake_minimum_required(VERSION 3.16...3.27)

# Nome do projeto
project(SimplesMC)

# Opção para UI e visualização (ligada por padrão)
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and visualization drivers" ON)

# Encontrar o Geant4 com UI e Vis
find_package(Geant4 REQUIRED ui_all vis_all)

# Configurar flags, includes e definições do Geant4
include(${Geant4_USE_FILE})

# Diretórios de inclusão locais
include_directories(
    ${PROJECT_SOURCE_DIR}
)

# Arquivos fonte essenciais
set(SOURCE_FILES
    main.cpp
    MyDetector.cpp
    PrimaryGenerator.cpp
    MyRunAction.cpp
    MySteppingAction.cpp
    MyActionInitialization.cpp
)

# Criar o executável
add_executable(simplesMC ${SOURCE_FILES})

# Linkar com as bibliotecas do Geant4
target_link_libraries(simplesMC ${Geant4_LIBRARIES})

# Instalação local (opcional)
install(TARGETS simplesMC DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
