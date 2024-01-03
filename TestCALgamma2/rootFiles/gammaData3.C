#define _USE_MATH_DEFINES
#include <math.h>

void gammaData3()
{
//Setup of CSV Data file and Canvases
	int Diameter = 8; //Diameters are either 5 or 8 cm
	int DistFromOrigin = 20; //Distance between Detector and Source is 20 cm
	float pi = M_PI; //Define pi
	cout << "pi is: " << pi << endl;

	ofstream myfile;
        myfile.open("gammaData3File.csv");
        myfile << "Energy, AbsEff, IntEff, SolidAngle, PredIntEff, \n";
	
	TCanvas *c1 = new TCanvas();
        c1->SetTickx();
        c1->SetTicky();
        c1->SetGridx();
        c1->SetGridy();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Setup & Start of ROOT File loop
//Loop over gamma raw data

	int NoEvt = 1000000;
	int NoBin = 60000;
	int yHeight = 75; //For display purposes (canvas) only

	int iter = 0;
	double Energy = 0.0;
	while (iter <= 21)
	{
		if(iter==0)
		{Energy = 0.1;}
		else if(iter==21)
		{Energy = 0.478;}
		else
		{Energy = iter * 0.25;}

		double ComptonEdge = Energy / (1 + 0.511 / (2 * Energy));
		double PhotopeakLower = Energy - 0.02;
		double PhotopeakUpper = Energy + 0.02;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Bin the contents of the gamma raw data

		TString filename = "TestCAL" + to_string(iter) + ".root";

		TFile *input1 = new TFile(filename, "read");

        	TTree *tree1 =  (TTree*)input1->Get("Edep");
        	TTree *tree3 =  (TTree*)input1->Get("Incident");

		double fEdep_HPGe;
		int Incident_Gammas;

		tree1->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe);
        	tree3->SetBranchAddress("Incident_Gammas", &Incident_Gammas);

        	int entries1 = tree1->GetEntries();
	       	int entries3 = tree3->GetEntries();

		TString title = to_string(Energy) + " MeV Gamma HPGe Energy Deposition";// (D = "+Diameter+"cm, L = "+Length+"cm)";

        	TH1F *hist1 = new TH1F("hist1", title, NoBin, 0 ,Energy+0.25);
        	for(int i = 0; i < entries1; i++)
        	{
        	        tree1->GetEntry(i);
        	        hist1->Fill(fEdep_HPGe);
        	}
		hist1->SetLineColorAlpha(kBlue, 0.35);
        	hist1->GetXaxis()->SetTitle("Energy (MeV)");
        	hist1->GetYaxis()->SetTitle("Counts");
        	hist1->GetYaxis()->SetRangeUser(0, yHeight);
		hist1->SetStats(0);
		hist1->Draw();

        	TH1F *hist3 = new TH1F("hist3", "", 2, 0 ,2);
        	for(int i = 0; i < entries3; i++)
        	{
        	        tree3->GetEntry(i);
        	        hist3->Fill(Incident_Gammas);
        	}


		TLegend *leg = new TLegend(0.6, 0.7, 0.85, 0.8);
        	leg->SetBorderSize(0);
        	leg->AddEntry(hist1, "Deposited Energy", "l");
		leg->Draw();

        	TLine *l1 = new TLine(ComptonEdge,0,ComptonEdge,yHeight);
        	l1->SetLineWidth(2);
        	l1->SetLineStyle(9);
        	l1->SetLineColor(kBlack);
		leg->AddEntry(l1, "Compton Edge", "l");
        	l1->Draw();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Obtain Statistics from gamma raw data
//Statistics Needed: "Gammas_Peak, Gammas_Emit, Gammas_Inc"

		//Gammas_Peak
		float Gammas_Peak = hist1->Integral(hist1->FindFixBin(PhotopeakLower), hist1->FindFixBin(PhotopeakUpper));
		//Gammas_Emit
		float Gammas_Emit = hist3->GetEntries();

		//Gammas_Inc
		float Gammas_Inc = hist3->GetMean() * Gammas_Emit;
		
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//"Energy, AbsEff, IntEff, SolidAngle, PredIntEff"
		
		//Absolute Efficiency "AbsEff"
		float AbsEff = Gammas_Peak / Gammas_Emit;
		
		//Intrinsic Efficiency "IntEff"
		float IntEff = Gammas_Peak / Gammas_Inc;
		
		//Solid Angle "SolidAngle"
		float SolidAngle = 2*pi*(1 - DistFromOrigin/sqrt(DistFromOrigin*DistFromOrigin + Diameter*Diameter/4));
		
		//Predicted Intrinsic Efficiency "PredIntEff"
		float PredIntEff = AbsEff * 4*pi / SolidAngle;


		myfile << to_string(Energy)+","+to_string(AbsEff)+","+to_string(IntEff)+","+to_string(SolidAngle)+","+to_string(PredIntEff)+", \n";

		TString GraphName = to_string(Energy) + "MeV_GammaSpectrum.png";
		c1->Print("Graphs/"+GraphName);

		cout << "Iter" << iter << "succeeded" << endl;
		iter += 1;
	}

	myfile.close();
}
