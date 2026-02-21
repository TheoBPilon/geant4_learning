#include "include/MyActionInitialization.hh"
#include "include/MyRunAction.hh"
#include "include/MySteppingAction.hh"
#include "include/PrimaryGenerator.hh"

MyActionInitialization::MyActionInitialization() {}

MyActionInitialization::~MyActionInitialization() {}

void MyActionInitialization::BuildForMaster() const {
  SetUserAction(new MyRunAction);
}

void MyActionInitialization::Build() const {
  SetUserAction(new MyPrimaryGenerator);
  SetUserAction(new MyRunAction);
  SetUserAction(new MySteppingAction);
}
