#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include<TROOT.h>
#include<iostream>
#include<iomanip>
#include<TString.h>
#include<vector>
#include<algorithm>
#include<fstream>
#include<assert.h>
#include<math.h>
#include<istream>
#include<TVector3.h>

// Header file for the classes stored in the TTree if any.
#include "TObject.h"

class ferrous_analysis {

public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxpart = 1;
   static constexpr Int_t kMaxhit = 1000;
   static constexpr Int_t kMaxsum = 1000;

   // Declaration of leaf types
   Int_t           dets_sd_coilDet;
   Int_t           dets_sd_collDet;
   Int_t           dets_sd_planeDet;
   Int_t           dets_sd_CollDet;
   Int_t           dets_sd_PlaneDet;
   Int_t           dets_sd_Hall_Det;
   Int_t           dets_sd_sbsBunkerVacuumDet_1;
   Int_t           dets_sd_Shielding_Block_Local_Lead;
   Int_t           dets_sd_hutDet;
   Int_t           dets_lv_logic_shield4_top_3;
   Int_t           dets_lv_logic_shield1_bot_3;
   Int_t           dets_lv_logicCollar1;
   Int_t           dets_lv_logicDSTracker;
   Int_t           dets_lv_logicCollar2;
   Int_t           dets_lv_PhotonBlocker_log1;
   Int_t           dets_lv_logic_Col4_accep2;
   Int_t           dets_lv_LeadCollar_logic;
   Int_t           dets_lv_TargWindow_logic;
   Int_t           dets_lv_cylHallDet_1_logic;
   Int_t           dets_lv_cylHallDet_2_logic;
   Int_t           dets_lv_cylHallDet_3_logic;
   Int_t           dets_lv_cylHallDet_4_logic;
   Int_t           dets_lv_topwalldet_logic;
   Int_t           dets_lv_cylwalldet_logic;
   Int_t           dets_lv_cylwall_logic;
   Int_t           dets_lv_sbsBunkerVacuumDet1_l;
   Int_t           dets_lv_logicUScoll_1;
   Int_t           dets_lv_col1_h20_logic2;
   Int_t           dets_lv_col1_h20_logic1;
   Int_t           dets_lv_col1_h20_CW_logic;
   Int_t           dets_lv_col1_jacket_logic;
   Int_t           dets_lv_Col2_accep1;
   Int_t           dets_lv_Col2_accep3;
   Int_t           dets_lv_PbWall_logic;
   Int_t           dets_lv_Lintel_logic1;
   Int_t           dets_lv_Coll5_logic1;
   Int_t           dets_lv_Coll6A_logic1;
   Int_t           dets_lv_Coll6B_logic1;
   Int_t           dets_lv_logic_inner_E1_1;
   Int_t           dets_lv_logic_C1_1;
   Int_t           dets_lv_logic_inner_E1_2;
   Int_t           dets_lv_logic_C1_2;
   Int_t           dets_lv_logic_inner_E1_3;
   Int_t           dets_lv_logic_C1_3;
   Int_t           dets_lv_logic_inner_E1_4;
   Int_t           dets_lv_logic_C1_4;
   Int_t           dets_lv_lefthut_Det_inside_logic;
   Int_t           dets_lv_lefthut_Det_outside_logic;
   Int_t           dets_lv_logic_inner_E1_5;
   Int_t           dets_lv_logic_C1_5;
   Int_t           dets_lv_logic_inner_E1_6;
   Int_t           dets_lv_logic_C1_6;
   Int_t           dets_lv_logic_inner_E1_7;
   Int_t           dets_lv_logic_C1_7;
   Int_t           dets_lv_logic_inner_E_1;
   Int_t           dets_lv_logic_C_1;
   Int_t           dets_lv_logic_shield1_top_1;
   Int_t           dets_lv_logic_shield2_top_1;
   Int_t           dets_lv_logic_shield3_top_1;
   Int_t           dets_lv_logic_shield4_top_1;
   Int_t           dets_lv_logic_inner_E_2;
   Int_t           dets_lv_logic_C_2;
   Int_t           dets_lv_logic_shield1_top_2;
   Int_t           dets_lv_logic_shield2_top_2;
   Int_t           dets_lv_logic_shield3_top_2;
   Int_t           dets_lv_logic_shield4_top_2;
   Int_t           dets_lv_logic_inner_E_3;
   Int_t           dets_lv_logic_C_3;
   Int_t           dets_lv_logic_shield2_top_3;
   Int_t           dets_lv_logic_shield3_top_3;
   Int_t           dets_lv_logic_inner_E_4;
   Int_t           dets_lv_logic_C_4;
   Int_t           dets_lv_logic_shield1_top_4;
   Int_t           dets_lv_logic_shield2_top_4;
   Int_t           dets_lv_logic_shield3_top_4;
   Int_t           dets_lv_logic_shield4_top_4;
   Int_t           dets_lv_logic_inner_E_5;
   Int_t           dets_lv_logic_C_5;
   Int_t           dets_lv_logic_shield1_top_5;
   Int_t           dets_lv_logic_shield2_top_5;
   Int_t           dets_lv_logic_shield3_top_5;
   Int_t           dets_lv_logic_shield4_top_5;
   Int_t           dets_lv_logic_inner_E_6;
   Int_t           dets_lv_logic_C_6;
   Int_t           dets_lv_logic_shield1_top_6;
   Int_t           dets_lv_logic_shield2_top_6;
   Int_t           dets_lv_logic_shield3_top_6;
   Int_t           dets_lv_logic_shield4_top_6;
   Int_t           dets_lv_logic_inner_E_7;
   Int_t           dets_lv_logic_C_7;
   Int_t           dets_lv_logic_shield1_top_7;
   Int_t           dets_lv_logic_shield2_top_7;
   Int_t           dets_lv_logic_shield3_top_7;
   Int_t           dets_lv_logic_shield4_top_7;
   Int_t           dets_lv_logic_twobounce_connector_1;
   Int_t           dets_lv_logic_twobounce_connector_2;
   Int_t           dets_lv_logic_twobounce_connector_3;
   Int_t           dets_lv_logic_twobounce_long;
   Int_t           dets_lv_logic_twobounce_groove;
 //remollSeed_t    *seed;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   Int_t           fRunNo;
   Int_t           fEvtNo;
   TString         fSeed;
   Double_t        rate;
 //remollEvent_t   *ev;
   Double_t        A;
   Double_t        Am;
   Double_t        xs;
   Double_t        Q2;
   Double_t        W2;
   Double_t        thcom;
   Double_t        beamp;
 //remollBeamTarget_t *bm;
   Double_t        x;
   Double_t        y;
   Double_t        z;
   Double_t        dx;
   Double_t        dy;
   Double_t        dz;
   Double_t        th;
   Double_t        ph;
   Int_t           part_;
   Int_t           part_pid[kMaxpart];   //[part_]
   Double_t        part_vx[kMaxpart];   //[part_]
   Double_t        part_vy[kMaxpart];   //[part_]
   Double_t        part_vz[kMaxpart];   //[part_]
   Double_t        part_px[kMaxpart];   //[part_]
   Double_t        part_py[kMaxpart];   //[part_]
   Double_t        part_pz[kMaxpart];   //[part_]
   Double_t        part_sx[kMaxpart];   //[part_]
   Double_t        part_sy[kMaxpart];   //[part_]
   Double_t        part_sz[kMaxpart];   //[part_]
   Double_t        part_th[kMaxpart];   //[part_]
   Double_t        part_ph[kMaxpart];   //[part_]
   Double_t        part_p[kMaxpart];   //[part_]
   Double_t        part_tpx[kMaxpart];   //[part_]
   Double_t        part_tpy[kMaxpart];   //[part_]
   Double_t        part_tpz[kMaxpart];   //[part_]
   Int_t           part_trid[kMaxpart];   //[part_]
   vector<double>  part_tjx[kMaxpart];
   vector<double>  part_tjy[kMaxpart];
   vector<double>  part_tjz[kMaxpart];
   Int_t           hit_;
   Int_t           hit_det[kMaxhit];   //[hit_]
   Int_t           hit_id[kMaxhit];   //[hit_]
   Int_t           hit_trid[kMaxhit];   //[hit_]
   Int_t           hit_pid[kMaxhit];   //[hit_]
   Int_t           hit_gen[kMaxhit];   //[hit_]
   Int_t           hit_mtrid[kMaxhit];   //[hit_]
   Double_t        hit_t[kMaxhit];   //[hit_]
   Double_t        hit_x[kMaxhit];   //[hit_]
   Double_t        hit_y[kMaxhit];   //[hit_]
   Double_t        hit_z[kMaxhit];   //[hit_]
   Double_t        hit_xl[kMaxhit];   //[hit_]
   Double_t        hit_yl[kMaxhit];   //[hit_]
   Double_t        hit_zl[kMaxhit];   //[hit_]
   Double_t        hit_r[kMaxhit];   //[hit_]
   Double_t        hit_ph[kMaxhit];   //[hit_]
   Double_t        hit_px[kMaxhit];   //[hit_]
   Double_t        hit_py[kMaxhit];   //[hit_]
   Double_t        hit_pz[kMaxhit];   //[hit_]
   Double_t        hit_pxl[kMaxhit];   //[hit_]
   Double_t        hit_pyl[kMaxhit];   //[hit_]
   Double_t        hit_pzl[kMaxhit];   //[hit_]
   Double_t        hit_sx[kMaxhit];   //[hit_]
   Double_t        hit_sy[kMaxhit];   //[hit_]
   Double_t        hit_sz[kMaxhit];   //[hit_]
   Double_t        hit_p[kMaxhit];   //[hit_]
   Double_t        hit_e[kMaxhit];   //[hit_]
   Double_t        hit_m[kMaxhit];   //[hit_]
   Double_t        hit_k[kMaxhit];   //[hit_]
   Double_t        hit_vx[kMaxhit];   //[hit_]
   Double_t        hit_vy[kMaxhit];   //[hit_]
   Double_t        hit_vz[kMaxhit];   //[hit_]
   Double_t        hit_edep[kMaxhit];   //[hit_]
   Int_t           sum_;
 //vector<remollGenericDetectorSumByPID_t> sum_by_pid[kMaxsum];
   Double_t        sum_edep[kMaxsum];   //[sum_]
   Int_t           sum_det[kMaxsum];   //[sum_]
   Int_t           sum_vid[kMaxsum];   //[sum_]
   Int_t           sum_n[kMaxsum];   //[sum_]

