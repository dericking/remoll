////////////////////////////////////////////////////////////////////////////////
// skimTreeMulti(string fileList, string DetNums, int beamGen, int test=0)
// >> fileList: Can be string or text file with list of paths
// >> string: DetNums -- "9001,9002,9003" [You get the idea]
// >> beamGen: Ask Ciprian
// >> test: test the file, optional argument per usual c++ rules.

// TODO: Still need to figure out primary and secondary issue...
// Talk to Cip and Kent about thresholds/cuts for secondaries.

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

TFile *ofile;
vector<Int_t>   detectorNumbers;
vector<Int_t>   detectorHitN;
vector<TFile *> outputFiles;
vector<TTree *> outputTrees;
TTree          *otree;
TBranch        *b_rate;
TBranch        *b_hit;
Double_t        newrate(0);
Int_t           testRun(0);
Double_t        scaleRate(1);
Long_t          processOne(string fnm);
Long_t          getEvents(string);

vector<remollGenericDetectorHit_t>  *newhit=0;

void skimTreeMulti(string fileList, string DetNums, int beamGen, int test=0){
  testRun = test;

  //////////////////////////////////////////////////////////////
  //// Detectors to be looked at
  std::stringstream ss(DetNums);
  while(ss.good()){
    string ss_parse;
    getline(ss,ss_parse,',');
    detectorNumbers.push_back( std::atoi(ss_parse.c_str()) );
    detectorHitN.push_back(0); //Initialize to zero.
  }

  long nTotHits(0);
  int nFiles(0);

  if(fileList==""){
    cout<<"\t did not find input file. Quitting!"<<endl;
    return;
  }

  //////////////////////////////////////////////////////////////
  //// Create output files and trees
  otree     = new TTree("T", "ferrous skim tree");
  b_rate    = otree->Branch("rate", &newrate);
  b_hit     = otree->Branch("hit",  &newhit );
  for(Int_t i = 0; i < detectorNumbers.size(); i++){
    cout << "Creating pointer to TFile named " << Form("o_remollSkimTree%d.root",detectorNumbers[i]) << endl;
    outputFiles.push_back( new TFile(Form("o_remollSkimTree%d.root",detectorNumbers[i]),"RECREATE") );
    otree->SetObject("T",Form("ferrous skim tree %i",detectorNumbers[i]));
    //otree->SetObject(Form("T%i",detectorNumbers[i]),Form("ferrous skim tree %i",detectorNumbers[i])); //for debugging
    outputTrees.push_back( otree->CloneTree(0) ); //Create a clone of tree and copy 0 entries
  }
  otree->SetObject("T","ferrous skim tree");

  //////////////////////////////////////////////////////////////
  //// Keep code from original script
  if( fileList.find(".root") < fileList.size() ){
    cout<<"Processing single file:\n\t"<<fileList<<endl;
    if(beamGen){
      scaleRate = getEvents(fileList);
    }
    nTotHits+=processOne(fileList);
    nFiles=1;
  }else{
    cout<<"Attempting to process list of output from\n\t"<<fileList<<endl;
    ifstream ifile(fileList.c_str());
    string data;
    while(ifile>>data){
      nFiles++;
      if(beamGen) scaleRate += getEvents(data);
      if(testRun==1 && nFiles>10) break;
    }
    ifile.close();
    if(!beamGen) scaleRate = nFiles;
    nFiles=0;

    cout<<"\t scale rate by "<<scaleRate<<endl;

    ifile.open(fileList.c_str());
    while(ifile>>data){
      nFiles++;
      cout<<" processing: "<<data<<endl;
      nTotHits+=processOne(data);
      if(testRun==1 && nFiles>10)
        break;
    }
  }

  for(Int_t i = 0; i < detectorNumbers.size(); i++){
    outputFiles[i]->cd();
    outputTrees[i]->Write();
    outputFiles[i]->Close();
    cout << "Ferrous Detector " << detectorNumbers[i] << " had " << detectorHitN[i] << " hits recorded." << endl;
  }
}

long getEvents(string fnm){
  TFile* ifile = new TFile(fnm.c_str(),"READ");
  TTree* itree = (TTree*)ifile->Get("T");
  long evn = itree->GetEntries();
  ifile->Close();
  delete ifile;

  return evn;
}


long processOne(string fnm){

  TFile* ifile = new TFile(fnm.c_str(),"READ");
  TTree* itree = (TTree*)ifile->Get("T");

  if(itree == nullptr) return 0;

  long nEntries = itree->GetEntries();
  cout<<"\tTotal events: "<<nEntries<<endl;

  Double_t rate;
  vector<remollGenericDetectorHit_t>  *hit=0;
  itree->SetBranchAddress("rate", &rate);
  itree->SetBranchAddress("hit", &hit);

  long nHits(0);
  double tempnum;
  for (long i=0; i < nEntries;i++){
    itree->GetEntry(i);
    tempnum=0;
    for(int j=0; j<hit->size(); j++){
      vector<Int_t> hitTracks;
      if((*hit)[j].trid == 1){
        if( !(std::find(hitTracks.begin(), hitTracks.end(), (*hit)[j].trid) != hitTracks.end()) ){
          hitTracks.push_back( (*hit)[j].trid );
          newhit->push_back((*hit)[j]);
          newrate = rate/scaleRate;
          if(rate == 0) newrate = 1/scaleRate;
          nHits++;
          std::vector<Int_t>::iterator iter = std::find(detectorNumbers.begin(), detectorNumbers.end(), (*hit)[j].det );
          if( iter != detectorNumbers.end() ){
            //cout << "Index for " << (*hit)[j].det << " in vector<Int_t> detectorNumbers is "
            //     << std::distance( detectorNumbers.begin(), iter ) << endl;
            Int_t index = std::distance( detectorNumbers.begin() , iter ); //create stack object and use 'index'
            outputFiles[index]->cd();
            outputTrees[index]->Fill();
            //cout << outputTrees[index]->GetName() << " now has " << outputTrees[index]->GetEntries() << " entries." << endl;
            detectorHitN[index]++;
            newhit->clear();
            newrate = 0;
          }else{
            //This is not necessarily an error... what if I accidentally left a
            //detector on, say 28, that's not a ferrous volume.
            cout << "WARNING! Detector number " << (*hit)[j].det
                 << " not found in ferrous detectors list." << endl;
            newhit->clear();
          }
        }else{
          //if the hit is alread in the collection... do we want to do anything???
          cout << "Hit already recorded once." << endl;
        }
      }
    }
  }
  ifile->Close();
  delete ifile;

  return nHits;
}
