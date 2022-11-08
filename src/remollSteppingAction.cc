#include "remollSteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"

remollSteppingAction::remollSteppingAction(): 
  fStepMessenger(0),
  fKillLateGen(0),
  fKillNotElec(0),
  fKillLessMeV(0)
{
  DefineStepCommands();
}

remollSteppingAction::~remollSteppingAction()
{
  if(fStepMessenger) delete fStepMessenger;
}

void remollSteppingAction::UserSteppingAction(const G4Step *aStep)
{

    G4Track * track = aStep->GetTrack();

    if( (track->GetTotalEnergy()/MeV < 1.0) ){
        track->SetTrackStatus(fStopAndKill); return;
    }

    //Kill any particles who are not beam or beam-daughter
    if(fKillLateGen){
        if( (track->GetParentID() > 1) ){
            track->SetTrackStatus(fStopAndKill); return;
        }
    }
}

void remollSteppingAction::SetKillLateGen(){
  fKillLateGen = true;
}

void remollSteppingAction::DefineStepCommands(){
  //Create Stepping Action Messenger
  G4cout << "Defining the Step Messenger..." << G4endl;

  fStepMessenger = new G4GenericMessenger(this,"/remoll/step/","Remoll Stepping Action Control Options");

  G4cout << "Defining the Step Commands..." << G4endl;

  auto& CmdKillLateGen = fStepMessenger->DeclareMethod("killLateGen", &remollSteppingAction::SetKillLateGen,"Kill all particles not beam or beam-daughter");
  //  CmdKillLateGen.SetParameterName("killLateGen",true);

  G4cout << "Defined the Step Commands..." << G4endl;

}