   // List of branches
   TBranch        *b_dets_sd;   //!
   TBranch        *b_dets_lv;   //!
   TBranch        *b_seed_fUniqueID;   //!
   TBranch        *b_seed_fBits;   //!
   TBranch        *b_seed_fRunNo;   //!
   TBranch        *b_seed_fEvtNo;   //!
   TBranch        *b_seed_fSeed;   //!
   TBranch        *b_rate;   //!
   TBranch        *b_ev_A;   //!
   TBranch        *b_ev_Am;   //!
   TBranch        *b_ev_xs;   //!
   TBranch        *b_ev_Q2;   //!
   TBranch        *b_ev_W2;   //!
   TBranch        *b_ev_thcom;   //!
   TBranch        *b_ev_beamp;   //!
   TBranch        *b_bm_x;   //!
   TBranch        *b_bm_y;   //!
   TBranch        *b_bm_z;   //!
   TBranch        *b_bm_dx;   //!
   TBranch        *b_bm_dy;   //!
   TBranch        *b_bm_dz;   //!
   TBranch        *b_bm_th;   //!
   TBranch        *b_bm_ph;   //!
   TBranch        *b_part_;   //!
   TBranch        *b_part_pid;   //!
   TBranch        *b_part_vx;   //!
   TBranch        *b_part_vy;   //!
   TBranch        *b_part_vz;   //!
   TBranch        *b_part_px;   //!
   TBranch        *b_part_py;   //!
   TBranch        *b_part_pz;   //!
   TBranch        *b_part_sx;   //!
   TBranch        *b_part_sy;   //!
   TBranch        *b_part_sz;   //!
   TBranch        *b_part_th;   //!
   TBranch        *b_part_ph;   //!
   TBranch        *b_part_p;   //!
   TBranch        *b_part_tpx;   //!
   TBranch        *b_part_tpy;   //!
   TBranch        *b_part_tpz;   //!
   TBranch        *b_part_trid;   //!
   TBranch        *b_part_tjx;   //!
   TBranch        *b_part_tjy;   //!
   TBranch        *b_part_tjz;   //!
   TBranch        *b_hit_;   //!
   TBranch        *b_hit_det;   //!
   TBranch        *b_hit_id;   //!
   TBranch        *b_hit_trid;   //!
   TBranch        *b_hit_pid;   //!
   TBranch        *b_hit_gen;   //!
   TBranch        *b_hit_mtrid;   //!
   TBranch        *b_hit_t;   //!
   TBranch        *b_hit_x;   //!
   TBranch        *b_hit_y;   //!
   TBranch        *b_hit_z;   //!
   TBranch        *b_hit_xl;   //!
   TBranch        *b_hit_yl;   //!
   TBranch        *b_hit_zl;   //!
   TBranch        *b_hit_r;   //!
   TBranch        *b_hit_ph;   //!
   TBranch        *b_hit_px;   //!
   TBranch        *b_hit_py;   //!
   TBranch        *b_hit_pz;   //!
   TBranch        *b_hit_pxl;   //!
   TBranch        *b_hit_pyl;   //!
   TBranch        *b_hit_pzl;   //!
   TBranch        *b_hit_sx;   //!
   TBranch        *b_hit_sy;   //!
   TBranch        *b_hit_sz;   //!
   TBranch        *b_hit_p;   //!
   TBranch        *b_hit_e;   //!
   TBranch        *b_hit_m;   //!
   TBranch        *b_hit_k;   //!
   TBranch        *b_hit_vx;   //!
   TBranch        *b_hit_vy;   //!
   TBranch        *b_hit_vz;   //!
   TBranch        *b_hit_edep;   //!
   TBranch        *b_sum_;   //!
   TBranch        *b_sum_edep;   //!
   TBranch        *b_sum_det;   //!
   TBranch        *b_sum_vid;   //!
   TBranch        *b_sum_n;   //!

