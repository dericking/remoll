#include <iostream>
#include <fstream>
#include <vector>
#include "include/remollMagneticField.hh"
#include "G4SystemOfUnits.hh"
#include <string>
#include <cmath>

using namespace std;

int main(){

  string fieldMap = "../map_directory/V2DSg.9.75cm.txt";

  remollMagneticField * magField;

  magField = new remollMagneticField(fieldMap);

  G4bool keepGoing = true;

  G4double px;
  G4double py;
  G4double pz;

  G4cout << " >>> QUICK AND DIRTY FIELD PROBER. CTRL-C TO EXIT. <<< " << G4endl << G4endl;

  while(keepGoing){
    cout << "Enter global x-coordinate: ";
    cin  >> px;
    cout << "Enter global y-coordinate: ";
    cin  >> py;
    cout << "Enter global z-coordinate: ";
    cin  >> pz;

    px *= m;
    py *= m;
    pz *= m;

    //Must use 4-position
    G4double pt[4] = {px,py,pz,0};
    //MolPol EM Field is B & E ... B: 0,1,2 -and- E: 3,4,5
    G4double B[6] = {0.,0.,0.,0.,0.,0.};

    magField->GetFieldValue(pt,B);

    cout << "Returned field value @ Point ( "
         << px / cm << " , " << py / cm << " , " << pz / cm << " , 0 "
         << " ) cm is ( " << B[0] / tesla * 10000 << " , " << B[1] / tesla * 10000 << " , "
         << B[2] / tesla * 10000 << " ) Gauss" << endl;

    G4cout << G4endl;
  }

}
