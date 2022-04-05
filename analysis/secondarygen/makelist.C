void makelist(TString maindir="/volatile/halla/moller12gev/ericking/", TString subdir="remoll_ferrous_pivot_production_test__2022_03_04", int NkEv = 100, int Nfiles=1000, TString outfilename = "mylist.txt"){

  ofstream outfile;
  outfile.open(outfilename);

  int Nstart=1;
  for(int ii=Nstart; ii<Nfiles+Nstart;ii++){
    if(ii<10){
      outfile << maindir.Data() << subdir.Data() <<"/"<<subdir.Data()<<"_"<<NkEv<<"kEv_"<<"00"<<ii<<"/remollout.root"<<endl;
    }
    if(ii>9&&ii<100){
      outfile << maindir.Data() << subdir.Data()<<"/"<<subdir.Data()<<"_"<<NkEv<<"kEv_"<<"0"<<ii<<"/remollout.root"<<endl;
    }
    if(ii>99&&ii<1000){
      outfile << maindir.Data() << subdir.Data()<<"/"<<subdir.Data()<<"_"<<NkEv<<"kEv_"<<""<<ii<<"/remollout.root"<<endl;
    }
    if(ii>999&&ii<10000){
      outfile << maindir.Data() << subdir.Data()<<"/"<<subdir.Data()<<"_"<<NkEv<<"kEv_"<<""<<ii<<"/remollout.root"<<endl;
    }
  }

}
