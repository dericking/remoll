#include "TROOT.h"
#include "TSystem.h"
#include <iostream>

void run_skimTreeMulti(string detlist){
  gRoot->ProcessLine(".L skimTreeMulti.C+");
  std::string command = "skimTreeMulti(\"rootfile.list\",\"" + detlist + "\")";
  const char * cCommand = command.c_str();
  cout << cCommand << endl;
  gROOT->ProcessLine( cCommand );
}