   ferrous_analysis(Int_t nPrimaries, string detectors);
   virtual ~ferrous_analysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(Int_t par, Int_t prim, string DetNums);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);


//items for the field storage
public:

   static constexpr double pi  = 3.14159265358979323846;
   enum Coord { kR, kPhi, kZ };

   Double_t meter   = 1.;
   Double_t deg2rad = 0.01745329251994329;
   Double_t tesla   = 1.;
   Double_t m2mm    = 1000.;
   Double_t T2gauss = 10000.;

   Int_t    fN[3];
   Double_t fUnit[3] = {meter,deg2rad,meter};
   Double_t fMin[3];
   Double_t fMax[3];
   Double_t fStep[3];
   Double_t fFileMin[3], fFileMax[3];
   Int_t    fNxtant; // Number of *tants (septants, or whatever)
   Double_t fPhi0, fPhiLow, fxtantSize;
   TString  fFilename;

   // Storage space for the table
   std::vector< std::vector< std::vector< Double_t > > > fBFieldData[3];

   Double_t fZoffset, fPhiOffset;
   Double_t fZMapOffset, fPhiMapOffset;

   Double_t fFieldScale; // Scale overall field by this amount
   Double_t fMagCurrent0; // Scale overall field by this amount

   Bool_t   fInit;

   virtual void     InitializeGrid();
   virtual void     ReadFieldMap();
   virtual void     GetFieldValue(const Double_t Point[4], Double_t *Bfield );
   virtual TString  GetName();
   virtual void     SetFieldScale(Double_t s);
   virtual void     SetMagnetCurrent(Double_t s);
   virtual void     remollMagneticField( TString filename );
   virtual Bool_t   IsInBoundingBox(const Double_t* p);
   virtual void     PrintFieldMap( TString filename );
};

//#endif

//#ifdef ferrous_analysis_cxx
ferrous_analysis::ferrous_analysis(Int_t nPrimaries, string detectors){
  Loop(1,nPrimaries,detectors);
}

