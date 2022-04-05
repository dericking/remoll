void gettable(int domore =0){

  TCanvas *c0 = new TCanvas("c0","c0",0,0,500,500);
  double Ne1[7];
  double Echarge[7][7];
  double Ncharge[7][7];
  double Eph[7][7];
  double Nph[7][7];
  double Eall[7][7];
  double Nall[7][7];
  double En[7][7];
  double Nn[7][7];
  double Ep[7][7];
  double Np[7][7];
  double Epip[7][7];
  double Npip[7][7];
  double Eve[7][7];
  double Nve[7][7];
  double pidother[7][7];
  double Nother[7][7];


  TString Ecut[7]={"hit.e<1",
		  "hit.e>1&&hit.e<10",
		  "hit.e>10&&hit.e<100",
		  "hit.e>100&&hit.e<1000",
		  "hit.e>1000&&hit.e<10000",
		  "hit.e>10000",
		  "1"  };
  TH1D* h;
  TFile* file;
  TTree* tree;

  //count primary e's used
  for(int jj=0;jj<7;jj++){
    //  file = TFile::Open(Form("./remollout%d.root",70+jj));
    if(domore==0){
      file = TFile::Open(Form("./remollout%d.root",70+jj));
    }
    if(domore==1){
      if(jj==0){
	file = TFile::Open(Form("./remollout%d_1M.root",70+jj));
      }
      if(jj!=0){
	file = TFile::Open(Form("./remollout%d_100k.root",70+jj));
      }
    }
  tree = (TTree*)file->Get("T");
  tree->Draw(Form("hit.e>>he1_%d",jj),Form("hit.trid==1&&hit.t<1e-5"));
    h=(TH1D*)file->FindObject(Form("he1_%d",jj));
    Ne1[jj]=h->GetEntries();
    //    cout<<Ne1[jj]<<" ";
    //  cout<<endl;
  }


  //  cout<<"e- and e+"<<endl;
  for(int jj=0;jj<7;jj++){
    if(domore==0){
      file = TFile::Open(Form("./remollout%d.root",70+jj));
    }
    if(domore==1){
      if(jj==0){
	file = TFile::Open(Form("./remollout%d_1M.root",70+jj));
      }
      if(jj!=0){
	file = TFile::Open(Form("./remollout%d_100k.root",70+jj));
      }
    }
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

    tree->Draw(Form("hit.e>>hpip_%d_%d",ii,jj),Form("hit.det==28&&(hit.pid==211)&&%s",Ecut[ii].Data()));
    h=(TH1D*)file->FindObject(Form("hpip_%d_%d",ii,jj));
    Npip[ii][jj]=h->GetEntries();
    Epip[ii][jj]=h->GetMean();


    tree->Draw(Form("hit.e>>hve_%d_%d",ii,jj),Form("hit.det==28&&(hit.pid==12||hit.pid==-12||hit.pid==14||hit.pid==-14)&&%s",Ecut[ii].Data()));
    h=(TH1D*)file->FindObject(Form("hve_%d_%d",ii,jj));
    Nve[ii][jj]=h->GetEntries();
    Eve[ii][jj]=h->GetMean();

    tree->Draw(Form("hit.pid>>hother_%d_%d",ii,jj),Form("hit.det==28&&!(hit.pid==12||hit.pid==-12||hit.pid==14||hit.pid==-14||hit.pid==11||hit.pid==-11||hit.pid==22||hit.pid==211||hit.pid==2112||hit.pid==2212)&&%s",Ecut[ii].Data()));
    h=(TH1D*)file->FindObject(Form("hother_%d_%d",ii,jj));
    Nother[ii][jj]=h->GetEntries();
    pidother[ii][jj]=h->GetMean();

  }
  //  cout<<endl;
      cout<<70+jj<<endl;
      tree->Scan("hit.pid",Form("hit.det==28&&!(hit.pid==12||hit.pid==-12||hit.pid==14||hit.pid==-14||hit.pid==11||hit.pid==-11||hit.pid==22||hit.pid==211||hit.pid==2212||hit.pid==2112)"));

  }

  cout<<endl;
  cout<<"all"<<endl;
  for(int jj=0;jj<7;jj++){
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nall[ii][jj]<<" ";
  }
  cout<<endl;
  }


  cout<<endl;
  cout<<"e- and e+"<<endl;
  for(int jj=0;jj<7;jj++){
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Ncharge[ii][jj]<<" ";
  }
  cout<<endl;
  }


  cout<<endl;
  cout<<"photons"<<endl;
  for(int jj=0;jj<7;jj++){
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nph[ii][jj]<<" ";
  }
  cout<<endl;
  }

  cout<<endl;
  cout<<"neutrons"<<endl;
  for(int jj=0;jj<7;jj++){
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nn[ii][jj]<<" ";
  }
  cout<<endl;
  }

  cout<<endl;
  cout<<"protons"<<endl;
  for(int jj=0;jj<7;jj++){
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Np[ii][jj]<<" ";
  }
  cout<<endl;
  }


  cout<<endl;
  cout<<"neutrinos and antineutrinos ve vu "<<endl;
  for(int jj=0;jj<7;jj++){
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nve[ii][jj]<<" ";
  }
  cout<<endl;
  }


  cout<<endl;
  cout<<"pion pi+ 211"<<endl;
  for(int jj=0;jj<7;jj++){
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Npip[ii][jj]<<" ";
  }
  cout<<endl;
  }


  cout<<endl;
  cout<<"others"<<endl;
  for(int jj=0;jj<7;jj++){
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<Nother[ii][jj]<<" ";
  }
  cout<<endl;
  }

  cout<<endl;
  cout<<"pid others"<<endl;
  for(int jj=0;jj<7;jj++){
  cout<<Ne1[jj]<<" ";
  for(int ii=0;ii<7;ii++){
    cout<<pidother[ii][jj]<<" ";
  }
  cout<<endl;
  }

}
