#pragma once

#include "G4UserRunAction.hh"

class MyRunAction : public G4UserRunAction {
public:
  MyRunAction();
  virtual ~MyRunAction();
  virtual void BeginOfRunAction(const G4Run *run);
  virtual void EndOfRunAction(const G4Run *run);
};
