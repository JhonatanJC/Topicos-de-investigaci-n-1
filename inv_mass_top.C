#include <iostream>
#include <string>
#include <stdio.h>
#include <TLorentzVector.h>

int main(){

	TChain *dataset = new TChain("mini");
	dataset->Add("/home/familia_jauja_ccana/TOPICOS1/prueba/mc_110090.stop_tchan_top.root");
		
	const int vs = 5;

	Int_t   lepton_n = -1,
        lepton_charge[vs], 
	lepton_type[vs],
	jet_n=-1;

	Float_t lepton_pt[vs],
        lepton_eta[vs],
        lepton_phi[vs],
        lepton_E[vs],
	jet_eta[vs],
	jet_pt[vs],
	jet_phi[vs],
	jet_MV2c10[vs],
	jet_E[vs],
	met_phi[vs];
	
	Float_t met_et;
	Bool_t	trigE;
   	Bool_t  trigM;

	dataset->SetBranchAddress("lep_n",      &lepton_n);
	dataset->SetBranchAddress("lep_charge", &lepton_charge);
	dataset->SetBranchAddress("lep_type",   &lepton_type);
	dataset->SetBranchAddress("lep_pt",     &lepton_pt);
	dataset->SetBranchAddress("lep_eta",    &lepton_eta);
	dataset->SetBranchAddress("lep_phi",    &lepton_phi);
	dataset->SetBranchAddress("lep_E",      &lepton_E);
	dataset->SetBranchAddress("met_et",	&met_et);
	dataset->SetBranchAddress("jet_n", 	&jet_n);
	dataset->SetBranchAddress("jet_eta", 	&jet_eta);
	dataset->SetBranchAddress("jet_pt", 	&jet_pt);
   	dataset->SetBranchAddress("jet_phi", 	&jet_phi);
   	dataset->SetBranchAddress("jet_E", 	&jet_E);
   	dataset->SetBranchAddress("jet_MV2c10", &jet_MV2c10);
	dataset->SetBranchAddress("trigE", &trigE);
   	dataset->SetBranchAddress("trigM", &trigM);
	dataset->SetBranchAddress("met_phi", &met_phi);
	
	TH1F *hist = new TH1F("variable","Masa del Quark Top ; masa invariante [GeV]; eventos",50,40,300);
	hist->SetFillColor(kBlue);

	int nentries, nbytes, i;
	nentries = (Int_t)dataset->GetEntries();

	// IMPORTANT: fraction events we want to run
	float fraction_events = 1;
	float events_to_run = nentries*fraction_events;

	std::cout << "Total # events = "  << nentries
          << ". Events to run = " << events_to_run
          << " corresponding to " << fraction_events*100
          << "% of total events!" << std::endl;
	
	
	TLorentzVector l1;         
        TLorentzVector jet1;  
	TLorentzVector jet2;
        TLorentzVector invmass; 
       	TLorentzVector MeT;	
	TLorentzVector invmass_b_l;
	
		
	for (i = 0; i < nentries; i++)
	{
    	nbytes = dataset->GetEntry(i);
		//Corte #1
      		if(met_et > 30000)
		{
			if(trigE || trigM)
			{
    		    	// Cut #2:
    		    	if(lepton_n == 1 && lepton_pt[0] > 35000 )
    		    	{
        	       		// Cut #4:
        	       		if(jet_n == 2)
        	       		{
					// Cut #4:
					if (jet_pt[0] > 30000 && jet_pt[1] > 30000)
					{
						// Cut #5: 
						if (jet_MV2c10[1] >0.8244273)
						{	
							// Cut #6:	
            	           				if ( TMath::Abs(jet_eta[0]) > 1.5 )
            	           				{
								// Cut #7:
								float deltaeta = TMath::Abs( jet_eta[0] - jet_eta[1] );		
				  				if (deltaeta > 1.5)
								{
                               						l1.SetPtEtaPhiE(lepton_pt[0]/1000., lepton_eta[0], lepton_phi[0], lepton_E[0]/1000.);
                               						jet1.SetPtEtaPhiE(jet_pt[0]/1000., jet_eta[0], jet_phi[0], jet_E[0]/1000.);
									jet2.SetPtEtaPhiE(jet_pt[1]/1000., jet_eta[1], jet_phi[1], jet_E[1]/1000.);
                               						// The default value in the root file is in MeV, so, we divide by 1000 to get it in GeV
                               						
									MeT.SetPtEtaPhiE(met_et/1000, 0, met_phi[0] , met_et/1000);
									//float mtw = sqrt(2*l1.Pt()*MeT.Et()*(1-cos(l1.DeltaPhi(MeT))));
									
									//float MW = sqrt(mtw*mtw - (l1.Px() + MeT.Px())*(l1.Px() + MeT.Px()) - (l1.Py() + MeT.Py())*(l1.Py() + MeT.Py()) ) ;

									invmass_b_l =  jet2 + l1;
									float masa_bl=invmass_b_l.M();

									if (masa_bl < 150)
									{
										
										invmass =  jet2 + l1 + MeT;

										hist->Fill(invmass.M());
									}

									
								}
							}
						}
            	           		}
        	        	}
    		    	}
			}
		}
	}
	
	TF1 *fitt = new TF1("fitt","gaus", 0,250);

	TCanvas *canvas = new TCanvas("c","c",10,10,700,700);
	hist->Draw();
	canvas->Draw();
	hist->Fit("fitt");

	return 0;
}

