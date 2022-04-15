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
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include "remolltypes.hh"
using namespace std;

TFile          *ofile;
vector<Int_t>   detectorNumbers;
vector<TFile *> outputFiles;
vector<TTree *> outputTrees;
TTree          *otree;
TBranch        *b_rate;
TBranch        *b_hit;
Double_t        newrate(0);
Int_t           testRun(0);
Int_t           generation(0);
Double_t        scaleRate(1);
Long_t          processOne(string fnm);
Long_t          getEvents(string);
Long_t          eventsSum(0);
std::clock_t    startTime;
std::clock_t    endTime;
Long_t          eventsSum;

vector<remollGenericDetectorHit_t>  *newhit=0;
vector< vector<Int_t> >             detectorHitN;

void skimTreeMulti(string fileList, string DetNums, Int_t gencut=0, int beamGen=1, int test=0){
  startTime = std::clock();
  std::ofstream fout;
  fout.open("ferrous_skimTree_results.txt");

  testRun    = test;
  generation = gencut;

  std::ofstream fout;
  fout.open("ferrous_skimTree_results.txt");


  //////////////////////////////////////////////////////////////
  //// Detectors to be looked at
  std::stringstream ss(DetNums);
  while(ss.good()){
    string ss_parse;
    getline(ss,ss_parse,',');
    detectorNumbers.push_back( std::atoi(ss_parse.c_str()) );
    vector<Int_t> tempVec;
    for(Int_t g=0; g<=generation; g++) tempVec.push_back(0);
    detectorHitN.push_back( tempVec );
  }
  cout << "Detectors to be examined: ";
  for(Int_t k=0; k<detectorNumbers.size(); k++){
    if(k==0 && detectorNumbers.size()==1){
      cout << "{" << detectorNumbers[k] << "}";
    }else if(k==0){
      cout << "{" << detectorNumbers[k];
    }else if(k==(detectorNumbers.size()-1)){
      cout << "," << detectorNumbers[k] << "}" << endl;
    }else{
      cout << "," << detectorNumbers[k];
    }
  }
  cout << "Recording all particles whose mother is <=" << generation << endl;

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
    //cout<<"Processing single file:\n\t"<<fileList<<endl;
    if(beamGen){
      scaleRate = getEvents(fileList);
    }
    nTotHits+=processOne(fileList);
    nFiles=1;
  }else{
    //cout<<"Attempting to process list of output from\n\t"<<fileList<<endl;
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
    //cout<<"\t scale rate by "<<scaleRate<<endl;
    ifile.open(fileList.c_str());
    while(ifile>>data){
      nFiles++;
      //cout<<" processing: "<<data<<endl;
      nTotHits+=processOne(data);
      if(testRun==1 && nFiles>10)
        break;
    }
  }

  fout << "Total events counted: " << eventsSum << endl;
  fout << "(Be sure this matches events run)" << endl << endl;

  fout << "Ferrout Detector Hits (e- only):" << endl;
  for(Int_t g=0; g<=generation; g++){
    if(g==0) fout << std::setw(10) << "Det\\mTrid";
    fout << fixed << std::setw(7) << g;
  }
  fout << fixed << std::setw(7) << "TOTAL" << endl;
  for(Int_t d=0; d<detectorNumbers.size(); d++){
    outputFiles[d]->cd();
    outputTrees[d]->Write();
    outputFiles[d]->Close();
    Int_t gTotal(0);
    for(Int_t g=0; g<=generation; g++){
      if(g==0) fout << fixed << std::setw(10) << detectorNumbers[d];
      fout << fixed << std::setw(7) << detectorHitN[d][g];
      gTotal+=detectorHitN[d][g];
      if(g==generation) fout << fixed << std::setw(7) << gTotal << endl;
    }
  }

  endTime = std::clock();

  fout << "Total time elapsed for analysis: " << (endTime-startTime)/(double)CLOCKS_PER_SEC << " seconds." << endl;

  fout.close();

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
  eventsSum += itree->GetEntries();
  //cout<<"\tTotal events: "<<nEntries<<endl;
  eventsSum+=nEntries;

  Double_t rate;
  vector<remollGenericDetectorHit_t>  *hit=0;
  itree->SetBranchAddress("rate", &rate);
  itree->SetBranchAddress("hit", &hit);

  long nHits(0);
  double tempnum;
  for (long i=0; i < nEntries;i++){
    itree->GetEntry(i);
    tempnum=0;
    vector<Int_t> hitTracks;
    for(int j=0; j<hit->size(); j++){
      if((*hit)[j].mtrid <= generation && abs((*hit)[j].pid)==11){
        std::vector<Int_t>::iterator iterTrid = std::find(hitTracks.begin(), hitTracks.end(), (*hit)[j].trid );
        if( iterTrid == hitTracks.end() ){
          hitTracks.push_back( (*hit)[j].trid );
          newhit->push_back((*hit)[j]);
          newrate = rate/scaleRate;
          if(rate == 0) newrate = 1/scaleRate;
          nHits++;
          std::vector<Int_t>::iterator iterDet = std::find(detectorNumbers.begin(), detectorNumbers.end(), (*hit)[j].det );
          if( iterDet != detectorNumbers.end() ){
            /*
            cout << endl<< "(" << i << ") Examining hit " << j << "/" << hit->size() << endl;
            cout << "   Hit: " << j << "<<<<<<<<<<<<<<<<<<"   << endl
                 << "   pid: " << (*hit)[j].pid  << endl
                 << "  trid: " << (*hit)[j].trid << endl
                 << " mtrid: " << (*hit)[j].mtrid<< endl
                 << " z-pos: " << (*hit)[j].z    << endl
                 << " hit.p: " << (*hit)[j].p    << " MeV"<< endl
                 << "hitdet: " << (*hit)[j].det  << endl;
            */
            //cout << "(" << i << ") Added track " << (*hit)[j].trid << " to < hitTracks >" << endl << endl;
            /*cout << "New size of < hitTracks > is: " << hitTracks.size() << endl;
            for(Int_t k=0; k<hitTracks.size(); k++){
              if(k==0 && hitTracks.size()==1){
                cout << "{" << hitTracks[k] << "}" << endl;
              }else if(k==0 ){
                cout << "{" << hitTracks[k];
              }else if(k==(hitTracks.size()-1)){
                cout << "," << hitTracks[k] << "}" << endl;
              }else{
                cout << "," << hitTracks[k];
              }
            }*/
            //cout << "!!!!!!!!!! RECORDED HIT TRACK " << (*hit)[j].trid << ":)" << endl;
            //cout << "Index for " << (*hit)[j].det << " in vector<Int_t> detectorNumbers is "
            //     << std::distance( detectorNumbers.begin(), iterDet ) << endl;
            Int_t index = std::distance( detectorNumbers.begin() , iterDet ); //create stack object and use 'index'
            outputFiles[index]->cd();
            outputTrees[index]->Fill();
            //cout << outputTrees[index]->GetName() << " now has " << outputTrees[index]->GetEntries() << " entries." << endl;
            detectorHitN[index][ (*hit)[j].mtrid ]++;
            newhit->clear();
            newrate = 0;
          }
        }
      }
    }
  }
  ifile->Close();
  delete ifile;

  return nHits;
}
