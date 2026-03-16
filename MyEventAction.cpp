#include "include/MyEventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
using namespace CLHEP;
using namespace std;
MyEventAction::MyEventAction() {}

MyEventAction::~MyEventAction() {}

void MyEventAction::BeginOfEventAction(const G4Event *anEvent) {
  total_edep = 0;
  total_edep_pmt = 0;
  ftriggerpassed = false;
}

void MyEventAction::EndOfEventAction(const G4Event *anEvent) {

  if (total_edep > 10 * keV && total_edep_pmt > 10 * keV) {

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, total_edep);
    analysisManager->FillNtupleDColumn(3, total_edep_pmt);
    analysisManager->AddNtupleRow();
  }
}
