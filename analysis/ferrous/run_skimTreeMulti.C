#include "TROOT.h"
#include "TSystem.h"
#include <iostream>

void run_skimTreeMulti(string detlist){
  gROOT->ProcessLine(".L skimTreeMulti.C+");
  std::string command = "skimTreeMulti(\"rootfile.list\",\"" + detlist + "\",1)";
  const char * cCommand = command.c_str();
  cout << cCommand << endl;
  gROOT->ProcessLine( cCommand );
}