ferrous_analysis::~ferrous_analysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ferrous_analysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ferrous_analysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ferrous_analysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("dets.sd", &dets_sd_coilDet, &b_dets_sd);
   fChain->SetBranchAddress("dets.lv", &dets_lv_logic_shield4_top_3, &b_dets_lv);
   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_seed_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_seed_fBits);
   fChain->SetBranchAddress("fRunNo", &fRunNo, &b_seed_fRunNo);
   fChain->SetBranchAddress("fEvtNo", &fEvtNo, &b_seed_fEvtNo);
   fChain->SetBranchAddress("fSeed", &fSeed, &b_seed_fSeed);
   fChain->SetBranchAddress("rate", &rate, &b_rate);
   fChain->SetBranchAddress("A", &A, &b_ev_A);
   fChain->SetBranchAddress("Am", &Am, &b_ev_Am);
   fChain->SetBranchAddress("xs", &xs, &b_ev_xs);
   fChain->SetBranchAddress("Q2", &Q2, &b_ev_Q2);
   fChain->SetBranchAddress("W2", &W2, &b_ev_W2);
   fChain->SetBranchAddress("thcom", &thcom, &b_ev_thcom);
   fChain->SetBranchAddress("beamp", &beamp, &b_ev_beamp);
   fChain->SetBranchAddress("x", &x, &b_bm_x);
   fChain->SetBranchAddress("y", &y, &b_bm_y);
   fChain->SetBranchAddress("z", &z, &b_bm_z);
   fChain->SetBranchAddress("dx", &dx, &b_bm_dx);
   fChain->SetBranchAddress("dy", &dy, &b_bm_dy);
   fChain->SetBranchAddress("dz", &dz, &b_bm_dz);
   fChain->SetBranchAddress("th", &th, &b_bm_th);
   fChain->SetBranchAddress("ph", &ph, &b_bm_ph);
   fChain->SetBranchAddress("part", &part_, &b_part_);
   fChain->SetBranchAddress("part.pid", part_pid, &b_part_pid);
   fChain->SetBranchAddress("part.vx", part_vx, &b_part_vx);
   fChain->SetBranchAddress("part.vy", part_vy, &b_part_vy);
   fChain->SetBranchAddress("part.vz", part_vz, &b_part_vz);
   fChain->SetBranchAddress("part.px", part_px, &b_part_px);
   fChain->SetBranchAddress("part.py", part_py, &b_part_py);
   fChain->SetBranchAddress("part.pz", part_pz, &b_part_pz);
   fChain->SetBranchAddress("part.sx", part_sx, &b_part_sx);
   fChain->SetBranchAddress("part.sy", part_sy, &b_part_sy);
   fChain->SetBranchAddress("part.sz", part_sz, &b_part_sz);
   fChain->SetBranchAddress("part.th", part_th, &b_part_th);
   fChain->SetBranchAddress("part.ph", part_ph, &b_part_ph);
   fChain->SetBranchAddress("part.p", part_p, &b_part_p);
   fChain->SetBranchAddress("part.tpx", part_tpx, &b_part_tpx);
   fChain->SetBranchAddress("part.tpy", part_tpy, &b_part_tpy);
   fChain->SetBranchAddress("part.tpz", part_tpz, &b_part_tpz);
   fChain->SetBranchAddress("part.trid", part_trid, &b_part_trid);
   fChain->SetBranchAddress("part.tjx", part_tjx, &b_part_tjx);
   fChain->SetBranchAddress("part.tjy", part_tjy, &b_part_tjy);
   fChain->SetBranchAddress("part.tjz", part_tjz, &b_part_tjz);
   fChain->SetBranchAddress("hit", &hit_, &b_hit_);
   fChain->SetBranchAddress("hit.det", hit_det, &b_hit_det);
   fChain->SetBranchAddress("hit.id", hit_id, &b_hit_id);
   fChain->SetBranchAddress("hit.trid", hit_trid, &b_hit_trid);
   fChain->SetBranchAddress("hit.pid", hit_pid, &b_hit_pid);
   fChain->SetBranchAddress("hit.gen", hit_gen, &b_hit_gen);
   fChain->SetBranchAddress("hit.mtrid", hit_mtrid, &b_hit_mtrid);
   fChain->SetBranchAddress("hit.t", hit_t, &b_hit_t);
   fChain->SetBranchAddress("hit.x", hit_x, &b_hit_x);
   fChain->SetBranchAddress("hit.y", hit_y, &b_hit_y);
   fChain->SetBranchAddress("hit.z", hit_z, &b_hit_z);
   fChain->SetBranchAddress("hit.xl", hit_xl, &b_hit_xl);
   fChain->SetBranchAddress("hit.yl", hit_yl, &b_hit_yl);
   fChain->SetBranchAddress("hit.zl", hit_zl, &b_hit_zl);
   fChain->SetBranchAddress("hit.r", hit_r, &b_hit_r);
   fChain->SetBranchAddress("hit.ph", hit_ph, &b_hit_ph);
   fChain->SetBranchAddress("hit.px", hit_px, &b_hit_px);
   fChain->SetBranchAddress("hit.py", hit_py, &b_hit_py);
   fChain->SetBranchAddress("hit.pz", hit_pz, &b_hit_pz);
   fChain->SetBranchAddress("hit.pxl", hit_pxl, &b_hit_pxl);
   fChain->SetBranchAddress("hit.pyl", hit_pyl, &b_hit_pyl);
   fChain->SetBranchAddress("hit.pzl", hit_pzl, &b_hit_pzl);
   fChain->SetBranchAddress("hit.sx", hit_sx, &b_hit_sx);
   fChain->SetBranchAddress("hit.sy", hit_sy, &b_hit_sy);
   fChain->SetBranchAddress("hit.sz", hit_sz, &b_hit_sz);
   fChain->SetBranchAddress("hit.p", hit_p, &b_hit_p);
   fChain->SetBranchAddress("hit.e", hit_e, &b_hit_e);
   fChain->SetBranchAddress("hit.m", hit_m, &b_hit_m);
   fChain->SetBranchAddress("hit.k", hit_k, &b_hit_k);
   fChain->SetBranchAddress("hit.vx", hit_vx, &b_hit_vx);
   fChain->SetBranchAddress("hit.vy", hit_vy, &b_hit_vy);
   fChain->SetBranchAddress("hit.vz", hit_vz, &b_hit_vz);
   fChain->SetBranchAddress("hit.edep", hit_edep, &b_hit_edep);
   fChain->SetBranchAddress("sum", &sum_, &b_sum_);
   fChain->SetBranchAddress("sum.edep", sum_edep, &b_sum_edep);
   fChain->SetBranchAddress("sum.det", sum_det, &b_sum_det);
   fChain->SetBranchAddress("sum.vid", sum_vid, &b_sum_vid);
   fChain->SetBranchAddress("sum.n", sum_n, &b_sum_n);
   Notify();
}

Bool_t ferrous_analysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ferrous_analysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t ferrous_analysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

Bool_t ferrous_analysis::IsInBoundingBox(const Double_t* p) {
    fZoffset = 0.;
    //cout << __PRETTY_FUNCTION__ << "p: (" << p[0] << "," << p[1] << "," << p[2] << ")" << endl;
    if ( (p[2] - fZoffset < fMin[kZ]) || (p[2] - fZoffset > fMax[kZ]) ){
        cout << "Failed condition 1" << endl;
        return false;
    }
    if (p[0] < -1.*fMax[kR] || p[0] > fMax[kR]){
        cout << "Failed condition 2" << endl;
        return false;
    }
    if (p[1] < -1.*fMax[kR] || p[1] > fMax[kR]){
        cout << "Failed condition 3" << endl;
        cout << ">>> if(p[1] < -1.*fMax[kR] || p[1] > fMax[kR])" << endl;
        cout << ">>> if(" << p[1] << "<" << -1.*fMax[kR] << " || " << p[1] << ">" << fMax[kR] << ")" << endl;
        return false;
    }
    return true;
}

