#include "HZZ2L2QRooPdfs.cc"
#include "jointPoly.cc"

double crystalball_function(double x, double alpha, double n, double sigma, double mean) {
	if (sigma < 0.)     return 0.;
	double z = (x - mean)/sigma;
	if (alpha < 0) z = -z;
	double abs_alpha = std::abs(alpha);
	if (z  > - abs_alpha)
		return std::exp(- 0.5 * z * z);
	else {
		double nDivAlpha = n/abs_alpha;
		double AA =  std::exp(-0.5*abs_alpha*abs_alpha);
		double B = nDivAlpha -abs_alpha;
		double arg = nDivAlpha/(B-z);
		return AA * std::pow(arg,n);
	}
}


//Efficiency fitting function for resolved jets
double merged_fit(Double_t *xx, Double_t *par){

	Double_t temp = 1700;  //quadratic spline
	Double_t x =xx[0], y=(x-temp)/1000.;
	Double_t f;

	if(x<=temp) f = par[0]*TMath::Erf( (x-par[1])/par[2] ) + (par[3] + par[4]*TMath::ATan((x-par[5])/par[6]));
	else {f = merged_fit(&temp, par) + par[7]*y+par[8]*y*y; }
	return f;

}

//Efficiency fitting function for resolved jets
double resolved_fit(Double_t *xx, Double_t *par){

	Float_t x =xx[0];
	Double_t f = par[8]*TMath::Gaus(x,par[4], par[10]) + TMath::ATan((x-par[9])/par[11])*par[0]*crystalball_function(x, par[1], par[2], par[3], par[4])+par[5]*TMath::Erf((x-par[6])/par[7]);
	return f;

}

//c-constant for D_vbf
float getDVBF2jetsConstant(float ZZMass){
	float par[9]={1.876,-55.488,403.32,0.3906,80.8,27.7,-0.06,54.97,309.96};
	float kappa =
		pow(1.-atan((ZZMass-par[1])/par[2])*2./TMath::Pi(), par[0])
		+ par[3]*exp(-pow((ZZMass-par[4])/par[5], 2))
		+ par[6]*exp(-pow((ZZMass-par[7])/par[8], 2));
	float constant = kappa/(1.-kappa);
	return constant;
}
