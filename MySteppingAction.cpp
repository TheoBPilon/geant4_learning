#include "include/MySteppingAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

MySteppingAction::MySteppingAction() {}
MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step *step) {
  G4Track *track = step->GetTrack();
  // 1) Apenas gamma primário
  if (track->GetParticleDefinition()->GetParticleName() != "gamma")
    return;
  if (track->GetParentID() != 0)
    return;
  // 2) Pega processo que definiu o passo
  const G4VProcess *process = step->GetPostStepPoint()->GetProcessDefinedStep();
  if (!process)
    return;
  G4String processName = process->GetProcessName();
  // 3) Ignora transporte
  if (processName == "Transportation")
    return;
  // 4) Energia inicial do fóton no momento da interação
  G4double energy = step->GetPreStepPoint()->GetKineticEnergy() / keV;
  auto analysisManager = G4AnalysisManager::Instance();
  // 5) Classificação do processo
  if (processName == "phot")
    analysisManager->FillNtupleIColumn(0, 0);
  else if (processName == "compt")
    analysisManager->FillNtupleIColumn(0, 1);
  else if (processName == "conv")
    analysisManager->FillNtupleIColumn(0, 2);
  else
    return;

  analysisManager->FillNtupleDColumn(1, energy);
  analysisManager->AddNtupleRow();

  // 6) Mata o gamma após a primeira interação
  track->SetTrackStatus(fStopAndKill);
}
