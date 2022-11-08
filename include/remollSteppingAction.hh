
#ifndef __REMOLLSTEPPINGACTION_HH
#define __REMOLLSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4GenericMessenger.hh"
#include "globals.hh"

#include <set>

class G4Material;
class G4GenericMessenger;

class remollSteppingAction : public G4UserSteppingAction
{
  public:
    remollSteppingAction();
    virtual ~remollSteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
    G4GenericMessenger * fStepMessenger;
    G4bool               fKillLateGen;
    G4bool               fKillNotElec;
    G4bool               fKillLessMeV;

    void DefineStepCommands();

  //public:
    void SetKillLateGen();


};



#endif//__REMOLLSTEPPINGACTION_HH
