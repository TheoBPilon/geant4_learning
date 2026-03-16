#include "FTFP_BERT.hh"
#include "G4DecayPhysics.hh"
#include "G4OpticalParameters.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "Shielding.hh"
// incluindo o detector
#include "include/MyActionInitialization.hh"
#include "include/MyDetector.hh"

int main(int argc, char **argv) {
  // criando interface do usuario para executar comandos
  auto ui = new G4UIExecutive(argc, argv);
  // criando o gerenciados do experimento
  auto runManager = new G4RunManager();
  // Registrando a classe de geometria no Kernel
  runManager->SetUserInitialization(new MyDetector());
  // Registrando a physics list no Kernel
  auto physicsList = new Shielding();
  physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics());
  physicsList->RegisterPhysics(new G4DecayPhysics());
  physicsList->RegisterPhysics(new G4OpticalPhysics());
  // ativar cintilacao por particula
  auto op_param = G4OpticalParameters::Instance();
  auto opticalPhysics = new G4OpticalPhysics();

  physicsList->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new MyActionInitialization());
  runManager->Initialize(); // inicia o kernel

  // Inicializando o visualizador
  auto visManager = new G4VisExecutive();
  visManager->Initialize();

  // pegar ponteiro para o gerenciador de comandos do Geant4
  auto uimanager = G4UImanager::GetUIpointer();
  uimanager->ApplyCommand("/control/execute ../macros/non_vis.mac");
  // Sessão interativa
  ui->SessionStart();
  // liberando recursos alocados
  delete visManager;
  delete runManager;
  delete ui;

  return 0;
}
