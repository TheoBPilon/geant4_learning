#include "include/MyRunAction.hh"
#include "G4AnalysisManager.hh"

MyRunAction::MyRunAction() {
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
}

MyRunAction::~MyRunAction() { delete G4AnalysisManager::Instance(); }

void MyRunAction::BeginOfRunAction(const G4Run *run) {
  // cria instancia do manager de enalise
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("gammaInteractions.root"); // abre arquivo .root
  analysisManager->CreateNtuple("interactions", "gamma process"); // cria ntuple
  analysisManager->CreateNtupleIColumn(
      "processID"); // cria linha com id da particula
  analysisManager->CreateNtupleDColumn("energy"); // cria coluna com energia

  analysisManager->FinishNtuple();
}

void MyRunAction::EndOfRunAction(const G4Run *run) {
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
