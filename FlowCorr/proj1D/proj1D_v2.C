#include "include/myAnaConsts.h"

void proj1D_v2(const char* input_d0= "/afs/cern.ch/user/z/zhangli/public/FlowCorr/batch/foutlist/MultiplicityMT100LT250_absRapidityMT0LT2_Pt1.5_3.0_5.0_7.0_10.0/data.list_total.root", const char* input_ref = "/afs/cern.ch/user/z/zhangli/public/FlowCorr/batch/fout_ref_list/MultiplicityMT100LT250_absRapidityMT0LT1_PT1.5_3.0_5.0_7.0_10.0_Ref/data.list.ref_total.root")
{
   TFile* f1 = new TFile(input_d0);
   TFile* f2 = new TFile(input_ref);
   TH2D* h2DSignal_D0_Small[ana::nMass][ana::nPt];
   TH2D* h2DBackground_D0_Small[ana::nMass][ana::nPt];
   TH1D* hMass_D0[ana::nMass][ana::nPt];
   TH1D* hMass_pt_small[ana::nPt]; 

   int Content_Background_Deta0_Dphi0[ana::nMass][ana::nPt] = {0};
   int etaBin0 = 0;
   int phiBin0 = 0;
    
   TF1 *constant = new TF1("constant","1.",-10,10);
   double v2_smallDCA[ana::nMass][ana::nPt] = {0.};
   double v2_smallDCA_err[ana::nMass][ana::nPt] = {0.};

   double v2_smallDCA_Ref = {0.}; 
   double v2_smallDCA_err_Ref = {0.}; 
   
   for(int ipt=0;ipt<ana::nPt;ipt++){
      hMass_pt_small[ipt] = (TH1D*) f1->Get(Form("h1DMass_pt%d",ipt)); 
      }

   
   for(int imass=0; imass<ana::nMass; imass++){
      for(int ipt=0; ipt<ana::nPt; ipt++){
         h2DSignal_D0_Small[imass][ipt] = (TH2D*) f1->Get(Form("hSignal_mass%d_pt%d_smallDCA", imass, ipt));
         h2DBackground_D0_Small[imass][ipt] = (TH2D*) f1->Get(Form("hBackground_mass%d_pt%d_smallDCA", imass, ipt));
         hMass_D0[imass][ipt] = (TH1D*) f1->Get(Form("hMassD0_mass%d_pt%d", imass, ipt));

      etaBin0 = h2DBackground_D0_Small[imass][ipt]->GetXaxis()->FindBin(0.);
      phiBin0 = h2DBackground_D0_Small[imass][ipt]->GetYaxis()->FindBin(0.);
      Content_Background_Deta0_Dphi0[imass][ipt] = h2DBackground_D0_Small[imass][ipt]->GetBinContent(etaBin0, phiBin0);
 
         }
     }

   
   TH1D* h1DSignal_D0_Neg[ana::nMass][ana::nPt];
   TH1D* h1DSignal_D0_Pos[ana::nMass][ana::nPt];
 
   TH1D* h1DBackground_D0_Neg[ana::nMass][ana::nPt];
   TH1D* h1DBackground_D0_Pos[ana::nMass][ana::nPt];
   
   TH1D* hDeltaPhi[ana::nMass][ana::nPt];
   
   //Double_t etaCut[4] = {-4.95,-1.,1.,4.95};
   Double_t etaCut[4] = {-4.95,-2.,2.,4.95};
   Int_t etaBin[4]; 
   for(int ie=0; ie<4; ie++) etaBin[ie] = h2DSignal_D0_Small[0][0]->GetXaxis()->FindBin(etaCut[ie]);
   for(int imass=0; imass<ana::nMass; imass++){
      for(int ipt=0; ipt<ana::nPt; ipt++){
         h1DSignal_D0_Neg[imass][ipt] = h2DSignal_D0_Small[imass][ipt]->ProjectionY(
               Form("h1DSignal_D0_Neg_mass%d_pt%d", imass, ipt), etaBin[0]-1,etaBin[1]-1);
         h1DSignal_D0_Pos[imass][ipt] = h2DSignal_D0_Small[imass][ipt]->ProjectionY(
               Form("h1DSignal_D0_Pos_mass%d_pt%d", imass, ipt), etaBin[2]-1,etaBin[3]-1);
         h1DSignal_D0_Neg[imass][ipt]->Add(h1DSignal_D0_Pos[imass][ipt]); 
         
         h1DBackground_D0_Neg[imass][ipt] = h2DBackground_D0_Small[imass][ipt]->ProjectionY(
               Form("h1DBackground_D0_Neg_mass%d_pt%d", imass, ipt), etaBin[0]-1,etaBin[1]-1);
         h1DBackground_D0_Pos[imass][ipt] = h2DBackground_D0_Small[imass][ipt]->ProjectionY(
               Form("h1DBackground_D0_pos_mass%d_pt%d", imass, ipt), etaBin[2]-1,etaBin[3]-1);
         h1DBackground_D0_Neg[imass][ipt]->Add(h1DBackground_D0_Pos[imass][ipt]);

         //h1DSignal_D0_Neg[imass][ipt]->Sumw2(); 
         //h1DBackground_D0_Neg[imass][ipt]->Sumw2(); 

         h1DSignal_D0_Neg[imass][ipt]->Divide(constant, 1./Content_Background_Deta0_Dphi0[imass][ipt]); 
         h1DSignal_D0_Neg[imass][ipt]->Divide(h1DBackground_D0_Neg[imass][ipt]); 
     }  
   }

   cout << "test1++++++++++++++++"<< endl; 
   TH2D* h2DSignal_D0_Ref = (TH2D*) f2->Get("hSignal_Ref");
   TH2D* h2DBackground_D0_Ref = (TH2D*) f2->Get("hBackground_Ref");

   int etaBin_Ref = h2DBackground_D0_Ref->GetXaxis()->FindBin(0.);
   int phiBin_Ref = h2DBackground_D0_Ref->GetYaxis()->FindBin(0.);
   int Content_Background_Deta0_Dphi0_Ref = h2DBackground_D0_Ref->GetBinContent(etaBin_Ref, phiBin_Ref);
   
   TH1D* h1DSignal_D0_Neg_Ref;
   TH1D* h1DSignal_D0_Pos_Ref;
   
   TH1D* h1DBackground_D0_Neg_Ref; 
   TH1D* h1DBackground_D0_Pos_Ref; 
   
   h1DSignal_D0_Neg_Ref = h2DSignal_D0_Ref->ProjectionY("h1DSignal_D0_Neg_Ref", etaBin[0],etaBin[1]-1);
   h1DSignal_D0_Pos_Ref = h2DSignal_D0_Ref->ProjectionY("h1DSignal_D0_Pos_Ref", etaBin[2]-1,etaBin[3]-1);
   h1DSignal_D0_Neg_Ref->Add(h1DSignal_D0_Pos_Ref);

   h1DBackground_D0_Neg_Ref = h2DBackground_D0_Ref->ProjectionY("h1DBackground_D0_Neg_Ref", etaBin[0],etaBin[1]-1);
   h1DBackground_D0_Pos_Ref = h2DBackground_D0_Ref->ProjectionY("h1DBackground_D0_Neg_Ref", etaBin[2]-1,etaBin[3]-1);
   h1DBackground_D0_Neg_Ref->Add(h1DBackground_D0_Pos_Ref); 
   //h1DSignal_D0_Neg_Ref->Sumw2(); 
   //h1DBackground_D0_Neg_Ref->Sumw2(); 
  
   h1DSignal_D0_Neg_Ref->Divide(constant, 1./Content_Background_Deta0_Dphi0_Ref); 
   h1DSignal_D0_Neg_Ref->Divide(h1DBackground_D0_Neg_Ref); 
   cout << "test2++++++++++++" << endl; 
   
   //Fitting 
   std::string function = "[0]*(1+2*([1]*TMath::Cos(x)+[2]*TMath::Cos(2*x)+[3]*TMath::Cos(3*x)))";

   TCanvas* cMass[14][4];
   char Tfname[128]; 
   char Tfname1[128]; 
   for(int imass=0; imass<ana::nMass; imass++){
      for(int ipt=0; ipt<ana::nPt; ipt++){
         cMass[imass][ipt] = new TCanvas();
         TF1 func("deltaPhi", function.c_str(), -3.14159*0.5, 3.14159*1.5);
         func.SetParameter(0, h1DSignal_D0_Neg[imass][ipt]->GetMaximum());
         func.SetParameter(1, 0.1);
         func.SetParameter(2, 0.1);
         func.SetParameter(3, 0.1);

         h1DSignal_D0_Neg[imass][ipt]->Fit(&func, "L q");
         h1DSignal_D0_Neg[imass][ipt]->Fit(&func, "L q");
         h1DSignal_D0_Neg[imass][ipt]->Fit(&func, "L");
        
         v2_smallDCA[imass][ipt] = func.GetParameter(2);
         v2_smallDCA_err[imass][ipt] = func.GetParError(2);
            
         sprintf(Tfname, "./out_v2/h1DSignal_D0_Neg_%d_%d.png",imass,ipt);
         cMass[imass][ipt]->SaveAs(Tfname); 
         delete cMass[imass][ipt]; 
      }
   }
   cout << "test3++++++++++" << endl; 
   TCanvas* cRef = new TCanvas();
   TF1 func_ref("deltaPhi_Ref", function.c_str(), -3.14159*0.5, 3.14159*1.5);
   func_ref.SetParameter(0, h1DSignal_D0_Neg_Ref->GetMaximum());
   func_ref.SetParameter(1, 0.1);
   func_ref.SetParameter(2, 0.1);
   func_ref.SetParameter(3, 0.1);

   h1DSignal_D0_Neg_Ref->Fit(&func_ref, "L q");
   h1DSignal_D0_Neg_Ref->Fit(&func_ref, "L q");
   h1DSignal_D0_Neg_Ref->Fit(&func_ref, "L");
   
   v2_smallDCA_Ref = func_ref.GetParameter(2); 
   v2_smallDCA_err_Ref = func_ref.GetParError(2); 

   sprintf(Tfname1, "./out_v2/h1DSignal_D0_Neg_Ref_%d_%d.png",0,0);
   cRef->SaveAs(Tfname1); 
   delete cRef; 
   
   TGraphErrors* v2_mass_pt_smallDCA[ana::nPt];
   for(int ipt=0; ipt<ana::nPt; ipt++){
      v2_mass_pt_smallDCA[ipt] = new TGraphErrors(ana::nMass);
      v2_mass_pt_smallDCA[ipt]->SetName(Form("v2_mass_pt_smallDCA_pt%d", ipt));

      for(int imass=0; imass<ana::nMass; imass++){
         double temp = v2_smallDCA[imass][ipt]/sqrt(v2_smallDCA_Ref);
         double temp_err = temp* sqrt(pow(v2_smallDCA_err[imass][ipt]/v2_smallDCA[imass][ipt], 2)
               + pow(0.5*v2_smallDCA_err_Ref/v2_smallDCA_Ref, 2));

         v2_smallDCA[imass][ipt] = temp;
         v2_smallDCA_err[imass][ipt] = temp_err;

         v2_mass_pt_smallDCA[ipt]->SetPoint(imass, hMass_D0[imass][ipt]->GetMean(), v2_smallDCA[imass][ipt]);
         v2_mass_pt_smallDCA[ipt]->SetPointError(imass, 0, v2_smallDCA_err[imass][ipt]);
      }
   }

   TFile f3(Form("out_v2/%s_v2.root", "D0"), "recreate");

   for(int ipt=0; ipt<ana::nPt; ipt++){
      v2_mass_pt_smallDCA[ipt]->Write();
      hMass_pt_small[ipt]->Write();
   }

   return;
}
