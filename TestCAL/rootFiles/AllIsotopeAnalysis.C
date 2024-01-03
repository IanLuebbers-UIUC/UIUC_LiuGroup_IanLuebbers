void AllIsotopeAnalysis()
{
//Setup of CSV Data file and Canvases
        int yHeight = 20000;
	int NoEvt = 1000000;
	int NoBin = 60000;

	TCanvas *c1 = new TCanvas();
        c1->SetTickx();
        c1->SetTicky();
        c1->SetGridx();
        c1->SetGridy();


	TFile *input1 = new TFile("TestCAL_Co60.root", "read");
	TFile *input2 = new TFile("TestCAL_Cs137.root", "read");
        TFile *input3 = new TFile("TestCAL_Na22.root", "read");

        TTree *tree1 =  (TTree*)input1->Get("Edep");
        TTree *tree2 =  (TTree*)input2->Get("Edep");
        TTree *tree3 =  (TTree*)input3->Get("Edep");

	double fEdep_HPGe;

	tree1->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe);
	tree2->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe);
        tree3->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe);

        int entries1 = tree1->GetEntries();
        int entries2 = tree2->GetEntries();
        int entries3 = tree3->GetEntries();

	TString IsotopeName;
	TTree *Dummy;
//Loop through each spectrum
	//for(int n = 1; n < 4; n++)
	int n = 3;
	{
		if(n==1)
		{
			IsotopeName = "Cobalt-60";
			Dummy = tree1;
		}
                if(n==2)
                {
                        IsotopeName = "Cesium-137";
			Dummy = tree2;
                }
                if(n==3)
                {
                        IsotopeName = "Sodium-22";
			Dummy = tree3;
                }

		TString Title = IsotopeName + " Gamma Energy Spectrum";

		TH1F *hist1 = new TH1F("hist1", Title, 6000, 0 , 1.6);
		for(int i = 0; i < entries1; i++)
		{
			Dummy->GetEntry(i);
			hist1->Fill(fEdep_HPGe);
		}
		hist1->SetLineColorAlpha(kBlue, 0.35);
		hist1->GetXaxis()->SetTitle("Energy (MeV)");
		hist1->GetYaxis()->SetTitle("Counts");
		hist1->GetYaxis()->SetRangeUser(0, yHeight);
		hist1->SetStats(0);	
		hist1->Draw();

		TLegend *leg = new TLegend(0.15, 0.77, 0.4, 0.87);
        	leg->SetBorderSize(0);
        	leg->AddEntry(hist1, "Deposited Energy", "l");
		leg->Draw();


		TString FileName = IsotopeName+ "_EDepSpectrum.pdf";

        	c1->Print(FileName);
	}
}