void ferrous_analysis::InitializeGrid() {
    /*!
     * Dynamically size 3D vectors to what we need to store the file
     * These should already be set from values read in from the fieldmap
     */

    if( fN[kR] <= 0 || fN[kPhi] <= 0 || fN[kZ] <= 0 ){
	cerr << "Error " << __FILE__ << " line " << __LINE__
	    << ": grid size invalid.  Aborting" << endl;
	exit(1);
    }

    cout << "Initializing field map grid for " << fFilename << endl;
    Int_t cidx, ridx, pidx, zidx;

    for( cidx = kR; cidx <= kZ; cidx++ ){
	// Set up storage space for table
	fBFieldData[cidx].clear();
	fBFieldData[cidx].resize(fN[kR]);

	for( ridx = 0; ridx < fN[kR]; ridx++) {
	    fBFieldData[cidx][ridx].resize(fN[kPhi]);

	    for( pidx=0; pidx<fN[kPhi]; pidx++) {
		fBFieldData[cidx][ridx][pidx].resize(fN[kZ]);

		for( zidx=0; zidx<fN[kZ]; zidx++) {
		    fBFieldData[cidx][ridx][pidx][zidx] = 0.0;
		} // end of z
	    } // end of phi
	} // end of r
    } // end coordinate index

    cout << "Map grid for " << fFilename << " initialized" << endl;

    return;
}

