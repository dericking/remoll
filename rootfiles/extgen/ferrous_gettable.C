void ferrous_gettable(int parallel =0, int Nevents=1e5){

  TString magfieldext;
  if(parallel==0){
    magfieldext="V2";
  }
  if(parallel==1){
    magfieldext="V2parallel";
  }

  const int Ndet=4;
  int DetNo[Ndet]={9001,9010,9020,9030};


  TCanvas *c0 = new TCanvas("c0","c0",0,0,500,500);
  double Ne1[Ndet];
  double Echarge[7][Ndet];
  double Ncharge[7][Ndet];
  double Eph[7][Ndet];
  double Nph[7][Ndet];
  double Eall[7][Ndet];
  double Nall[7][Ndet];
  double En[7][Ndet];
  double Nn[7][Ndet];
  double Ep[7][Ndet];
  double Np[7][Ndet];
  double Epip[7][Ndet];
  double Npip[7][Ndet];
  double Emu[7][Ndet];
  double Nmu[7][Ndet];
  double Eve[7][Ndet];
  double Nve[7][Ndet];
  double pidother[7][Ndet];
  double Nother[7][Ndet];


  TString Ecut[7]={"hit.e<1&&hit.r>500&&hit.r<1300",
		  "hit.e>1&&hit.e<10&&hit.r>500&&hit.r<1300",
		  "hit.e>10&&hit.e<100&&hit.r>500&&hit.r<1300",
		  "hit.e>100&&hit.e<1000&&hit.r>500&&hit.r<1300",
		  "hit.e>1000&&hit.e<10000&&hit.r>500&&hit.r<1300",
		  "hit.e>10000&&hit.r>500&&hit.r<1300",
		  "1&&hit.r>500&&hit.r<1300"  };
  TH1D* h;
  TFile* file;
  TTree* tree;

  //count primary e's used
  for(int jj=0;jj<Ndet;jj++){
    file = TFile::Open(Form("o_ferrous_extgen_%d_%s_100k.root",DetNo[jj],magfieldext.Data()));

    //this isn't working
    //  tree = (TTree*)file->Get("T");
    // tree->Draw(Form("hit.e>>he1_%d",jj),Form("hit.trid==1&&hit.t<1e-5"));
    // h=(TH1D*)file->FindObject(Form("he1_%d",jj));
    Ne1[jj]=Nevents;//=h->GetEntries();
  }

  //  cout<<"e- and e+"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  file = TFile::Open(Form("./o_ferrous_extgen_%d_%s_100k.root",DetNo[jj],magfieldext.Data()));

  tree = (TTree*)file->Get("T");
  //  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
        tree->Draw(Form("hit.e>>hall_%d_%d",ii,jj),Form("hit.det==28&&%s",Ecut[ii].Data()));
        h=(TH1D*)file->FindObject(Form("hall_%d_%d",ii,jj));
        Nall[ii][jj]=h->GetEntries();
        Eall[ii][jj]=h->GetMean();

        tree->Draw(Form("hit.e>>hcharge_%d_%d",ii,jj),Form("hit.det==28&&(hit.pid==11||hit.pid==-11)&&%s",Ecut[ii].Data()));
        h=(TH1D*)file->FindObject(Form("hcharge_%d_%d",ii,jj));
        Ncharge[ii][jj]=h->GetEntries();
        Echarge[ii][jj]=h->GetMean();
	//    cout<<Ncharge[ii][jj]<<" ";

    tree->Draw(Form("hit.e>>hph_%d_%d",ii,jj),Form("hit.det==28&&(hit.pid==22)&&%s",Ecut[ii].Data()));
    h=(TH1D*)file->FindObject(Form("hph_%d_%d",ii,jj));
    Nph[ii][jj]=h->GetEntries();
    Eph[ii][jj]=h->GetMean();


    tree->Draw(Form("hit.e>>hn_%d_%d",ii,jj),Form("hit.det==28&&(hit.pid==2112)&&%s",Ecut[ii].Data()));
    h=(TH1D*)file->FindObject(Form("hn_%d_%d",ii,jj));
    Nn[ii][jj]=h->GetEntries();
    En[ii][jj]=h->GetMean();

    tree->Draw(Form("hit.e>>hp_%d_%d",ii,jj),Form("hit.det==28&&(hit.pid==2212)&&%s",Ecut[ii].Data()));
    h=(TH1D*)file->FindObject(Form("hp_%d_%d",ii,jj));
    Np[ii][jj]=h->GetEntries();
    Ep[ii][jj]=h->GetMean();

    tree->Draw(Form("hit.e>>hpip_%d_%d",ii,jj),Form("hit.det==28&&(hit.pid==211||hit.pid==-211)&&%s",Ecut[ii].Data()));
    h=(TH1D*)file->FindObject(Form("hpip_%d_%d",ii,jj));
    Npip[ii][jj]=h->GetEntries();
    Epip[ii][jj]=h->GetMean();

    tree->Draw(Form("hit.e>>hmu_%d_%d",ii,jj),Form("hit.det==28&&(hit.pid==13||hit.pid==-13)&&%s",Ecut[ii].Data()));
    h=(TH1D*)file->FindObject(Form("hmu_%d_%d",ii,jj));
    Nmu[ii][jj]=h->GetEntries();
    Emu[ii][jj]=h->GetMean();


    tree->Draw(Form("hit.e>>hve_%d_%d",ii,jj),Form("hit.det==28&&(hit.pid==12||hit.pid==-12||hit.pid==14||hit.pid==-14||hit.pid==16||hit.pid==-16)&&%s",Ecut[ii].Data()));
    h=(TH1D*)file->FindObject(Form("hve_%d_%d",ii,jj));
    Nve[ii][jj]=h->GetEntries();
    Eve[ii][jj]=h->GetMean();

    tree->Draw(Form("hit.pid>>hother_%d_%d",ii,jj),Form("hit.det==28&&!(hit.pid==12||hit.pid==-12||hit.pid==14||hit.pid==-14||hit.pid==11||hit.pid==-11||hit.pid==22||hit.pid==211||hit.pid==-211||hit.pid==2112||hit.pid==2212||hit.pid==13||hit.pid==-13)&&%s",Ecut[ii].Data()));
    h=(TH1D*)file->FindObject(Form("hother_%d_%d",ii,jj));
    Nother[ii][jj]=h->GetEntries();
    pidother[ii][jj]=h->GetMean();

  }
  //  cout<<endl;
      cout<<DetNo[jj]<<endl;
      tree->Scan("hit.pid",Form("hit.det==28&&!(hit.pid==12||hit.pid==-12||hit.pid==14||hit.pid==-14||hit.pid==11||hit.pid==-11||hit.pid==22||hit.pid==211||hit.pid==-211||hit.pid==2212||hit.pid==2112||hit.pid==13||hit.pid==-13)"));

  }

  cout<<"For fields: "<<magfieldext.Data()<<endl;

  cout<<endl;
  cout<<"all"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  cout<<DetNo[jj]<<" ";
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nall[ii][jj]<<" ";
  }
  cout<<endl;
  }


  cout<<endl;
  cout<<"e- and e+"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  cout<<DetNo[jj]<<" ";
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Ncharge[ii][jj]<<" ";
  }
  cout<<endl;
  }


  cout<<endl;
  cout<<"photons"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  cout<<DetNo[jj]<<" ";
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nph[ii][jj]<<" ";
  }
  cout<<endl;
  }

  cout<<endl;
  cout<<"neutrons"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  cout<<DetNo[jj]<<" ";
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nn[ii][jj]<<" ";
  }
  cout<<endl;
  }

  cout<<endl;
  cout<<"protons"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  cout<<DetNo[jj]<<" ";
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Np[ii][jj]<<" ";
  }
  cout<<endl;
  }


  cout<<endl;
  cout<<"neutrinos and antineutrinos ve vu vtau"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  cout<<DetNo[jj]<<" ";
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nve[ii][jj]<<" ";
  }
  cout<<endl;
  }


  cout<<endl;
  cout<<"pion pi+- +-211"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  cout<<DetNo[jj]<<" ";
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Npip[ii][jj]<<" ";
  }
  cout<<endl;
  }

  cout<<endl;
  cout<<"pion mu+- +-13"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  cout<<DetNo[jj]<<" ";
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nmu[ii][jj]<<" ";
  }
  cout<<endl;
  }


  cout<<endl;
  cout<<"others"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  cout<<DetNo[jj]<<" ";
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nother[ii][jj]<<" ";
  }
  cout<<endl;
  }

  cout<<endl;
  cout<<"pid others"<<endl;
  for(int jj=0;jj<Ndet;jj++){
  cout<<DetNo[jj]<<" ";
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<pidother[ii][jj]<<" ";
  }
  cout<<endl;
  }

}
