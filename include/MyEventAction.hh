#pragma once
#include "G4UserEventAction.hh"
#include <G4Types.hh>
class MyEventAction : public G4UserEventAction {
public:
  MyEventAction();
  virtual ~MyEventAction();
  virtual void BeginOfEventAction(const G4Event *anEvent);
  virtual void EndOfEventAction(const G4Event *anEvent);

  void AddEdep(G4double edep) { total_edep += edep; }
  void AddEdepPMT(G4double edep) { total_edep_pmt += edep; }
  void AddGammaKinEnergy(G4double kin_energy) {
    gamma_kin_energy += kin_energy;
  }
  void SetTriggerPassed() { ftriggerpassed = true; }
  G4bool GetTriggerPassed() { return ftriggerpassed; }

private:
  G4double total_edep;
  G4double total_edep_pmt;
  G4double gamma_kin_energy;
  G4bool ftriggerpassed;
};
