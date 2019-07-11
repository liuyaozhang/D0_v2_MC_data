#include "include/myAnaConsts.h"

//void massHists(const char* output="MC/hMC_mass_npd0ana1_y1.0.root", const float yMax=1.0, bool isPrompt=false)
//void massHists(const char* output="MC/hMC_mass_d0ana_y1.2.root", const float yMax=1.2, bool isPrompt=true)
//void massHists(const char* output="MC/hMC_mass_d0ana_y1.5.root", const float yMax=1.5, bool isPrompt=true)
void massHists_MC(const char* output="MC/hMC_mass_d0ana_y2_new.root", const float yMax=2, bool isPrompt=true)
{
   TFile* f1[ana::nPt];
   TH3D* hDcaVsMassAndMva[ana::nPt];
   TH3D* hDcaVsMassAndMva_All[ana::nPt];
   for(int ipt=0; ipt<ana::nPt; ipt++){
      if(isPrompt) {
         f1[ipt] = new TFile(Form("MC/d0ana_hists_pT%.1f-%.1f_y%.1f-%.1f.root", ana::ptbin[ipt], ana::ptbin[ipt+1], 0.0, yMax));
         hDcaVsMassAndMva[ipt] = (TH3D*)f1[ipt]->Get(Form("hDcaVsMassAndMvaPD0_%d_%d", ipt+1,1));
         hDcaVsMassAndMva_All[ipt] = (TH3D*)f1[ipt]->Get(Form("hDcaVsMassAndMvaPD0_All_%d_%d", ipt+1,1));
      }else{
        continue; 
        }  
   }
   TH1D* hMass_small[ana::nPt];
   TH1D* hMass_small_all[ana::nPt];
   int mvaBinMin, mvaBinMax, dcaBinMinSmall, dcaBinMaxSmall;
   for(int ipt=0; ipt<ana::nPt; ipt++){
      if(isPrompt){
         mvaBinMin = hDcaVsMassAndMva[ipt]->GetYaxis()->FindBin(ana::mvaCut_PD0[ipt]+0.1*hDcaVsMassAndMva[ipt]->GetYaxis()->GetBinWidth(1));
         mvaBinMax = hDcaVsMassAndMva[ipt]->GetYaxis()->GetNbins()+1;
         dcaBinMinSmall = 0;
         dcaBinMaxSmall = hDcaVsMassAndMva[ipt]->GetZaxis()->GetNbins()+1;
      }else{
          continue; 
          } 
      hMass_small[ipt] = hDcaVsMassAndMva[ipt]->ProjectionX(Form("hMC_mass_small_pt%d", ipt), mvaBinMin, mvaBinMax, dcaBinMinSmall, dcaBinMaxSmall);
      hMass_small_all[ipt] = hDcaVsMassAndMva_All[ipt]->ProjectionX(Form("hMC_mass_all_small_pt%d", ipt), mvaBinMin, mvaBinMax, dcaBinMinSmall, dcaBinMaxSmall);
   }
   TFile f2(output, "recreate");
   for(int ipt=0; ipt<ana::nPt; ipt++){
      hMass_small[ipt]->Write();
     // hMass_large[ipt]->Write();
      hMass_small_all[ipt]->Write();
    //  hMass_large_all[ipt]->Write();
   }
}
