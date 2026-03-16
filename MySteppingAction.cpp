#include "include/MySteppingAction.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4UserEventAction.hh"
#include "G4VProcess.hh"
#include "include/MyEventAction.hh"
#include <G4ios.hh>
MySteppingAction::MySteppingAction() {}
MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step *step) {
  auto track = step->GetTrack();
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  // ponteiro para MyEventAction
  auto evtAction =
      dynamic_cast<MyEventAction *>(const_cast<G4UserEventAction *>(
          G4RunManager::GetRunManager()->GetUserEventAction()));

  if (!evtAction) {
    G4cout << "nao ta achando o MyEventAction" << G4endl;
  }

  auto physvol = step->GetPreStepPoint()->GetPhysicalVolume();
  auto post_step_physvol = step->GetPostStepPoint()->GetPhysicalVolume();

  if (!physvol)
    return;
  if (!post_step_physvol)
    return;

  if (physvol->GetName() == "trigger_volume") {
    G4double edep_trigger = step->GetTotalEnergyDeposit();
    if (edep_trigger > 50 * keV) {
      evtAction->SetTriggerPassed();
      evtAction->AddEdepPMT(edep_trigger);
    }
  }

  if (post_step_physvol->GetName() == "Box2") {
    auto edep = step->GetTotalEnergyDeposit();
    if (track->GetDefinition()->GetParticleName() == "gamma" &&
        physvol->GetName() == "Box") {
      auto gamma_kin_energy = step->GetPreStepPoint()->GetKineticEnergy();
      if (gamma_kin_energy > 1 * keV) {
        analysisManager->FillNtupleDColumn(1, gamma_kin_energy);
      }
    }

    evtAction->AddEdep(edep);
  }
}
