#include <iostream>

#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TString.h"

#include <vector>

#include "Fit/Fitter.h"
#include "Fit/BinData.h"
#include "Fit/Chi2FCN.h"
#include "TList.h"
#include "Math/WrappedMultiTF1.h"
#include "HFitInterface.h"

int iparmassfit_poly3bkg_floatwidth[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
int iparvnfit_poly3bkg_floatwidth[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

struct GlobalChi2_poly3bkg_floatwidth {
    GlobalChi2_poly3bkg_floatwidth(  ROOT::Math::IMultiGenFunction & f1,
                                   ROOT::Math::IMultiGenFunction & f2) :
    fChi2_1(&f1), fChi2_2(&f2) {}
    
    // parameter vector is first background (in common 1 and 2)
    // and then is signal (only in 2)
    double operator() (const double *par) const {
        double p1[13];
        for(int i = 0; i < 13; ++i) p1[i] = par[iparmassfit_poly3bkg_floatwidth[i]];
        
        double p2[16];
        for(int i = 0; i < 16; ++i) p2[i] = par[iparvnfit_poly3bkg_floatwidth[i]];
        
        return (*fChi2_1)(p1) + (*fChi2_2)(p2);
    }
    
    const  ROOT::Math::IMultiGenFunction * fChi2_1;
    const  ROOT::Math::IMultiGenFunction * fChi2_2;
};

void massfitvn_combine_d0()
{   
    
    gStyle->SetOptStat(0); 

    double fit_range_low = 1.7;
    double fit_range_high = 2.0;
    double D0_mass = 1.8648;
    TFile* file0 = TFile::Open("./MC/hMC_mass_d0ana_y2_new.root");
    TFile* file1 = TFile::Open("/home/lz55/FlowCorr/proj1D/out_v2/D0_v2_MultiplicityMT100LT250_absRapidityMT0LT2_DetaMT1_new.root");
    
    TFile ofile("./v2vs_pt_and_fitting/v2vspt_fromfit_d0_HM100_250_etagap1_new.root","RECREATE");
    
    TF1* fmasssig[9];
    TF1* fmassswap[9];
    TF1* fmassbkg[9];
    TF1* fmasstotal[9];
    TF1* fvn[9];
    
     cout << "test0++++++++++++++++" << endl; 
 //   double pt[13];
    double KET_ncq[13];
    double v2[13];
    double v2e[13];
    double v2_bkg[13];
    double v2_ncq[13];
    double v2e_ncq[13];
    double ptbin[5] = {1.5, 3., 5., 7., 10.};
    double a[13];
    double b[13];
    double sigfrac[13];
    
    TCanvas* c[4];
    for(int i=0;i<4;i++)
    {
        c[i] = new TCanvas(Form("c_%d",i),Form("c_%d",i),800,400);
        c[i]->Divide(2,1);
    }
    
    for(int i=0;i<4;i++)
    {
        c[i]->cd(1)->SetTopMargin(0.06);
        c[i]->cd(1)->SetLeftMargin(0.18);
        c[i]->cd(1)->SetRightMargin(0.043);
        c[i]->cd(1)->SetBottomMargin(0.145);
        c[i]->cd(2)->SetTopMargin(0.06);
        c[i]->cd(2)->SetLeftMargin(0.18);
        c[i]->cd(2)->SetRightMargin(0.043);
        c[i]->cd(2)->SetBottomMargin(0.145);

    }
    
    TCanvas* c2 = new TCanvas("c2","c2",100,100);
    
    TLatex* tex = new TLatex;
    tex->SetNDC();
    tex->SetTextFont(42);
    tex->SetTextSize(0.045);
    tex->SetLineWidth(2);
 
    TLatex* texCMS = new TLatex;
    texCMS->SetNDC();
    texCMS->SetTextFont(42);
    texCMS->SetTextSize(0.05);
    texCMS->SetTextAlign(12);
    
    TH1D* hist = new TH1D("hist","",10,1.7,2.0);
    hist->SetLineWidth(0);
    //hist->GetYaxis()->SetRangeUser(0,0.3);
    hist->GetXaxis()->SetTitle("m_{#piK} (GeV/c^{2})");
    hist->GetYaxis()->SetTitle("v_{2}");
    hist->GetXaxis()->CenterTitle();
    hist->GetYaxis()->CenterTitle();
    hist->GetXaxis()->SetTitleOffset(1.3);
    hist->GetYaxis()->SetTitleOffset(2);
    hist->GetXaxis()->SetLabelOffset(0.007);
    hist->GetYaxis()->SetLabelOffset(0.007);
    hist->GetXaxis()->SetTitleSize(0.045);
    hist->GetYaxis()->SetTitleSize(0.045);
    hist->GetXaxis()->SetTitleFont(42);
    hist->GetYaxis()->SetTitleFont(42);
    hist->GetXaxis()->SetLabelFont(42);
    hist->GetYaxis()->SetLabelFont(42);
    hist->GetXaxis()->SetLabelSize(0.04);
    hist->GetYaxis()->SetLabelSize(0.04);
    hist->SetMinimum(0.05);
    hist->SetMaximum(0.45);
    
    c2->cd();
    hist->Draw();
     cout << "test1++++++++++++++++" << endl; 
    for(int i=0;i<4;i++)
    {
        TH1D* h_mc_match_signal = (TH1D*)file0->Get(Form("hMC_mass_small_pt%d",i));
        TH1D* h_mc_match_all = (TH1D*)file0->Get(Form("hMC_mass_all_small_pt%d",i));
        TH1D* h_data = (TH1D*)file1->Get(Form("h1DMass_pt%d",i));
        
        h_data->SetMinimum(0);
        h_data->SetMarkerSize(0.8);
        h_data->SetMarkerStyle(20);
        h_data->SetLineWidth(1);
        h_data->SetOption("e");
        h_data->GetXaxis()->SetRangeUser(1.7,2);
        h_data->GetXaxis()->SetTitle("m_{#piK} (GeV/c^{2})");
        h_data->GetYaxis()->SetTitle("Entries / 5 MeV");
        h_data->GetXaxis()->CenterTitle();
        h_data->GetYaxis()->CenterTitle();
        h_data->GetXaxis()->SetTitleOffset(1.3);
        h_data->GetYaxis()->SetTitleOffset(2);
        h_data->GetXaxis()->SetLabelOffset(0.007);
        h_data->GetYaxis()->SetLabelOffset(0.007);
        h_data->GetXaxis()->SetTitleSize(0.045);
        h_data->GetYaxis()->SetTitleSize(0.045);
        h_data->GetXaxis()->SetTitleFont(42);
        h_data->GetYaxis()->SetTitleFont(42);
        h_data->GetXaxis()->SetLabelFont(42);
        h_data->GetYaxis()->SetLabelFont(42);
        h_data->GetXaxis()->SetLabelSize(0.04);
        h_data->GetYaxis()->SetLabelSize(0.04);
        
        h_data->GetXaxis()->SetNoExponent(true);
        ((TGaxis*)h_data->GetXaxis())->SetMaxDigits(7);
        
        h_data->SetMaximum(h_data->GetMaximum()*1.5);
        
        //TH1D* h_pt = (TH1D*)file1->Get(Form("PtD0_pt%d",i));
        //TH1D* h_KET = (TH1D*)file1->Get(Form("KETD0_pt%d",i));
        //pt[i] = h_pt->GetMean();
        //KET_ncq[i] = h_KET->GetMean()/2.0;

        c[i]->cd(1);
        /*The full fitting function is constructed as follow
         [0] is signal + swap yield;
         [1] is common mean of double gaussian;
         [2] is signal gaussian 1 sigma;
         [3] is signal gaussian 2 sigma;
         [4] is fractional signal gaussian 1 yield; 1-[4] is fractional signal gaussian 2 yield;
         [5] is fractional double gaussian signal yield, 1-[5] is fractional swap yield;
         [6] is a factor to let width of the gaussians to vary in data;
         [7] is swap gaussian sigma;
         [8] is swap gaussian mean;
         [9-12] is 3rd order poly parameters
         */
        
        TF1* f = new TF1(Form("f_%d",i),"[0]*([5]*([4]*TMath::Gaus(x,[1],[2]*(1.0 +[6]))/(sqrt(2*3.14159)*[2]*(1.0 +[6]))+(1-[4])*TMath::Gaus(x,[1],[3]*(1.0 +[6]))/(sqrt(2*3.14159)*[3]*(1.0 +[6])))+(1-[5])*TMath::Gaus(x,[8],[7]*(1.0 +[6]))/(sqrt(2*3.14159)*[7]*(1.0 +[6]))) + [9] + [10]*x + [11]*x*x + [12]*x*x*x", fit_range_low, fit_range_high);
        f->SetLineColor(2);
        f->SetLineWidth(1);

        //first fit MC signal, swap and poly bkg set to 0
         
        cout << "test1_1++++++++++++++++" << endl; 
        f->SetParameter(0,100.);
        f->SetParameter(1,D0_mass);
        f->SetParameter(2,0.03);
        f->SetParameter(3,0.005);
        f->SetParameter(4,0.1);
        
        f->FixParameter(5,1);
        f->FixParameter(6,0); //always 0 in MC
        f->FixParameter(7,0.1); //does not really mater here as yield is fix to 0
        f->FixParameter(8,D0_mass); //does not really mater here as yield is fix to 0
        f->FixParameter(9,0);
        f->FixParameter(10,0);
        f->FixParameter(11,0);
        f->FixParameter(12,0);
        
        f->SetParLimits(2,0.01,0.1);
        f->SetParLimits(3,0.001,0.05);
        f->SetParLimits(4,0,1);
        f->SetParLimits(5,0,1);
        
        f->FixParameter(1,1.8648); //for first few attempt fix mean of gaussian to get reasonable estimation of other pars; later open it up
        h_mc_match_signal->Fit(Form("f_%d",i),"q","",fit_range_low,fit_range_high);
        h_mc_match_signal->Fit(Form("f_%d",i),"q","",fit_range_low,fit_range_high);
        f->ReleaseParameter(1); //now let gaussian mean float
        h_mc_match_signal->Fit(Form("f_%d",i),"L q","",fit_range_low,fit_range_high);
        h_mc_match_signal->Fit(Form("f_%d",i),"L q","",fit_range_low,fit_range_high);
        h_mc_match_signal->Fit(Form("f_%d",i),"L m","",fit_range_low,fit_range_high);
        cout << "test1_2++++++++++++++++" << endl; 
        
        //now fix signal double gaussian mean, sigma and gaus1,gaus2 yield ratio
        f->FixParameter(1,f->GetParameter(1));
        f->FixParameter(2,f->GetParameter(2));
        f->FixParameter(3,f->GetParameter(3));
        f->FixParameter(4,f->GetParameter(4));
        
        //now release swap bkg parameters to fit signal+swap MC
        f->ReleaseParameter(5);
        f->ReleaseParameter(7);
        f->ReleaseParameter(8);
        
        f->SetParameter(7,0.1);
        f->SetParameter(8,D0_mass);
        
        //fit signal+swap MC
        h_mc_match_all->Fit(Form("f_%d",i),"L q","",fit_range_low,fit_range_high);
        h_mc_match_all->Fit(Form("f_%d",i),"L q","",fit_range_low,fit_range_high);
        h_mc_match_all->Fit(Form("f_%d",i),"L q","",fit_range_low,fit_range_high);
        h_mc_match_all->Fit(Form("f_%d",i),"L q","",fit_range_low,fit_range_high);
        h_mc_match_all->Fit(Form("f_%d",i),"L m","",fit_range_low,fit_range_high);
        
        //now fix swap bkg parameters to fit data
        f->FixParameter(5,f->GetParameter(5));
        f->FixParameter(7,f->GetParameter(7));
        f->FixParameter(8,f->GetParameter(8));
        
        //now release poly bkg pars
        f->ReleaseParameter(9);
        f->ReleaseParameter(10);
        f->ReleaseParameter(11);
        f->ReleaseParameter(12);
        
        cout << "test1_3++++++++++++++++" << endl; 
        //now fit data
        h_data->Fit(Form("f_%d",i),"q","",fit_range_low,fit_range_high);
        h_data->Fit(Form("f_%d",i),"q","",fit_range_low,fit_range_high);
        f->ReleaseParameter(1); //allow data to have different mass peak mean than MC
        f->ReleaseParameter(6); //allow data to have different peak width than MC
        f->SetParameter(6,0);
        f->SetParLimits(6,-1,1);
        //f->FixParameter(5,1);
        h_data->Fit(Form("f_%d",i),"L q","",fit_range_low,fit_range_high);
        h_data->Fit(Form("f_%d",i),"L q","",fit_range_low,fit_range_high);
        h_data->Fit(Form("f_%d",i),"L q","",fit_range_low,fit_range_high);
        h_data->Fit(Form("f_%d",i),"L m","",fit_range_low,fit_range_high);
        
        cout << "test1_4++++++++++++++++" << endl; 
        //draw D0 signal separately
        TF1* f1 = new TF1(Form("f_sig_%d",i),"[0]*([5]*([4]*TMath::Gaus(x,[1],[2]*(1.0 +[6]))/(sqrt(2*3.14159)*[2]*(1.0 +[6]))+(1-[4])*TMath::Gaus(x,[1],[3]*(1.0 +[6]))/(sqrt(2*3.14159)*[3]*(1.0 +[6]))))", fit_range_low, fit_range_high);
        f1->SetLineColor(kOrange-3);
        f1->SetLineWidth(1);
        f1->SetLineStyle(2);
        f1->SetFillColorAlpha(kOrange-3,0.3);
        f1->SetFillStyle(1001);
        f1->FixParameter(0,f->GetParameter(0));
        f1->FixParameter(1,f->GetParameter(1));
        f1->FixParameter(2,f->GetParameter(2));
        f1->FixParameter(3,f->GetParameter(3));
        f1->FixParameter(4,f->GetParameter(4));
        f1->FixParameter(5,f->GetParameter(5));
        f1->FixParameter(6,f->GetParameter(6));
        
        cout << "test1_5++++++++++++++++" << endl; 
        fmasssig[i] = (TF1*)f1->Clone();
        fmasssig[i]->SetName(Form("masssigfcn_pt%d",i));
        fmasssig[i]->Write();
        
        f1->Draw("LSAME");
        cout << "test1_6++++++++++++++++" << endl; 
        
        //draw swap bkg separately
        TF1* f2 = new TF1(Form("f_swap_%d",i),"[0]*((1-[5])*TMath::Gaus(x,[8],[7]*(1.0 +[6]))/(sqrt(2*3.14159)*[7]*(1.0 +[6])))", fit_range_low, fit_range_high);
        f2->SetLineColor(kGreen+4);
        f2->SetLineWidth(1);
        f2->SetLineStyle(1);
        f2->SetFillColorAlpha(kGreen+4,0.3);
        f2->SetFillStyle(1001);
        f2->FixParameter(0,f->GetParameter(0));
        f2->FixParameter(5,f->GetParameter(5));
        f2->FixParameter(6,f->GetParameter(6));
        f2->FixParameter(7,f->GetParameter(7));
        f2->FixParameter(8,f->GetParameter(8));
        cout << "test1_7++++++++++++++++" << endl; 
        
        fmassswap[i] = (TF1*)f2->Clone();
        fmassswap[i]->SetName(Form("massswapfcn_pt%d",i));
        fmassswap[i]->Write();
        
        f2->Draw("LSAME");
        
        //draw poly bkg separately
        TF1* f3 = new TF1(Form("f_bkg_%d",i),"[9] + [10]*x + [11]*x*x + [12]*x*x*x", fit_range_low, fit_range_high);
        f3->SetLineColor(4);
        f3->SetLineWidth(1);
        f3->SetLineStyle(2);
        f3->FixParameter(9,f->GetParameter(9));
        f3->FixParameter(10,f->GetParameter(10));
        f3->FixParameter(11,f->GetParameter(11));
        f3->FixParameter(12,f->GetParameter(12));
        
        fmassbkg[i] = (TF1*)f3->Clone();
        fmassbkg[i]->SetName(Form("massbkgfcn_pt%d",i));
        fmassbkg[i]->Write();
        
        cout << "test1_8++++++++++++++++" << endl; 
        f3->Draw("LSAME");
        
        tex->DrawLatex(0.22,0.86,"100 #leq N_{trk}^{offline} < 250");
        tex->DrawLatex(0.22,0.80,Form("%.1f < p_{T} < %.1f GeV/c",ptbin[i],ptbin[i+1]));
        tex->DrawLatex(0.22,0.74,"|y| < 2");
        
        texCMS->DrawLatex(.18,.97,"#font[61]{CMS} #it{Preliminary}");
        texCMS->DrawLatex(0.62,0.97, "#scale[0.8]{pp #sqrt{s_{NN}} = 13 TeV}");
        
        TLegend* leg = new TLegend(0.65,0.58,0.81,0.9,NULL,"brNDC");
        leg->SetBorderSize(0);
        leg->SetTextSize(0.045);
        leg->SetTextFont(42);
        leg->SetFillStyle(0);
        leg->AddEntry(h_data,"data","p");
        leg->AddEntry(f,"Fit","L");
        leg->AddEntry(f1,"D^{0}+#bar{D^{#lower[0.2]{0}}} Signal","f");
        leg->AddEntry(f2,"K-#pi swap","f");
        leg->AddEntry(f3,"Combinatorial","l");
        leg->Draw("SAME");
        
        sigfrac[i] = (f1->Integral(1.85,1.88) + f2->Integral(1.85,1.88))/f->Integral(1.85,1.88);
        cout << "test1_9++++++++++++++++" << endl; 
        
        //fit vn
        //[13] is vn_sig
        //[14-15] is vn bkg, const + linear vn(pT)
        TGraphErrors* vn_data = (TGraphErrors*)file1->Get(Form("v2_mass_pt_smallDCA_pt%d",i));
        
        c[i]->cd(2);
        
        hist->Draw();
        
        TF1* fmass_combinemassvnfit = new TF1(Form("fmass_combinemassvnfit_%d",i),"[0]*([5]*([4]*TMath::Gaus(x,[1],[2]*(1.0 +[6]))/(sqrt(2*3.14159)*[2]*(1.0 +[6]))+(1-[4])*TMath::Gaus(x,[1],[3]*(1.0 +[6]))/(sqrt(2*3.14159)*[3]*(1.0 +[6])))+(1-[5])*TMath::Gaus(x,[8],[7]*(1.0 +[6]))/(sqrt(2*3.14159)*[7]*(1.0 +[6]))) + [9] + [10]*x + [11]*x*x + [12]*x*x*x", fit_range_low, fit_range_high);
        
        TF1* fvn_combinemassvnfit = new TF1(Form("fvn_combinemassvnfit_%d",i), "( ( [0]*([5]*([4]*TMath::Gaus(x,[1],[2]*(1.0 +[6]))/(sqrt(2*3.14159)*[2]*(1.0 +[6]))+(1-[4])*TMath::Gaus(x,[1],[3]*(1.0 +[6]))/(sqrt(2*3.14159)*[3]*(1.0 +[6])))+(1-[5])*TMath::Gaus(x,[8],[7]*(1.0 +[6]))/(sqrt(2*3.14159)*[7]*(1.0 +[6]))) ) / ( [0]*([5]*([4]*TMath::Gaus(x,[1],[2]*(1.0 +[6]))/(sqrt(2*3.14159)*[2]*(1.0 +[6]))+(1-[4])*TMath::Gaus(x,[1],[3]*(1.0 +[6]))/(sqrt(2*3.14159)*[3]*(1.0 +[6])))+(1-[5])*TMath::Gaus(x,[8],[7]*(1.0 +[6]))/(sqrt(2*3.14159)*[7]*(1.0 +[6]))) + [9] + [10]*x + [11]*x*x + [12]*x*x*x ) ) * [13] + ( 1.0 - ( ( [0]*([5]*([4]*TMath::Gaus(x,[1],[2]*(1.0 +[6]))/(sqrt(2*3.14159)*[2]*(1.0 +[6]))+(1-[4])*TMath::Gaus(x,[1],[3]*(1.0 +[6]))/(sqrt(2*3.14159)*[3]*(1.0 +[6])))+(1-[5])*TMath::Gaus(x,[8],[7]*(1.0 +[6]))/(sqrt(2*3.14159)*[7]*(1.0 +[6]))) ) / ( [0]*([5]*([4]*TMath::Gaus(x,[1],[2]*(1.0 +[6]))/(sqrt(2*3.14159)*[2]*(1.0 +[6]))+(1-[4])*TMath::Gaus(x,[1],[3]*(1.0 +[6]))/(sqrt(2*3.14159)*[3]*(1.0 +[6])))+(1-[5])*TMath::Gaus(x,[8],[7]*(1.0 +[6]))/(sqrt(2*3.14159)*[7]*(1.0 +[6]))) + [9] + [10]*x + [11]*x*x + [12]*x*x*x ) ) ) * ( [14] + [15] * x)", fit_range_low, fit_range_high);
        
        fmass_combinemassvnfit->SetLineColor(2);
        fmass_combinemassvnfit->SetLineWidth(1);
        
        fvn_combinemassvnfit->SetLineColor(2);
        fvn_combinemassvnfit->SetLineWidth(1);

        ROOT::Math::WrappedMultiTF1 wfmass_combinemassvnfit(*fmass_combinemassvnfit,1);
        ROOT::Math::WrappedMultiTF1 wfvn_combinemassvnfit(*fvn_combinemassvnfit,1);
        
        ROOT::Fit::DataOptions opt;
        ROOT::Fit::DataRange range_massfit;

        range_massfit.SetRange(fit_range_low,fit_range_high);
        ROOT::Fit::BinData datamass(opt,range_massfit);
        ROOT::Fit::FillData(datamass, h_data);
        
        ROOT::Fit::DataRange range_vnfit;
        range_vnfit.SetRange(fit_range_low,fit_range_high);
        ROOT::Fit::BinData datavn(opt,range_vnfit);
        ROOT::Fit::FillData(datavn, vn_data);
        
        ROOT::Fit::Chi2Function chi2_B(datamass, wfmass_combinemassvnfit);
        ROOT::Fit::Chi2Function chi2_SB(datavn, wfvn_combinemassvnfit);
        
        GlobalChi2_poly3bkg_floatwidth globalChi2(chi2_B, chi2_SB);

        ROOT::Fit::Fitter fitter;
        
        const int Npar = 16;
        double par0[Npar];
        for( int ipar = 0; ipar < f->GetNpar(); ipar++ ) par0[ipar] = f->GetParameter(ipar);
        par0[13] = 0.01;
        par0[14] = 0.10;
        par0[15] = 0.05;
        
        cout << "test1_10++++++++++++++++" << endl; 
        
        fitter.Config().SetParamsSettings(Npar,par0);
        // fix parameter
        fitter.Config().ParSettings(2).Fix();
        fitter.Config().ParSettings(3).Fix();
        fitter.Config().ParSettings(4).Fix();
        fitter.Config().ParSettings(5).Fix();
        fitter.Config().ParSettings(7).Fix();
        fitter.Config().ParSettings(8).Fix();

        fitter.Config().ParSettings(1).SetLimits(1.7, 2.0);

        fitter.Config().MinimizerOptions().SetPrintLevel(0);
        fitter.Config().SetMinimizer("Minuit2","Migrad");

        fitter.FitFCN(Npar,globalChi2,0,datamass.Size()+datavn.Size(),true);
        ROOT::Fit::FitResult result = fitter.Result();
        result.Print(std::cout);
        
        fmass_combinemassvnfit->SetFitResult( result, iparmassfit_poly3bkg_floatwidth);
        fmass_combinemassvnfit->SetRange(range_massfit().first, range_massfit().second);
        fmass_combinemassvnfit->SetLineColor(kRed);
        h_data->GetListOfFunctions()->Add(fmass_combinemassvnfit);
        
        fvn_combinemassvnfit->SetFitResult( result, iparvnfit_poly3bkg_floatwidth);
        fvn_combinemassvnfit->SetRange(range_vnfit().first, range_vnfit().second);
        fvn_combinemassvnfit->SetLineColor(2);
        //fvn_combinemassvnfit->SetLineStyle(2);
        //vn_data->GetListOfFunctions()->Add(fvn_combinemassvnfit);
        vn_data->SetTitle("");
        vn_data->SetMarkerSize(0.8);
        vn_data->SetMarkerStyle(20);
        vn_data->SetLineWidth(1);
        vn_data->Draw("PESAME");
        
        fvn[i] = (TF1*)fvn_combinemassvnfit->Clone();
        fvn[i]->SetName(Form("vnfit_pt%d",i));
        fvn[i]->Draw("LSAME");
        //fvn[i]->Write();
        
        fmasstotal[i] = (TF1*)fmass_combinemassvnfit->Clone();
        fmasstotal[i]->SetName(Form("masstotalfcn_pt%d",i));
        fmasstotal[i]->Write();
        
        tex->DrawLatex(0.22,0.86,"100 #leq N_{trk}^{offline} < 250");
        tex->DrawLatex(0.22,0.80,Form("%.1f < p_{T} < %.1f GeV/c",ptbin[i],ptbin[i+1]));
        tex->DrawLatex(0.22,0.74,"|y| < 2, |#Delta#eta| > 1");
        //tex->DrawLatex(0.22,0.68,"|#Delta#eta| > 2");

        
        texCMS->DrawLatex(.18,.97,"#font[61]{CMS} #it{Preliminary}");
        texCMS->DrawLatex(0.62,0.97, "#scale[0.8]{pp #sqrt{s_{NN}} = 13 TeV}");
        
        v2[i] = fvn_combinemassvnfit->GetParameter(13);
        v2e[i] = fvn_combinemassvnfit->GetParError(13);
        v2_bkg[i] = fvn_combinemassvnfit->GetParameter(14) + fvn_combinemassvnfit->GetParameter(15) * 1.864;
        v2_ncq[i] = v2[i]/2.0;
        v2e_ncq[i] = v2e[i]/2.0;
        a[i] = fvn_combinemassvnfit->GetParameter(14);
        b[i] = fvn_combinemassvnfit->GetParameter(15);
        
        TF1* fvnbkg = new TF1(Form("fvnbkg_%d",1),"( [0] + [1] * x)", fit_range_low, fit_range_high);
        fvnbkg->FixParameter(0,fvn_combinemassvnfit->GetParameter(14));
        fvnbkg->FixParameter(1,fvn_combinemassvnfit->GetParameter(15));
        
        fvnbkg->SetName(Form("fvnbkg_fcn_pt%d",i));
        fvnbkg->Write();
        
        fvnbkg->SetLineStyle(7);
        //fvnbkg->Draw("LSAME");
        
     cout << "test2++++++++++++++++" << endl; 
        TLegend* leg1 = new TLegend(0.65,0.78,0.95,0.9,NULL,"brNDC");
        leg1->SetBorderSize(0);
        leg1->SetTextSize(0.045);
        leg1->SetTextFont(42);
        leg1->SetFillStyle(0);
        leg1->AddEntry(h_data,"data","p");
        //leg1->AddEntry(fvnbkg,"v_{2}^{bkg}","L");
//        leg1->AddEntry(f1,"D^{0}+#bar{D^{#lower[0.2]{0}}} Signal","f");
//        leg1->AddEntry(f2,"K-#pi swap","f");
//        leg1->AddEntry(f3,"Combinatorial","l");
        //leg1->Draw("SAME");

        
        TF1* falpha = new TF1(Form("falpha_%d",1),"( [0]*([5]*([4]*TMath::Gaus(x,[1],[2]*(1.0 +[6]))/(sqrt(2*3.14159)*[2]*(1.0 +[6]))+(1-[4])*TMath::Gaus(x,[1],[3]*(1.0 +[6]))/(sqrt(2*3.14159)*[3]*(1.0 +[6])))+(1-[5])*TMath::Gaus(x,[8],[7]*(1.0 +[6]))/(sqrt(2*3.14159)*[7]*(1.0 +[6]))) )/( [0]*([5]*([4]*TMath::Gaus(x,[1],[2]*(1.0 +[6]))/(sqrt(2*3.14159)*[2]*(1.0 +[6]))+(1-[4])*TMath::Gaus(x,[1],[3]*(1.0 +[6]))/(sqrt(2*3.14159)*[3]*(1.0 +[6])))+(1-[5])*TMath::Gaus(x,[8],[7]*(1.0 +[6]))/(sqrt(2*3.14159)*[7]*(1.0 +[6]))) + [9] + [10]*x + [11]*x*x + [12]*x*x*x )", fit_range_low,fit_range_high);
        
        for(int j=0;j<13;j++)
        {
            falpha->FixParameter(j,fmass_combinemassvnfit->GetParameter(j));
        }
        
        falpha->SetName(Form("sigfrac_fcn_pt%d",i));
        falpha->Write();
        
        double xmass[200];
        double pullmass[200];
        
        float Chi2=0;
        int ndf = 0.3/0.005 - 11;
        
        for(int k=0;k<h_data->GetNbinsX();k++)
        {
            xmass[k] = h_data->GetBinCenter(k);
            pullmass[k] = (h_data->GetBinContent(k) - fmass_combinemassvnfit->Eval(xmass[k]))/h_data->GetBinError(k);
            if(fabs(pullmass[k])<5)
            {
                //cout<<pullmass[k]<<endl;
                Chi2 += pullmass[k]*pullmass[k];
            }
        }

        c[i]->cd(1);
        tex->DrawLatex(0.22,0.68,Form("Chi2/ndf = %.0f/%d",Chi2,ndf));
        
        double xv2[200];
        double pullv2[200];
        double v2y[200];
        
        float Chi2v2=0;
        int ndfv2 = 14 - 2;
        
        for(int k=0;k<vn_data->GetN();k++)
        {
            vn_data->GetPoint(k,xv2[k],v2y[k]);
            //xv2[k] = vn_dara->GetBinCenter(k);
            pullv2[k] = (v2y[k] - fvn_combinemassvnfit->Eval(xv2[k]))/vn_data->GetErrorY(k);
            cout<<pullv2[k]<<endl;
            if(fabs(pullv2[k])<100)
            {
                //cout<<pullmass[k]<<endl;
                Chi2v2 += pullv2[k]*pullv2[k];
            }
        }

        c[i]->cd(2);
        tex->DrawLatex(0.22,0.68,Form("Chi2/ndf = %.0f/%d",Chi2v2,ndfv2));
        
    }
    
    for(int i=0;i<4;i++)
    {
        c[i]->Print(Form("plots/D0_mass_vnfit_combine_pt%d.png",i));
    }
    
    const double pt[] = {2.25, 4., 6., 8.5}; 
    const double pt_binWidth[] = {0.75, 1.0, 1.0, 1.5 };   
    
    TGraphErrors* v2plot = new TGraphErrors(4,pt, v2, pt_binWidth, v2e);
    v2plot->GetXaxis()->SetTitle("p_{T}(GeV/c)"); 
    v2plot->GetYaxis()->SetTitle("v2"); 
    v2plot->GetXaxis()->SetRangeUser(1.,10.);
    v2plot->Write("v2plot_small");
    /*
    TGraphErrors* v2ncqplot = new TGraphErrors(9,KET_ncq,v2_ncq,0,v2e_ncq);
    TGraphErrors* v2bkgplot = new TGraphErrors(9,pt,v2_bkg,0,0);
    
    v2plot->SetName("v2vspt");
    v2ncqplot->SetName("v2vsKET_ncq");
    v2bkgplot->SetName("v2bkgvspt");
    
    v2plot->Write();
    v2ncqplot->Write();
    v2bkgplot->Write();
    */
}