void ferrous_analysis::ReadFieldMap(){
    const Double_t eps = 1e-6;

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << __PRETTY_FUNCTION__ << ": Reading the field grid from " << fFilename << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    Int_t ridx = 0, pidx=0, zidx=0;

    Double_t raw_R_m,raw_Z_m,raw_Phi_deg;
    Double_t br,bp,bz;

    // open the field map file
    Int_t nlines = 0;
    // int nread;
    Int_t cidx;


    // Create STL ifstream
    std::ifstream inputfile;

    // Set file as source
    inputfile.open(fFilename.Data());

    if (!inputfile.good() ){
	cerr << "Error " << __FILE__ << " line " << __LINE__ << endl << endl << endl << endl << endl
	    << ": File " << fFilename << " could not open.  Aborting" << endl
	    << ": ERROR - Suggested MOLLER Field Map files available at http://hallaweb.jlab.org/12GeV/Moller/downloads/remoll/" << endl << endl << endl << endl << endl;
	exit(1);
    }

    // Variable that will contain single lines
    std::string inputline;

    // Read in data about grid
    for (size_t cidx = kR; cidx <= kZ; cidx++) {
        getline(inputfile,inputline);
        if (std::istringstream(inputline) >> fN[cidx] >> fMin[cidx] >> fMax[cidx]) {
            //fMin[cidx] *= fUnit[cidx];
            //fMax[cidx] *= fUnit[cidx];
            fStep[cidx] = (fMax[cidx] - fMin[cidx]) / (fN[cidx] - 1);
            cout << "N,min,max,step[" << cidx << "] = " << fN[cidx] << ","
                   << fMin[cidx] << "," << fMax[cidx] << "," << fStep[cidx] << endl;
        } else {
	    cerr << "Error " << __FILE__ << " line " << __LINE__
		<< ": File " << fFilename << " contains unreadable header.  Aborting" << endl;
	    exit(1);
	}
    }

    // Read in grid offsets.
    getline(inputfile,inputline);
    if (std::istringstream(inputline) >> fPhiMapOffset >> fZMapOffset) {
        cout << "PhiMapOffset,ZMapOffset = " << fPhiMapOffset << ","
            << fZMapOffset << endl;
    } else {
	cerr << "Error " << __FILE__ << " line " << __LINE__
	    << ": File " << fFilename << " contains unreadable header.  Aborting" << endl;
	exit(1);
    }

    cout << "fPhiMapOffset (raw read): " << fPhiMapOffset << endl;

    fPhiMapOffset *= fUnit[kPhi];
    fZMapOffset   *= fUnit[kZ];

    cout << "fPhiMapOffset (unit adj): " << fPhiMapOffset << endl;

    getline(inputfile,inputline);
    if (std::istringstream(inputline) >> fNxtant) {
        cout << "Nxtant = " << fNxtant << endl;
    } else {
	cerr << "Error " << __FILE__ << " line " << __LINE__
	    << ": File " << fFilename << " contains unreadable header.  Aborting" << endl;
	exit(1);
    }
    cout << __PRETTY_FUNCTION__ << ": N xtants = " << fNxtant << endl;

    fxtantSize = 2.0*pi/fNxtant;

    //////////////////////////////////////////////////////////////////////
    getline(inputfile,inputline);
    if (std::istringstream(inputline) >> fMagCurrent0) {
        cout << "MagCurrent0 = " << fMagCurrent0 << endl;
    } else {
	cerr << "Error " << __FILE__ << " line " << __LINE__
	    << ": File " << fFilename << " contains unreadable header.  Aborting" << endl;
	exit(1);
    }
    cout << __PRETTY_FUNCTION__ << ": field current = " << fMagCurrent0 << " A" << endl;

    // Sanity check on header data
    if( !( fMin[kR] >= 0.0 && fMin[kR] < fMax[kR] &&
	   -180.0 <= fMin[kPhi] && fMin[kPhi] <= 180.0 &&
	   -180.0 <= fMax[kPhi] && fMax[kPhi] <= 180.0 &&
	   fMin[kPhi]  < fMax[kPhi] &&
	   fMin[kZ] < fMax[kZ] &&
	   fN[kR] > 0 && fN[kPhi] > 0 && fN[kZ] > 0 &&
	   fNxtant > 0
       	 )
      ){
	cerr << "Error " << __FILE__ << " line " << __LINE__
	    << ": File " << fFilename << " contains invalid header data.  Aborting" << endl;
	cerr<<"fMin[kR] >= 0.0: "<<bool(fMin[kR] >= 0.0)<<" "<<fMin[kR]<<endl
	      <<"fMin[kR] <= fMax[kR]: "<<bool( fMin[kR] <= fMax[kR])<<" "<<fMax[kR]<<endl
	      <<"-180.0 < fMin[kPhi]: "<<bool( -180.0 < fMin[kPhi])<<" "<<fMin[kPhi]<<endl
	      <<"fMin[kPhi] < 180.0: "<<bool(fMin[kPhi] < 180.0)<<endl
	      <<"-180.0 <= fMax[kPhi]: "<<bool(-180.0 <= fMax[kPhi])<<" "<<fMax[kPhi]<<endl
	      <<"fMax[kPhi] < 180.0: "<<bool(fMax[kPhi] < 180.0 )<<endl
	      <<"fMin[kPhi]  < fMax[kPhi]: "<<bool(fMin[kPhi]  < fMax[kPhi] )<<endl
	      <<"fMin[kZ] < fMax[kZ]: "<<bool(fMin[kZ] < fMax[kZ] )<<" "<<fMin[kZ]<<" "<<fMax[kZ]<<endl
	      <<"fN[kR] > 0: "<<bool(fN[kR] > 0 )<<" "<<fN[kR]<<endl
	      <<"fN[kPhi] > 0: "<<bool(fN[kPhi] > 0 )<<" "<<fN[kPhi]<<endl
	      <<"fN[kZ] > 0: "<<bool(fN[kZ] > 0 )<<" "<<fN[kZ]<<endl
	      <<"fNxtant > 0: "<<bool(fNxtant > 0 )<<" "<<fNxtant<<endl<<endl;

	exit(1);
    }

    // Get in proper units
    for( cidx = kR; cidx <= kZ; cidx++ ){
        cout << "(raw read) fMin[" << cidx << "]=" << fMin[cidx] << endl;
        cout << "(raw read) fMax[" << cidx << "]=" << fMax[cidx] << endl;
	fMin[cidx] *= fUnit[cidx];
	fMax[cidx] *= fUnit[cidx];
	cout << "(unit adj) fMin[" << cidx << "]=" << fMin[cidx] << endl;
	cout << "(unit adj) fMax[" << cidx << "]=" << fMax[cidx] << endl;
    }

    for( cidx = kR; cidx <= kZ; cidx++ ){
	fFileMin[cidx] = fMin[cidx];
	fFileMax[cidx] = fMax[cidx];
    }

    fMin[kPhi] += fPhiMapOffset;
    fMax[kPhi] += fPhiMapOffset;

    cout << fixed << setprecision(15) << "(w/offset) fMin[" << kPhi << "]=" << fMin[kPhi] << endl;
    cout << fixed << setprecision(15) << "(w/offset) fMax[" << kPhi << "]=" << fMax[kPhi] << endl;

    // Put between -180 and 180
    fMin[kPhi] = fmodf(fMin[kPhi] + pi/2.0, pi) - pi/2.0;
    fMax[kPhi] = fmodf(fMax[kPhi] + pi/2.0, pi) - pi/2.0;

    cout << "(post put) fMin[" << kPhi << "]=" << fMin[kPhi] << endl;
    cout << "(post put) fMax[" << kPhi << "]=" << fMax[kPhi] << endl;

    double mapphirange = fMax[kPhi] - fMin[kPhi];

    if( mapphirange < 0.0 ){
	mapphirange += 2.0*pi;
    }

    fMin[kZ]   += fZMapOffset;
    fMax[kZ]   += fZMapOffset;

    ///////////////////////////////////

    if( !( fMin[kPhi] >= -pi && fMin[kPhi] <= pi ) ){
	cerr << "Error " << __FILE__ << " line " << __LINE__
	    << ": File " << fFilename << " header contains invalid phi range.  Aborting" << endl;
	exit(1);
    }

    if( fabs( fabs((mapphirange - fxtantSize)/mapphirange)) > 0.03 ){
	cerr << "Warning " << __FILE__ << " line " << __LINE__ << endl
	    << "File " << fFilename << " header contains too narrow phi range for given xtants." << endl
            << "Warning:   Proceeding assuming null field in non-described regions" << endl
            << (fMax[kPhi] - fMin[kPhi])/fUnit[kPhi] << " deg range < " <<  fxtantSize/fUnit[kPhi] << " deg xtant" << endl;
    }

    if( fabs(fabs(mapphirange - fxtantSize) / (mapphirange/fN[kPhi]) - 1) < 0.03 ){
	cerr << "Warning " << __FILE__ << " line " << __LINE__ << endl
	    << "File " << fFilename << " header contains a gap in the phi range which seems" << endl
            << "to correspond perfectly with a slice in phi. This will result in a gap in coverage." << endl;
    }


    fPhi0   = (fMax[kPhi] + fMin[kPhi])/2.0;
    if( fMax[kPhi] < fMin[kPhi] ){
	fPhi0 += pi;
    }

    fPhiLow = fPhi0 - fxtantSize/2.0;
    if( fPhiLow < -pi ){
	fPhiLow += 2.0*pi;
    }

    InitializeGrid();


    for( zidx = 0; zidx < fN[kZ]; zidx++ ){
	for( pidx = 0; pidx < fN[kPhi]; pidx++ ){
	    for( ridx = 0; ridx < fN[kR]; ridx++ ){

	        getline(inputfile,inputline);

		// Read in field values and assign units
		if (std::istringstream(inputline) >> raw_R_m >> raw_Phi_deg >> raw_Z_m >> br >> bp >> bz) {
                  nlines++;
		} else {
		    cerr << "Error " << __FILE__ << " line " << __LINE__
			<< ": File " << fFilename << " contains invalid data.  Aborting" << endl;
		    exit(1);
		}

		/* convert to proper units */
		Double_t val_R   = raw_R_m*fUnit[kR];
		Double_t val_Z   = raw_Z_m*fUnit[kZ];
		Double_t val_Phi = raw_Phi_deg*fUnit[kPhi];

		////////////////////////////////////////////////////////////////////
		// This checks that we're reading in properly framed data
		// This can probably be condensed
		if( ridx == 0 ){
		    if( fabs(val_R - fMin[kR]) > eps ){
			cerr << "Error " << __FILE__ << " line " << __LINE__
			    << ": File " << fFilename << " contains bad data framing in R.  Aborting" << endl;
			cerr << "Index ("<< ridx << ", " << pidx << ", " <<  zidx <<  ")  Expected " << fMin[kR] << " read " << raw_R_m << endl;
			exit(1);
		    }
		}
		if( pidx == 0 ){
		    if( fabs(val_Phi - fFileMin[kPhi]) > eps ){
			cerr << "Error " << __FILE__ << " line " << __LINE__
			    << ": File " << fFilename << " contains bad data framing in Phi.  Aborting" << endl;
			cerr << "Index ("<< ridx << ", " << pidx << ", " <<  zidx <<  ")  Expected fFileMin[kPhi]: " << fFileMin[kPhi] << " read raw_phi_deg: " << raw_Phi_deg << ", and val_Phi is: " << val_Phi << endl;
			exit(1);
		    }
		}
		if( zidx == 0 ){
		    if( fabs(val_Z - fFileMin[kZ]) > eps ){
			cerr << "Error " << __FILE__ << " line " << __LINE__
			    << ": File " << fFilename << " contains bad data framing in Z.  Aborting" << endl;
			cerr << "Index ("<< ridx << ", " << pidx << ", " <<  zidx <<  ")  Expected " << fFileMin[kZ] << " read " << raw_Z_m << endl;
			exit(1);
		    }
		}
		if( ridx == fN[kR]-1 ){
		    if( fabs(val_R - fMax[kR]) > eps ){
			cerr << "Error " << __FILE__ << " line " << __LINE__
			    << ": File " << fFilename << " contains bad data framing in R.  Aborting" << endl;
			cerr << "Index ("<< ridx << ", " << pidx << ", " <<  zidx <<  ")  Expected " << fMax[kR] << " read " << raw_R_m << endl;
			exit(1);
		    }
		}
		if( pidx == fN[kPhi]-1 ){
		    if( fabs(val_Phi - fFileMax[kPhi]) > eps ){
			cerr << "Error " << __FILE__ << " line " << __LINE__
			    << ": File " << fFilename << " contains bad data framing in Phi.  Aborting" << endl;
			cerr << "Index ("<< ridx << ", " << pidx << ", " <<  zidx <<  ")  Expected " << fFileMax[kPhi] << " read " << raw_Phi_deg << endl;
			exit(1);
		    }
		}
		if( zidx == fN[kZ]-1 ){
		    if( fabs(val_Z - fFileMax[kZ]) > eps ){
			cerr << "Error " << __FILE__ << " line " << __LINE__
			    << ": File " << fFilename << " contains bad data framing in Z.  Aborting" << endl;
			cerr << "Index ("<< ridx << ", " << pidx << ", " <<  zidx <<  ")  Expected " << fFileMax[kZ] << " read " << raw_Z_m << endl;
			exit(1);
		    }
		}
		////////////////////////////////////////////////////////////////////


		// Set the grid values to the values which have been read-in
		fBFieldData[kR][ridx][pidx][zidx]   = br*tesla;
		fBFieldData[kPhi][ridx][pidx][zidx] = bp*tesla;
		fBFieldData[kZ][ridx][pidx][zidx]   = bz*tesla;

	    }
	}
    }

    fInit = true;
    cout << "... done reading " << nlines << " lines." << endl<< endl;

}

