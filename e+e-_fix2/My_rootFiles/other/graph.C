void graph()
{
//Set File Parameters
	int Diameter = 8;
	int Length = 3;

        int yHeight = 25000;

	TCanvas *c1 = new TCanvas();
        c1->SetTickx();
        c1->SetTicky();
        c1->SetGridx();
        c1->SetGridy();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Read Data from TFile
	TFile *input1 = new TFile("scoring.root", "read");
        TTree *tree1 =  (TTree*)input1->Get("crystal_edep");
        double fEdep_HPGe;
	tree1->SetBranchAddress("cyrstal_edep_score", &fEdep_HPGe);
        int entries1 = tree1->GetEntries();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Graph the data
	TString Title = "graph";

	//Draw Spectra
        TH1F *hist1 = new TH1F("hist1", Title, 1000, 0 , 0.7);
        for(int i = 0; i < entries1; i++)
        {
                tree1->GetEntry(i);
                hist1->Fill(fEdep_HPGe);
        }
        hist1->SetLineColorAlpha(kRed, 0.35);
        hist1->GetXaxis()->SetTitle("Energy (MeV)");
        hist1->GetYaxis()->SetTitle("Counts");
        hist1->GetYaxis()->SetRangeUser(0, yHeight);	
	hist1->SetStats(0);	
	hist1->Draw();

	//Draw Legend
	TLegend *leg = new TLegend(0.4, 0.77, 0.65, 0.87);
	leg->SetBorderSize(0);
	leg->AddEntry(hist1, "Deposited Energy", "l");
	leg->Draw();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Print the Graph
        TString GraphName = "BackgroundNoShield.pdf";
                
	//c1->Print(GraphName);
}
