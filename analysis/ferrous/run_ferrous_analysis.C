#include "TROOT.h"
#include "TSystem.h"
#include <iostream>

void run_ferrous_analysis(Int_t nevents, string detlist){
  gROOT->ProcessLine(".L ferrous_analysis.C+");
  std::string command = "ferrous_analysis f(" + std::to_string(nevents) + ",\"" + detlist + "\")";
  const char * cCommand = command.c_str();
  cout << cCommand << endl;
  gROOT->ProcessLine( cCommand );
}