void ferrous_analysis::GetFieldValue(const Double_t Point[4], Double_t *Bfield )
{
    //cout << "Checking field at vertex point: " << Point[0] << "," << Point[1] << "," << Point[2] << ")" << endl;
    // Check the bounding box
    if (! IsInBoundingBox(Point)){cerr << "Point outside of bounding box!!!" << endl; return;}

    // First we have to translate into polar or cylindric coordinates
    // since the field maps are given in cylindric coordinates and the
    // interpolation will be done in cylindric coordinates as well.

    // Then we need to translate to cartesian components to give them
    // back to the field manager

    // 1. First calculate r and z
    Double_t r = sqrt(Point[0]*Point[0] + Point[1]*Point[1]);
    Double_t z = Point[2] - fZoffset;

    // Check that the point is a valid number
    if( std::isnan(r) || std::isinf(r) ||
	std::isnan(z) || std::isinf(z) ){

	cerr << __FILE__ << " line " << __LINE__ << ": ERROR bad conversion to cylindrical coordinates" << endl;
	cerr << "Point  ( " << Point[0]/meter << ", " << Point[1]/meter << ", " << Point[2]/meter << " ) m" << endl;

	exit(1);
    }

    // Check that the point is within the defined region
    if( r >= fMax[kR] || r < fMin[kR] ||
	z >= fMax[kZ] || z < fMin[kZ] ){
	return;
    }

    // Ensure we're going to get our grid indices correct
    assert( fMin[kR] <= r && r < fMax[kR] );
    assert( fMin[kZ] <= z && z < fMax[kZ] );

    // 2. Next calculate phi (slower)
    Double_t phi = atan2(Point[1],Point[0]);

    // Check that the point is a valid number
    if( std::isnan(phi) || std::isinf(phi) ){

	cerr << __FILE__ << " line " << __LINE__ << ": ERROR bad conversion to cylindrical coordinates" << endl;
	cerr << "Point  ( " << Point[0]/meter << ", " << Point[1]/meter << ", " << Point[2]/meter << " ) m" << endl;

	exit(1);
    }

    // Get xtant number and fraction into xtant
    Double_t dxtant;
    Double_t dphi =
	phi - fPhiLow >= 0.0 ?
		modf( (  phi           - fPhiLow)/fxtantSize, &dxtant ):
		modf( ( (2.0*pi + phi) - fPhiLow)/fxtantSize, &dxtant ); // Wrap around

    Int_t xtant = (Int_t) dxtant;

    // Local phi (in file coordinates)
    Double_t lphi = dphi*fxtantSize + fPhiLow - fPhiMapOffset;
    if( lphi < -pi ){ lphi += 2.0*pi; }
    if( lphi >  pi ){ lphi -= 2.0*pi; }

    if( !( xtant >= 0 && xtant < fNxtant ) ){

	cerr << "Error:  " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":" << endl
               << "  xtant calculation failed. xtant " <<  xtant << " ( " << dxtant << " )  found where "
               << fNxtant << " is specified.  phi = " << phi/fUnit[kPhi] << " deg" << endl;

	exit(1);
    }

    // Check that the point is within the defined region
    // before interpolation.  If it is outside, the field is zero
    if( lphi >= fFileMax[kPhi] || lphi < fFileMin[kPhi] ){
	return;
    }

    // Ensure we're going to get our grid indices correct
    assert( fFileMin[kPhi] <= lphi && lphi < fFileMax[kPhi] );


    // 3. Get interoplation variables
    // the N-1 here is fencepost problem
    Double_t x[3] = {0};
    Double_t didx[3] = {0};
    x[kR]   = modf( ( r - fMin[kR] )*(fN[kR]-1)/( fMax[kR] - fMin[kR] ),            &(didx[kR])   );
    x[kPhi] = modf( ( lphi - fFileMin[kPhi] )*(fN[kPhi]-1)/( fFileMax[kPhi] - fFileMin[kPhi] ), &(didx[kPhi]) );
    x[kZ]   = modf( ( z - fMin[kZ] )*(fN[kZ]-1)/( fMax[kZ] - fMin[kZ] ),            &(didx[kZ])   );

    // Cast these to integers for indexing and check
    Int_t idx[3] = {0};
    for(int cidx = 0; cidx < 3; cidx++ ){ idx[cidx] = (Int_t) didx[cidx]; }

    assert( 0 <= idx[kR]   && idx[kR]   < fN[kR] );
    assert( 0 <= idx[kPhi] && idx[kPhi] < fN[kPhi] );
    assert( 0 <= idx[kZ]   && idx[kZ]   < fN[kZ] );

    // Interpolate
    Double_t Bint[3] = {0};
    for(int cidx = 0; cidx < 3; cidx++ ){

        Double_t c00, c10, c01, c11;
	c00 = fBFieldData[cidx][idx[kR]][idx[kPhi]][idx[kZ]]*(1.0-x[kR])
	    + fBFieldData[cidx][idx[kR]+1][idx[kPhi]][idx[kZ]]*x[kR];
	c10 = fBFieldData[cidx][idx[kR]][idx[kPhi]+1][idx[kZ]]*(1.0-x[kR])
	    + fBFieldData[cidx][idx[kR]+1][idx[kPhi]+1][idx[kZ]]*x[kR];
	c01 = fBFieldData[cidx][idx[kR]][idx[kPhi]][idx[kZ]+1]*(1.0-x[kR])
	    + fBFieldData[cidx][idx[kR]+1][idx[kPhi]][idx[kZ]+1]*x[kR];
	c11 = fBFieldData[cidx][idx[kR]][idx[kPhi]+1][idx[kZ]+1]*(1.0-x[kR])
	    + fBFieldData[cidx][idx[kR]+1][idx[kPhi]+1][idx[kZ]+1]*x[kR];

        Double_t c0, c1;
	c0  = c00*(1.0-x[kPhi]) + c10*x[kPhi];
	c1  = c01*(1.0-x[kPhi]) + c11*x[kPhi];

	Bint[cidx] = c0*(1.0-x[kZ])+c1*x[kZ];
    }

    TVector3 Bcart = TVector3(Bint[kR], Bint[kPhi], Bint[kZ]);
    Bcart.RotateZ(lphi + fPhiMapOffset);      // this changes coordinates from Br, Bphi to Bx, By
    // Now we are cartesian, which is what we need to feed Geant4 (yay)

    Bcart.RotateZ(xtant*fxtantSize);  // rotate into our xtant
    Bfield[0] = Bcart.x()*fFieldScale;
    Bfield[1] = Bcart.y()*fFieldScale;
    Bfield[2] = Bcart.z()*fFieldScale;
}

