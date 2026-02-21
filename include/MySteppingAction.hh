#pragma once

#include "G4UserSteppingAction.hh"
class MySteppingAction : public G4UserSteppingAction {
public:
  MySteppingAction();
  virtual ~MySteppingAction();
  virtual void UserSteppingAction(const G4Step *step);
};
