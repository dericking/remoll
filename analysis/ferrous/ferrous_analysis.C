///////////////////////////////////////////////////////////////
//  ferrous_analysis::Loop(Int_t par, Int_t prim)
//  par: parallel maps used? 0 or 1
//  prim: total number of events in the primary simulation
//  detlist: detector list string
///////////////////////////////////////////////////////////////

//#define ferrous_analysis_cxx
#include"ferrous_analysis.h"
#include<TH2.h>
#include<TStyle.h>
#include<TCanvas.h>
#include<TSystem.h>
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
using namespace std;

Bool_t firstPDFpage(true);

void ferrous_analysis::Loop(Int_t par, Int_t prim, string DetNums, string primHits)
{
  const Int_t primSimEvents = prim;
  const Int_t parallel = par;

  Double_t detRmin = 500.;
  Double_t detRmax = 1300.;

  Double_t ambientB= 1.; // a 1G ambient/stray field strength.

  const Int_t Nset = 7; //six energy divisions + all

  //General for cycling through files.
  TFile *fThis;
  TTree *t;
  Long64_t nentries,nbytes,nb;

  //Plotting details
  Int_t   lineColors[Nset] = {kRed,kBlue,kGreen,kMagenta,kCyan,kOrange-3,kBlack};
  TString energyText[Nset] = {"(KE < 1MeV)","(1MeV < KE < 10MeV)","(10MeV < KE < 100MeV)","(100MeV < KE < 1GeV)","(1GeV < KE < 10GeV)","(10GeV < KE)","(ALL KE)"};

  //Text output file
  std::ofstream fout;
  fout.open("ferrous_analysis_results.txt");

  //THIS WILL BE USED FOR THE ANALYSIS ON THE SERVER.  HERE I'LL JUST REFER TO ONE ABSOLUTE FILE PATH

  ///////////////////////////////////////////////////////////////  (╯°□°）╯︵ ┻━┻
  //CHECK FOR THE EXISTENCE OF ENVIRONMENTAL VARIABLES
  if(!gSystem->Getenv("REMOLLINSTALL")){ cout <<"Grab environment variable REMOLLINSTALL\n" << endl; exit(1); }

  TString sRemollInstall = gSystem->Getenv("REMOLLINSTALL");
  TString sUSFieldMapLocation;
  TString sDSFieldMapLocation;
  TString magfieldext;

  if(parallel==0){
    magfieldext="V2";
    sUSFieldMapLocation = sRemollInstall + "/map_directory/V2U.1a.50cm.txt";
    sDSFieldMapLocation = sRemollInstall + "/map_directory/V2DSg.9.75cm.txt";
  }
  if(parallel==1){
    magfieldext="V2parallel";
    sUSFieldMapLocation = sRemollInstall + "/map_directory/V2U.1a.50cm.parallel.txt";
    sDSFieldMapLocation = sRemollInstall + "/map_directory/V2DSg.9.75cm.parallel.txt";
  }

  cout << "Field Map Locations:" << endl
       << "\tUS:" << sUSFieldMapLocation << endl
       << "\tDS:" << sDSFieldMapLocation << endl << endl;

  remollMagneticField( sDSFieldMapLocation );
  

  // USED FOR ANALYSIS ON TEMPLE WORKSTATION
  //TString magfieldext;
  //if(parallel==0){ magfieldext="V2"; }
  //if(parallel==1){ magfieldext="V2parallel"; }
  //TString sUSFieldMapLocation = "/home/tup89084/Remoll/CarynRemoll/remoll/map_directory/V2U.1a.50cm.txt";
  //TString sDSFieldMapLocation = "/home/tup89084/Remoll/CarynRemoll/remoll/map_directory/V2DSg.9.75cm.txt";
  //remollMagneticField( sDSFieldMapLocation );


  ///////////////////////////////////////////////////////////////  (╯°□°）╯︵ ┻━┻
  //PUSH RECORDED PRIMARY HITS INTO VECTOR
  std::vector<Int_t> PrimHits;
  std::stringstream ssPH(primHits);
  while(ssPH.good()){
    string ss_parse;
    getline(ssPH,ss_parse,',');
    PrimHits.push_back( std::atoi(ss_parse.c_str()) );
  }


  ///////////////////////////////////////////////////////////////  (╯°□°）╯︵ ┻━┻
  //SET DETECTORS
  std::vector<Int_t> DetNo;
  std::stringstream ss(DetNums);
  while(ss.good()){
    string ss_parse;
    getline(ss,ss_parse,',');
    DetNo.push_back( std::atoi(ss_parse.c_str()) );
  }
  const Int_t Ndet = (Int_t)DetNo.size();

  Double_t Nevents[Ndet];
  Double_t Echarge[7][Ndet];
  Double_t Ncharge[7][Ndet];
  Double_t NchargeBWtd[7][Ndet];
  Double_t NchargeBWtd2[7][Ndet];
  Double_t Eph[7][Ndet];
  Double_t Nph[7][Ndet];
  Double_t Eall[7][Ndet];
  Double_t Nall[7][Ndet];
  Double_t En[7][Ndet];
  Double_t Nn[7][Ndet];
  Double_t Ep[7][Ndet];
  Double_t Np[7][Ndet];
  Double_t Epip[7][Ndet];
  Double_t Npip[7][Ndet];
  Double_t Emu[7][Ndet];
  Double_t Nmu[7][Ndet];
  Double_t Eve[7][Ndet];
  Double_t Nve[7][Ndet];
  Double_t pidother[7][Ndet];
  Double_t Nother[7][Ndet];

  for(Int_t d=0; d<Ndet; d++){
    Nevents[d]=0;
    for(Int_t s=0; s<Nset; s++){
      Echarge[s][d]=0; Ncharge[s][d]=0; NchargeBWtd[s][d]=0; NchargeBWtd2[s][d]=0; Eph[s][d]=0; Nph[s][d]=0;
    }
  }



  TString Ecut[7]={"hit.e<1&&hit.r>500&&hit.r<1300",
                    "hit.e>1&&hit.e<10&&hit.r>500&&hit.r<1300",
                    "hit.e>10&&hit.e<100&&hit.r>500&&hit.r<1300",
                    "hit.e>100&&hit.e<1000&&hit.r>500&&hit.r<1300",
                    "hit.e>1000&&hit.e<10000&&hit.r>500&&hit.r<1300",
                    "hit.e>10000&&hit.r>500&&hit.r<1300",
                    "1&&hit.r>500&&hit.r<1300"
                   };

  for(Int_t jj=0; jj<Ndet; jj++){
    TString fname = "o_ferrous_extgen_" + to_string(DetNo[jj]) + "_" + magfieldext.Data() + ".root";// "_100k.root";
    const char * filename = fname.Data();

    TH1F * h1Ee[Nset];
    TH1F * h1Re[Nset];
    TH1F * h1Be[Nset];
    TH1F * h1NeWtd[Nset]; //Weights Bfield recordings, later take integral
    TH1F * h1Eg[Nset];
    TH1F * h1Rg[Nset];
    TH1F * h1Bg[Nset];
    //TH2F * h2BRe = new TH2F( Form("h2BRe_%i",DetNo[jj]),Form("e-e+ DetNo=%i Field.Radius (ALL); BField.Mag (For now) [log10(B.Mag/Gauss)];Radius from Beamline [mm]",DetNo[jj]),100,-2,4,100,0,700 );
    //TH2F * h2BRg = new TH2F( Form("h2BRg_%i",DetNo[jj]),Form("gammas DetNo=%i Field.Radius (ALL); BField.Mag (For now) [log10(B.Mag/Gauss)];Radius from Beamline [mm]",DetNo[jj]),100,-2,4,100,0,700 );
    TH2F * h2BRe = new TH2F( Form("h2BRe_%i",DetNo[jj]),Form("e-e+ DetNo=%i Field.Radius (ALL); BField.Mag (For now) [log10(B.Mag/Gauss)];Radius from Beamline [mm]",DetNo[jj]),2000,0,2000,100,0,700 );
    TH2F * h2BRg = new TH2F( Form("h2BRg_%i",DetNo[jj]),Form("gammas DetNo=%i Field.Radius (ALL); BField.Mag (For now) [log10(B.Mag/Gauss)];Radius from Beamline [mm]",DetNo[jj]),2000,0,2000,100,0,700 );
    for(Int_t i=0; i<Nset; i++){
      h1Ee[i]    = new TH1F( Form("hEe%i",i), Form("e-e+ DetNo=%i hit.e %s",DetNo[jj],energyText[i].Data()),60,-1,5);
      h1NeWtd[i] = new TH1F( Form("hNeBWtd%i",i), Form("#vec{B} Wtd e-e+ DetNo=%i hit.e %s",DetNo[jj],energyText[i].Data()),60,-1,5);
      h1Re[i]    = new TH1F( Form("hRe%i",i), Form("e-e+ DetNo=%i hit.r %s",DetNo[jj],energyText[i].Data()),80,detRmin,detRmax);
      //h1Be[i]   = new TH1F( Form("hBe%i",i), Form("e-e+ DetNo=%i ExtGenVertex.B.mag %s",DetNo[jj],energyText[i].Data()),80,-2,4);
      h1Be[i]    = new TH1F( Form("hBe%i",i), Form("e-e+ DetNo=%i ExtGenVertex.B.mag %s",DetNo[jj],energyText[i].Data()),2000,0,2000);
      h1Eg[i]    = new TH1F( Form("hEg%i",i), Form("gammas DetNo=%i hit.e %s",DetNo[jj],energyText[i].Data()),60,-1,5);
      h1Rg[i]    = new TH1F( Form("hRg%i",i), Form("gammas DetNo=%i hit.r %s",DetNo[jj],energyText[i].Data()),80,detRmin,detRmax);
      //h1Bg[i]   = new TH1F( Form("hBg%i",i), Form("gammas DetNo=%i ExtGenVertex.B.mag %s",DetNo[jj],energyText[i].Data()),80,-2,4);
      h1Bg[i]    = new TH1F( Form("hBg%i",i), Form("gammas DetNo=%i ExtGenVertex.B.mag %s",DetNo[jj],energyText[i].Data()),2000,0,2000);
    }

    fThis = new TFile(filename);
    t = 0;
    fThis->GetObject("T",t);
    Init(t);

    nentries = fChain->GetEntriesFast();
    nbytes   = 0;
    nb       = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      Nevents[jj] = nentries; //WILL NEED TO MODIFY THIS FOR LOOP OVER DETECTORS

      //if(hit_ > 0) cout << "(" << jentry << ")" << "number of hits: " << hit_ << endl;

      //initialize point to be inspected - must convert to m so divide by m2mm (=1000)
      Double_t point[4]={part_vx[0]/m2mm,part_vy[0]/m2mm,part_vz[0]/m2mm,0.0};
      //Initialize field object
      Double_t field[3]={0.0,0.0,0.0};
      GetFieldValue(point,field);
      //Convert these to gauss and add 'ambient' field for points outside maps.
      for(Int_t i = 0; i < 3; i++){
        field[i] *= 10000.;
        field[i] += ambientB;
      }
      Double_t vertexRad= sqrt( part_vx[0]*part_vx[0]+part_vy[0]*part_vy[0] );
      Double_t fieldMag = sqrt( field[0]*field[0]+field[1]*field[1]+field[2]*field[2] );
      //cout << std::setprecision(1) << std::fixed << std::setprecision(10) << "vertex (x,y,z)[mm]: "
      //     << point[0]*m2mm << "," << point[1]*m2mm << "," << point[2]*m2mm << endl;
      //cout << std::fixed << std::setprecision(10) << ">> Magnetic Field @ Vertex [G]: |("
      //     << field[0] << "," << field[1] << "," << field[2] << ")| = "
      //     << sqrt(field[0]*field[0]+field[1]*field[1]+field[2]*field[2]) << endl;

      // if (Cut(ientry) < 0) continue;
      for(Int_t n=0; n<hit_; n++){
        if( (hit_pid[n]==11 || hit_pid[n]==-11) && hit_r[n] > detRmin && hit_r[n] < detRmax && hit_det[n] == 28){
          if(hit_e[n] < 1.){
            h1Ee[0]->Fill( log10(hit_e[n]) );
            h1Re[0]->Fill( hit_r[n] );
            h1Be[0]->Fill( (fieldMag) );
            h1NeWtd[0]->Fill( 1 , fieldMag );
            NchargeBWtd2[0][jj] += fieldMag;
          }
          if(hit_e[n] >= 1.    && hit_e[n] < 10.   ){
            h1Ee[1]->Fill( log10(hit_e[n]) );
            h1Re[1]->Fill( hit_r[n] );
            h1Be[1]->Fill( (fieldMag) );
            h1NeWtd[1]->Fill( 1 , fieldMag );
            NchargeBWtd2[1][jj] += fieldMag;
          }
          if(hit_e[n] >= 10.   && hit_e[n] < 100.  ){
            h1Ee[2]->Fill( log10(hit_e[n]) );
            h1Re[2]->Fill( hit_r[n] );
            h1Be[2]->Fill( (fieldMag) );
            h1NeWtd[2]->Fill( 1 , fieldMag );
            NchargeBWtd2[2][jj] += fieldMag;
          }
          if(hit_e[n] >= 100.  && hit_e[n] < 1000. ){
            h1Ee[3]->Fill( log10(hit_e[n]) );
            h1Re[3]->Fill( hit_r[n] );
            h1Be[3]->Fill( (fieldMag) );
            h1NeWtd[3]->Fill( 1 , fieldMag );
            NchargeBWtd2[3][jj] += fieldMag;
          }
          if(hit_e[n] >= 1000. && hit_e[n] < 10000.){
            h1Ee[4]->Fill( log10(hit_e[n]) );
            h1Re[4]->Fill( hit_r[n] );
            h1Be[4]->Fill( (fieldMag) );
            h1NeWtd[4]->Fill( 1 , fieldMag );
            NchargeBWtd2[4][jj] += fieldMag;
          }
          if(hit_e[n] >= 10000.){
            h1Ee[5]->Fill( log10(hit_e[n]) );
            h1Re[5]->Fill( hit_r[n] );
            h1Be[5]->Fill( (fieldMag) );
            h1NeWtd[5]->Fill( 1 , fieldMag );
            NchargeBWtd2[5][jj] += fieldMag;
          }
          h1Ee[6]->Fill( log10(hit_e[n]) ); //Seventh histogram is all hits.
          h1Re[6]->Fill( hit_r[n] );
          h1Be[6]->Fill( (fieldMag) );
          h2BRe->Fill( (fieldMag) , vertexRad );
          h1NeWtd[6]->Fill( 1 , fieldMag );
          NchargeBWtd2[6][jj] += fieldMag;
        }//end if(part==11)
        if( hit_pid[n]==22 && hit_r[n] > detRmin && hit_r[n] < detRmax && hit_det[n] == 28){
          if(hit_e[n] < 1.){
            h1Eg[0]->Fill( log10(hit_e[n]) );
            h1Rg[0]->Fill( hit_r[n] );
            h1Bg[0]->Fill( (fieldMag) );
          }
          if(hit_e[n] >= 1.    && hit_e[n] < 10.   ){
            h1Eg[1]->Fill( log10(hit_e[n]) );
            h1Rg[1]->Fill( hit_r[n] );
            h1Bg[1]->Fill( (fieldMag) );
          }
          if(hit_e[n] >= 10.   && hit_e[n] < 100.  ){
            h1Eg[2]->Fill( log10(hit_e[n]) );
            h1Rg[2]->Fill( hit_r[n] );
            h1Bg[2]->Fill( (fieldMag) );
          }
          if(hit_e[n] >= 100.  && hit_e[n] < 1000. ){
            h1Eg[3]->Fill( log10(hit_e[n]) );
            h1Rg[3]->Fill( hit_r[n] );
            h1Bg[3]->Fill( (fieldMag) );
          }
          if(hit_e[n] >= 1000. && hit_e[n] < 10000.){
            h1Eg[4]->Fill( log10(hit_e[n]) );
            h1Rg[4]->Fill( hit_r[n] );
            h1Bg[4]->Fill( (fieldMag) );
          }
          if(hit_e[n] >= 10000.){
            h1Eg[5]->Fill( log10(hit_e[n]) );
            h1Rg[5]->Fill( hit_r[n] );
            h1Bg[5]->Fill( (fieldMag) );
          }
          h1Eg[6]->Fill( log10(hit_e[n]) ); //Seventh histogram is all hits.
          h1Rg[6]->Fill( hit_r[n] );
          h1Bg[6]->Fill( (fieldMag) );
          h2BRg->Fill( (fieldMag) , vertexRad );
        }//end if(part==22)

      }//end for(hit_)

    }//end for(nentries)

    for(Int_t s = 0; s < Nset; s++){
      Ncharge[s][jj]    = h1Ee[s]->GetEntries();
      NchargeBWtd[s][jj]= h1NeWtd[s]->Integral();
      Nph[s][jj]        = h1Eg[s]->GetEntries();
    }

    TCanvas * CRe = new TCanvas(Form("CRe_%i",DetNo[jj]),"CRe",800,464);
    CRe->SetGrid();
    for(Int_t k=Nset-1; k>=0; k--){
      h1Re[k]->SetLineColor( lineColors[k] );
      h1Re[k]->SetLineWidth( 2 );
      h1Re[k]->Draw("SAME");
    }
    CRe->BuildLegend();

    TCanvas * CEe = new TCanvas(Form("CEe_%i",DetNo[jj]),"CEe",800,464);
    CEe->SetGrid();
    CEe->SetLogy();
    for(Int_t k=Nset-1; k>=0; k--){
      h1Ee[k]->SetLineColor( lineColors[k] );
      h1Ee[k]->SetLineWidth( 2 );
      h1Ee[k]->Draw("SAME");
    }
    CEe->BuildLegend();

    TCanvas * CBe = new TCanvas(Form("CBe_%i",DetNo[jj]),"CBe",800,464);
    CBe->SetGrid();
    CBe->SetLogy();
    for(Int_t k=Nset-1; k>=0; k--){
      h1Be[k]->SetLineColor( lineColors[k] );
      h1Be[k]->SetLineWidth( 2 );
      h1Be[k]->Draw("SAME");
    }
    CBe->BuildLegend();

    TCanvas * C2BRe = new TCanvas(Form("C2BRe_%i",DetNo[jj]),"C2BRe",800,464);
    h2BRe->SetStats(211);
    h2BRe->Draw();

    TCanvas * C2BRg = new TCanvas(Form("C2BRg_%i",DetNo[jj]),"C2BRg",800,464);
    h2BRg->SetStats(211);
    h2BRg->Draw();

    CRe->SaveAs( Form("CRe_DetNo_%i.png",DetNo[jj]) );
    CEe->SaveAs( Form("CEe_DetNo_%i.png",DetNo[jj]) );
    CBe->SaveAs( Form("CBe_DetNo_%i.png",DetNo[jj]) );
    C2BRe->SaveAs( Form("C2BRe_DetNo_%i.png",DetNo[jj]) );

  }

  ////////////////// PRINT EVENTS
  fout << ">>> Parallel: " << parallel << endl;
  fout << ">>> Primary Sim Events: " << primSimEvents << endl;

  fout << endl;
  fout << "'Charges(e+e-) [Count]'" << endl;
  fout <<  " DetNo "   << "  ExtGenN" << "   <1MeV" << "    1-10M" << "  10-100M"
       << "    .1-1G" << "    1-10G" << "   >10GeV" << "    TOTAL" << endl;
  for(int jj = 0; jj < Ndet; jj++){
    fout << defaultfloat << std::setw(6) << DetNo[jj] << " ";
    fout << std::setw(8) << Nevents[jj] << " ";
    for(int ii = 0; ii < Nset; ii++){
      fout << std::setw(8) << Ncharge[ii][jj] <<" ";
    }
    fout << endl;
  }

  fout << endl;
  fout << "'BField-Wtd Charges(e+e-) [Count]'" << endl;
  fout <<  " DetNo "   << "  ExtGenN" << "   <1MeV" << "    1-10M" << "  10-100M"
       << "    .1-1G" << "    1-10G" << "   >10GeV" << "    TOTAL" << endl;
  for(int jj = 0; jj < Ndet; jj++){
    fout << defaultfloat << std::setw(6) << DetNo[jj] << " ";
    fout << std::setw(8) << Nevents[jj] << " ";
    for(int ii = 0; ii < Nset; ii++){
      fout << std::setw(8) << NchargeBWtd[ii][jj] <<" ";
    }
    fout << endl;
  }

  fout << endl;
  fout << "'(2) BField-Wtd Charges(e+e-) [Count]'" << endl;
  fout <<  " DetNo "   << "  ExtGenN" << "   <1MeV" << "    1-10M" << "  10-100M"
       << "    .1-1G" << "    1-10G" << "   >10GeV" << "    TOTAL" << endl;
  for(int jj = 0; jj < Ndet; jj++){
    fout << defaultfloat << std::setw(6) << DetNo[jj] << " ";
    fout << std::setw(8) << Nevents[jj] << " ";
    for(int ii = 0; ii < Nset; ii++){
      fout << std::setw(8) << NchargeBWtd2[ii][jj] <<" ";
    }
    fout << endl;
  }

  fout << endl;
  fout << "Gammas [Count]" << endl;
  fout <<  " DetNo "   << "  ExtGenN" << "   <1MeV" << "    1-10M" << "  10-100M"
       << "    .1-1G" << "    1-10G" << "   >10GeV" << "    TOTAL" << endl;
  for(int jj = 0; jj < Ndet; jj++){
    fout << defaultfloat << std::setw(6) << DetNo[jj] << " ";
    fout << std::setw(8) << Nevents[jj] << " ";
    for(int ii = 0; ii < Nset; ii++){
      fout << std::setw(8) << Nph[ii][jj] <<" ";
    }
    fout << endl;
  }

  fout << endl;
  fout << "Charges(e+e-) [Normalized by Primary*ExtendedGen]" << endl;
  fout <<  " DetNo "   << " Prim*Ext" << "   <1MeV" << "    1-10M" << "  10-100M"
       << "    .1-1G" << "    1-10G" << "   >10GeV" << "    TOTAL" << endl;
  for(int jj = 0; jj < Ndet; jj++){
    fout << std::defaultfloat << std::fixed << std::setprecision(0) << std::setw(6) << (Double_t)DetNo[jj] << " ";
    fout << std::scientific << std::setprecision(1) << std::setw(8) << Nevents[jj]*primSimEvents << " ";
    for(int ii = 0; ii < Nset; ii++){
      fout << std::scientific << std::setprecision(1) << std::setw(8) << Ncharge[ii][jj]/(Double_t)Nevents[jj]/(Double_t)primSimEvents <<" ";
    }
    fout << endl;
  }

  fout << endl;
  fout << "BField Weighted Charges(e+e-) [Normalized by Primary*ExtendedGen]" << endl;
  fout <<  " DetNo "   << " Prim*Ext" << "   <1MeV" << "    1-10M" << "  10-100M"
       << "    .1-1G" << "    1-10G" << "   >10GeV" << "    TOTAL" << endl;
  for(int jj = 0; jj < Ndet; jj++){
    fout << std::defaultfloat << std::fixed << std::setprecision(0) << std::setw(6) << (Double_t)DetNo[jj] << " ";
    fout << std::scientific << std::setprecision(1) << std::setw(8) << Nevents[jj]*primSimEvents << " ";
    for(int ii = 0; ii < Nset; ii++){
      fout << std::scientific << std::setprecision(1) << std::setw(8) << NchargeBWtd2[ii][jj]/(Double_t)Nevents[jj]/(Double_t)primSimEvents <<" ";
    }
    fout << endl;
  }

  fout << endl;
  fout << "BField Weighted Charges(e+e-) [Normalized by PrimHits/Primary*ExtendedGen]" << endl;
  fout <<  " DetNo "   << " Prim*Ext" << "   <1MeV" << "    1-10M" << "  10-100M"
       << "    .1-1G" << "    1-10G" << "   >10GeV" << "    TOTAL" << endl;
  for(int jj = 0; jj < Ndet; jj++){
    fout << std::defaultfloat << std::fixed << std::setprecision(0) << std::setw(6) << (Double_t)DetNo[jj] << " ";
    fout << std::scientific << std::setprecision(1) << std::setw(8) << Nevents[jj]*primSimEvents << " ";
    for(int ii = 0; ii < Nset; ii++){
      fout << std::scientific << std::setprecision(1) << std::setw(8) << NchargeBWtd2[ii][jj]*PrimHits[jj]/(Double_t)Nevents[jj]/(Double_t)primSimEvents <<" ";
    }
    fout << endl;
  }

  fout << endl;
  fout << "Gammas [Normalized by Primary*ExtendedGen]" << endl;
  fout <<  " DetNo "   << " Prim*Ext" << "   <1MeV" << "    1-10M" << "  10-100M"
       << "    .1-1G"  << "    1-10G" << "   >10GeV" << "    TOTAL" << endl;
  for(int jj = 0; jj < Ndet; jj++){
    fout << std::defaultfloat << std::fixed << std::setprecision(0) << std::setw(6) << (Double_t)DetNo[jj] << " ";
    fout << std::scientific << std::setprecision(1) << std::setw(8) << Nevents[jj]*primSimEvents << " ";
    for(int ii = 0; ii < Nset; ii++){
      fout << std::scientific << std::setprecision(1) << std::setw(8) << Nph[ii][jj]/(Double_t)Nevents[jj]/(Double_t)primSimEvents <<" ";
    }
    fout << endl;
  }

  //^^^COULD OUTPUT AS A TPAVETEXT ON A PAGE.

  fout.close();

}//end script
