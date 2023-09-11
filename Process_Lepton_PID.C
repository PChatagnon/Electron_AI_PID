#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TF1.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH2D.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH3F.h"
#include "bib/TCSclass.h"
#include "bib/TCSfunc.h"
#include "reader.h"
using namespace std;

int Process_Lepton_PID()
{

	Int_t argc = gApplication->Argc();
	char **argv = gApplication->Argv();

	TString output_file = TString(argv[argc - 1]);
	cout<<"Outputting on "<<output_file<<"\n";


	Float_t P_Lepton, Theta_Lepton, Phi_Lepton;
	Float_t SFPCAL_Lepton, SFECIN_Lepton, SFECOUT_Lepton;
	Float_t m2PCAL_Lepton, m2ECIN_Lepton, m2ECOUT_Lepton;
	Float_t m3PCAL_Lepton, m3ECIN_Lepton, m3ECOUT_Lepton;
	Float_t vx_Lepton, vy_Lepton, vz_Lepton;

	TFile *outFile_Lepton = new TFile(Form("outFile_Lepton%s.root", output_file.Data()), "recreate");
	TTree *out_Lepton = new TTree("tree", "tree");

	out_Lepton->Branch("P", &P_Lepton, "P/F");
	out_Lepton->Branch("Theta", &Theta_Lepton, "Theta/F");
	out_Lepton->Branch("Phi", &Phi_Lepton, "Phi/F");
	out_Lepton->Branch("SFPCAL", &SFPCAL_Lepton, "SFPCAL/F");
	out_Lepton->Branch("SFECIN", &SFECIN_Lepton, "SFECIN/F");
	out_Lepton->Branch("SFECOUT", &SFECOUT_Lepton, "SFECOUT/F");
	out_Lepton->Branch("m2PCAL", &m2PCAL_Lepton, "m2PCAL/F");
	out_Lepton->Branch("m2ECIN", &m2ECIN_Lepton, "m2ECIN/F");
	out_Lepton->Branch("m2ECOUT", &m2ECOUT_Lepton, "m2ECOUT/F");
	out_Lepton->Branch("m3PCAL", &m3PCAL_Lepton, "m3PCAL/F");
	out_Lepton->Branch("m3ECIN", &m3ECIN_Lepton, "m3ECIN/F");
	out_Lepton->Branch("m3ECOUT", &m3ECOUT_Lepton, "m3ECOUT/F");
	out_Lepton->Branch("vx_Lepton", &vx_Lepton, "vx_Lepton/F");
	out_Lepton->Branch("vy_Lepton", &vy_Lepton, "vy_Lepton/F");
	out_Lepton->Branch("vz_Lepton", &vz_Lepton, "vz_Lepton/F");

	Float_t P_Pion, Theta_Pion, Phi_Pion;
	Float_t SFPCAL_Pion, SFECIN_Pion, SFECOUT_Pion;
	Float_t m2PCAL_Pion, m2ECIN_Pion, m2ECOUT_Pion;
	Float_t m3PCAL_Pion, m3ECIN_Pion, m3ECOUT_Pion;

	TFile *outFile_Pion = new TFile(Form("outFile_Pion%s.root", output_file.Data()), "recreate");
	TTree *out_Pion = new TTree("tree", "tree");

	out_Pion->Branch("P", &P_Pion, "P/F");
	out_Pion->Branch("Theta", &Theta_Pion, "Theta/F");
	out_Pion->Branch("Phi", &Phi_Pion, "Phi/F");
	out_Pion->Branch("SFPCAL", &SFPCAL_Pion, "SFPCAL/F");
	out_Pion->Branch("SFECIN", &SFECIN_Pion, "SFECIN/F");
	out_Pion->Branch("SFECOUT", &SFECOUT_Pion, "SFECOUT/F");
	out_Pion->Branch("m2PCAL", &m2PCAL_Pion, "m2PCAL/F");
	out_Pion->Branch("m2ECIN", &m2ECIN_Pion, "m2ECIN/F");
	out_Pion->Branch("m2ECOUT", &m2ECOUT_Pion, "m2ECOUT/F");
	out_Pion->Branch("m3PCAL", &m3PCAL_Pion, "m3PCAL/F");
	out_Pion->Branch("m3ECIN", &m3ECIN_Pion, "m3ECIN/F");
	out_Pion->Branch("m3ECOUT", &m3ECOUT_Pion, "m3ECOUT/F");

	double me = 0.000511;
	double mp = 0.938;
	double mPion = 0.139;
	double ebeam = 10.6;

	double Pi = 3.14159269;

	int LTCC = 16;
	int HTCC = 15;
	int ECAL = 7;

	// ECAL layer
	int PCAL = 1;
	int ECIN = 4;
	int ECOUT = 7;

	Particle vLepton;
	Particle vPion;

	int nbEvent = 0;

	

	for (Int_t i = 3; i < (argc - 1); i++)
	{
		TString nameFiles = TString(argv[i]);
		cout<<"Running on "<<nameFiles<<"\n";
		hipo::reader reader;
		reader.open(nameFiles);

		hipo::dictionary factory;
		reader.readDictionary(factory);
		factory.show();
		hipo::event event;

		hipo::bank EVENT(factory.getSchema("REC::Event"));
		hipo::bank PART(factory.getSchema("REC::Particle"));
		hipo::bank MCPART(factory.getSchema("MC::Particle"));
		hipo::bank CALO(factory.getSchema("REC::Calorimeter"));

		while (reader.next())
		{

			int nLepton = 0;
			int nPion = 0;

			reader.read(event);
			event.getStructure(PART);
			event.getStructure(MCPART);
			event.getStructure(CALO);
			event.getStructure(EVENT);

			nbEvent++;


			Particle vRecParticle;
			Particle vPion;
			Particle vLepton;

			Particle MC_Lepton;
			Particle MC_Pion;

			MC_Lepton.Vector.SetXYZM(MCPART.getFloat("px", 0), MCPART.getFloat("py", 0), MCPART.getFloat("pz", 0), me);
			MC_Pion.Vector.SetXYZM(MCPART.getFloat("px", 1), MCPART.getFloat("py", 1), MCPART.getFloat("pz", 1), mPion);

			int np = PART.getRows();
			for (int i = 0; i < np; i++)
			{

				int pid = PART.getInt("pid", i);
				float px = PART.getFloat("px", i);
				float py = PART.getFloat("py", i);
				float pz = PART.getFloat("pz", i);
				float beta = PART.getFloat("beta", i);
				int status = PART.getInt("status", i);
				float chi2 = PART.getFloat("chi2pid", i);
				float vx = PART.getFloat("vx", i);
				float vy = PART.getFloat("vy", i);
				float vz = PART.getFloat("vz", i);

				if (abs(pid) == 11)
				{
					vRecParticle.Vector.SetXYZM(px, py, pz, me);
					vRecParticle.index = i;
					vRecParticle.pid = 11;
					vRecParticle.beta = beta;
					vRecParticle.status = status;
					vRecParticle.chi2 = chi2;
					vRecParticle.vertex.x = vx;
					vRecParticle.vertex.y = vy;
					vRecParticle.vertex.z = vz;
					if (abs(MC_Lepton.Vector.Phi() - vRecParticle.Vector.Phi()) < (60. * 3.141592 / 180.))
					{
						nLepton++;
						vLepton = vRecParticle;
					}
					if (abs(MC_Pion.Vector.Phi() - vRecParticle.Vector.Phi()) < (60. * 3.141592 / 180.))
					{
						nPion++;
						vPion = vRecParticle;
					}

				}
			}

			vector<Particle> Particles = {vLepton, vPion};

			CalorimeterResp Calo;
			CheResp Che;
			ScinResp Scin;
			for (int i = 0; i < 2; i++)
			{

				for (int c = 0; c < CALO.getRows(); c++)
				{

					int Calopindex = CALO.getInt("pindex", c);
					int Calosector = CALO.getInt("sector", c);
					int Calolayer = CALO.getInt("layer", c);
					int Calodetector = CALO.getInt("detector", c);
					float Caloenergy = CALO.getFloat("energy", c);
					float Calox = CALO.getFloat("x", c);
					float Caloy = CALO.getFloat("y", c);
					float Caloz = CALO.getFloat("z", c);
					float Calodu = CALO.getFloat("du", c);
					float Calodv = CALO.getFloat("dv", c);
					float Calodw = CALO.getFloat("dw", c);
					float Calom2u = CALO.getFloat("m2u", c);
					float Calom2v = CALO.getFloat("m2v", c);
					float Calom2w = CALO.getFloat("m2w", c);
					float Calom3u = CALO.getFloat("m3u", c);
					float Calom3v = CALO.getFloat("m3v", c);
					float Calom3w = CALO.getFloat("m3w", c);

					if (Calopindex == (Particles[i].index))
					{
						Calo.detector = Calodetector;
						Calo.pindex = Calopindex;
						Calo.sector = Calosector;
						Calo.layer = Calolayer;
						Calo.energy = Caloenergy;
						Calo.x = Calox;
						Calo.y = Caloy;
						Calo.z = Caloz;
						Calo.du = Calodu;
						Calo.dv = Calodv;
						Calo.dw = Calodw;
						Calo.m2u = Calom2u;
						Calo.m2v = Calom2v;
						Calo.m2w = Calom2w;
						Calo.m3u = Calom3u;
						Calo.m3v = Calom3v;
						Calo.m3w = Calom3w;
						Particles[i].Calorimeter.push_back(Calo);
					}
				}
			}

			vLepton = Particles[0];
			vPion = Particles[1];

			if (nLepton == 1)
			{
				double SFleptonPCAL = (vLepton.Energy(ECAL, PCAL)) / vLepton.Vector.P();
				double SFleptonECALin = (vLepton.Energy(ECAL, ECIN)) / vLepton.Vector.P();
				double SFleptonECALout = (vLepton.Energy(ECAL, ECOUT)) / vLepton.Vector.P();

				// CALO.show();
				double M2PCAL = -1.;
				double M2ECIN = -1.;
				double M2ECOUT = -1.;

				double M3PCAL = -5.;
				double M3ECIN = -5.;
				double M3ECOUT = -5.;

				for (int i = 0; i < vLepton.Calorimeter.size(); i++)
				{
					if (PCAL == vLepton.Calorimeter[i].layer)
					{
						M2PCAL = (vLepton.Calorimeter[i].m2u + vLepton.Calorimeter[i].m2v + vLepton.Calorimeter[i].m2w) / 3.;
						M3PCAL = (vLepton.Calorimeter[i].m3u + vLepton.Calorimeter[i].m3v + vLepton.Calorimeter[i].m3w) / 3.;
					}
					if (ECIN == vLepton.Calorimeter[i].layer)
					{
						M2ECIN = (vLepton.Calorimeter[i].m2u + vLepton.Calorimeter[i].m2v + vLepton.Calorimeter[i].m2w) / 3.;
						M3ECIN = (vLepton.Calorimeter[i].m3u + vLepton.Calorimeter[i].m3v + vLepton.Calorimeter[i].m3w) / 3.;
					}
					if (ECOUT == vLepton.Calorimeter[i].layer)
					{
						M2ECOUT = (vLepton.Calorimeter[i].m2u + vLepton.Calorimeter[i].m2v + vLepton.Calorimeter[i].m2w) / 3.;
						M3ECOUT = (vLepton.Calorimeter[i].m3u + vLepton.Calorimeter[i].m3v + vLepton.Calorimeter[i].m3w) / 3.;
					}
				}

				m2PCAL_Lepton = M2PCAL;
				m2ECIN_Lepton = M2ECIN;
				m2ECOUT_Lepton = M2ECOUT;

				if (abs(M3PCAL) < 10. && !isnan(M3PCAL))
					m3PCAL_Lepton = M3PCAL;
				if (abs(M3ECIN) < 10. && !isnan(M3ECIN))
					m3ECIN_Lepton = M3ECIN;
				if (abs(M3ECOUT) < 10. && !isnan(M3ECOUT))
					m3ECOUT_Lepton = M3ECOUT;

				SFPCAL_Lepton = SFleptonPCAL;
				SFECIN_Lepton = SFleptonECALin;
				SFECOUT_Lepton = SFleptonECALout;

				P_Lepton = vLepton.Vector.P();
				Theta_Lepton = vLepton.Vector.Theta();
				Phi_Lepton = vLepton.Vector.Phi();

				vx_Lepton = vLepton.vertex.x;
				vy_Lepton = vLepton.vertex.y;
				vz_Lepton = vLepton.vertex.z;

				out_Lepton->Fill();
			}

			if (nPion == 1)
			{
				double SFpionPCAL = (vPion.Energy(ECAL, PCAL)) / vPion.Vector.P();
				double SFpionECALin = (vPion.Energy(ECAL, ECIN)) / vPion.Vector.P();
				double SFpionECALout = (vPion.Energy(ECAL, ECOUT)) / vPion.Vector.P();

				// CALO.show();
				double M2PCAL = -1.;
				double M2ECIN = -1.;
				double M2ECOUT = -1.;

				double M3PCAL = -5.;
				double M3ECIN = -5.;
				double M3ECOUT = -5.;

				for (int i = 0; i < vPion.Calorimeter.size(); i++)
				{
					if (PCAL == vPion.Calorimeter[i].layer)
					{
						M2PCAL = (vPion.Calorimeter[i].m2u + vPion.Calorimeter[i].m2v + vPion.Calorimeter[i].m2w) / 3.;
						M3PCAL = (vPion.Calorimeter[i].m3u + vPion.Calorimeter[i].m3v + vPion.Calorimeter[i].m3w) / 3.;
					}
					if (ECIN == vPion.Calorimeter[i].layer)
					{
						M2ECIN = (vPion.Calorimeter[i].m2u + vPion.Calorimeter[i].m2v + vPion.Calorimeter[i].m2w) / 3.;
						M3ECIN = (vPion.Calorimeter[i].m3u + vPion.Calorimeter[i].m3v + vPion.Calorimeter[i].m3w) / 3.;
					}
					if (ECOUT == vPion.Calorimeter[i].layer)
					{
						M2ECOUT = (vPion.Calorimeter[i].m2u + vPion.Calorimeter[i].m2v + vPion.Calorimeter[i].m2w) / 3.;
						M3ECOUT = (vPion.Calorimeter[i].m3u + vPion.Calorimeter[i].m3v + vPion.Calorimeter[i].m3w) / 3.;
					}
				}

				m2PCAL_Pion = M2PCAL;
				m2ECIN_Pion = M2ECIN;
				m2ECOUT_Pion = M2ECOUT;

				if (abs(M3PCAL) < 10. && !isnan(M3PCAL))
					m3PCAL_Pion = M3PCAL;
				if (abs(M3ECIN) < 10. && !isnan(M3ECIN))
					m3ECIN_Pion = M3ECIN;
				if (abs(M3ECOUT) < 10. && !isnan(M3ECOUT))
					m3ECOUT_Pion = M3ECOUT;

				SFPCAL_Pion = SFpionPCAL;
				SFECIN_Pion = SFpionECALin;
				SFECOUT_Pion = SFpionECALout;

				P_Pion = vPion.Vector.P();
				Theta_Pion = vPion.Vector.Theta();
				Phi_Pion = vPion.Vector.Phi();
				
				/*
				cout<<"//////////////////////////\n";
				cout<<"EVENT\n";
				cout<<vPion.Vector.Px()<<" "<<vPion.Vector.Py()<<" "<<vPion.Vector.Pz()<<"\n";
				PART.show();

				cout<<MC_Pion.Vector.Px()<<" "<<MC_Pion.Vector.Py()<<" "<<MC_Pion.Vector.Pz()<<"\n";
				MCPART.show();
				cout<<"//////////////////////////\n";
				*/

				out_Pion->Fill();
			}
		}
	}

	outFile_Lepton->cd();
	out_Lepton->Write();
	outFile_Lepton->Close();
	outFile_Lepton->Close();

	outFile_Pion->cd();
	out_Pion->Write();
	outFile_Pion->Close();
	outFile_Pion->Close();

	gApplication->Terminate();

	return 0;
}
