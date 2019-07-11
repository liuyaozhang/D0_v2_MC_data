//
1. readMVAtreeFull.C :filling MC sample in TH3D hDcaVsMassAndMvaPD0_%d_%d and 
hDcaVsMassAndMvaPD0_all_%d_%d

2. massHists_MC.C:  ProjectionX to produce mass distributions from hDcaVsMassAndMvaPD0. 

3. ./FlowCorr/src/corr2D_trg_d0.cc:  The pe-trigger-particle associated yield distribution
                                     B(0,0)*S(Deta,Dphi)/B(Deta,Dphi) and prepare to extract the 
                                     Vn(p_{T}^{trig},p_{T}^{assoc}). 
   
   ./FlowCor/src/corr2D_trg_ref.cc:  prepare to extract the Vn(p_{T}^{assoc},p_{T}^{assoc}). 

4. /FlowCorr/proj1D/proj1D_v2.C: to fitted fourier coeffients (vn(p_{T}^{trig})). 

5.  massfitvn_combine_d0.C: mass distrubiion and vn_mass. 

      
