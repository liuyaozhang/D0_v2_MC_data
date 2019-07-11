#include "myAnaConsts.h"

using namespace std;
string ana::findDCA(const double& DCA)
{
   bool isLarge = DCA > ana::dcaSep;
   bool isSmall = DCA <= ana::dcaSep;
   if(isLarge) return "smallDCA";
   if(isSmall) return "smallDCA";
   return "";
}

int ana::findMassBin(const double& mass)
{
   for(int imass=0; imass<ana::nMass; imass++){
      if(ana::massbin[imass]<=mass && mass<ana::massbin[imass+1]) return imass;
   }
   return -1;
}

int ana::findPtBin(const double& pT)
{
   for(int ipt=0; ipt<ana::nPt; ipt++){
      if(ana::ptbin[ipt]<=pT && pT<ana::ptbin[ipt+1]) return ipt;
   }
   return -1;
}

int ana::findYBin(const double& y)
{
   for(int iy=0; iy<ana::nY; iy++){
      if(ana::ybin[iy]<=fabs(y) && fabs(y)<ybin[iy+1]) return iy;
   }
   return -1;
}