void ferrous_analysis::remollMagneticField( TString filename ){

    fFilename = filename;

    // Initialize grid variables
    for( int cidx = kR; cidx < kZ; cidx++ ){
	fN[cidx] = -1;
	fMin[cidx] = -1e9;
	fMax[cidx] = -2e9;
    }

    fPhi0 = -1e9;

    // Default offset for field maps in reference frame with
    // the hall pivot at z = 0.
    fZoffset = 0.0;

    fInit = false;
    fMagCurrent0 = -1e9;

    fFieldScale = 1.0;

    ReadFieldMap();
}

//UNSURE IF NEEDED... PROBABLY NEED FOR MULTIPLE MAPS
TString ferrous_analysis::GetName(){
    if( !fInit ){
	cerr << "WARNING " << __FILE__ << " line " << __LINE__
	    << ": access uninitialized field." << endl;
	return TString("");
    }

    return fFilename;
}

//UNSURE IF NEEDED
void ferrous_analysis::SetFieldScale(Double_t s){
    fFieldScale = s;
    cout << fFilename << " scale set to " << s << endl;
    return;
}

//UNSURE IF NEEDED
void ferrous_analysis::SetMagnetCurrent(Double_t s){
    if( fMagCurrent0 > 0.0 ){
       	SetFieldScale(s/fMagCurrent0);
    } else {
    	cerr << "Warning:  " << __FILE__ << " line " << __LINE__
	    << ": Field current not specified in map " << fFilename << " - Ignoring and proceeding " << endl;
    }
    return;
}

void ferrous_analysis::PrintFieldMap( TString filename ){

    remollMagneticField( filename );

    Int_t counter(0);

    Int_t ridx = 0, pidx=0, zidx=0;

    for( zidx = 0; zidx < fN[kZ]; zidx++ ){
	for( pidx = 0; pidx < fN[kPhi]; pidx++ ){
	    for( ridx = 0; ridx < fN[kR]; ridx++ ){

		Double_t Br = fBFieldData[kR][ridx][pidx][zidx];
		Double_t Bp = fBFieldData[kPhi][ridx][pidx][zidx];
		Double_t Bz = fBFieldData[kZ][ridx][pidx][zidx];

		Double_t R  = (Double_t)ridx/(Double_t)(fN[kR]-1)  * (Double_t)(fMax[kR]-fMin[kR])    + (Double_t)fMin[kR];
		Double_t P  = (Double_t)pidx/(Double_t)(fN[kPhi]-1)* /*(Double_t)(fMax[kPhi]-fMin[kPhi])*/ 360.+ (Double_t)fMin[kPhi] * 180 / pi;
		Double_t Z  = (Double_t)zidx/(Double_t)(fN[kZ]-1)  * (Double_t)(fMax[kZ]-fMin[kZ])    + (Double_t)fMin[kZ];

		if(counter%10000==0){
		    cout << fixed
		         << setprecision(3) << R  << "\t"
		         << setprecision(0) << P  << "\t"
		         << setprecision(1) << Z  << "\t"
		         << setprecision(8) << Br << "\t"
		         << setprecision(8) << Bp << "\t"
		         << setprecision(8) << Bz << endl;
		}

		counter++;

            }
        }
    }

}

//#endif
//#ifdef ferrous_analysis_cxx
