#include "../include/Utils.h"


bool TopTag(TopJet topjet,  double &mjet, int &nsubjets, double &mmin){
 
  nsubjets=topjet.numberOfDaughters();

  LorentzVector allsubjets(0,0,0,0);
  
  for(int j=0; j<topjet.numberOfDaughters(); ++j){
    allsubjets += topjet.subjets()[j].v4();
  }
  if(!allsubjets.isTimelike()){
    mjet=0;
    mmin=0;
    return false;
  }

  mjet = allsubjets.M();
  
  if(nsubjets>=3) {

    std::vector<Particle> subjets = topjet.subjets();
    sort(subjets.begin(), subjets.end(), HigherPt());

    double m01 = 0;
    if( (subjets[0].v4()+subjets[1].v4()).isTimelike()) 
      m01=(subjets[0].v4()+subjets[1].v4()).M();
    double m02 = 0;
    if( (subjets[0].v4()+subjets[2].v4()).isTimelike() ) 
      m02=(subjets[0].v4()+subjets[2].v4()).M();
    double m12 = 0;
    if( (subjets[1].v4()+subjets[2].v4()).isTimelike()  )
      m12 = (subjets[1].v4()+subjets[2].v4()).M();
    
    //minimum pairwise mass > 50 GeV/c^2
    mmin = std::min(m01,std::min(m02,m12));
  }

  //at least 3 sub-jets
  if(nsubjets<3) return false;
  //minimum pairwise mass > 50 GeV/c^2
  if(mmin<50) return false;
  //jet mass between 140 and 250 GeV/c^2
  if(mjet<140 || mjet>250) return false;
  
  return true;
}

Jet* nextJet(const Particle *p, std::vector<Jet> *jets){

  double deltarmin = double_infinity();
  Jet* nextjet=0;
  for(unsigned int i=0; i<jets->size();++i){
    if(jets->at(i).deltaR(*p) < deltarmin){
      deltarmin = jets->at(i).deltaR(*p);
      nextjet = &jets->at(i);
    }
  }

  return nextjet;
}

double pTrel(const Particle *p, std::vector<Jet> *jets){

  double ptrel=0;

  Jet* nextjet =  nextJet(p,jets);

  TVector3 p3(p->v4().Px(),p->v4().Py(),p->v4().Pz());
  TVector3 jet3(nextjet->v4().Px(),nextjet->v4().Py(),nextjet->v4().Pz());

  if(p3.Mag()!=0 && jet3.Mag()!=0){
    double sin_alpha = (p3.Cross(jet3)).Mag()/p3.Mag()/jet3.Mag();
    ptrel = p3.Mag()*sin_alpha;
  }
  else{
    std::cout << "something strange happend in the ptrel calculation: either lepton or jet momentum is 0" <<std::endl;
  }

  return ptrel;
}

double deltaRmin(const Particle *p, std::vector<Jet> *jets){
  return nextJet(p,jets)->deltaR(*p);
}

TVector3 toVector(LorentzVector v4){

  TVector3 v3(0,0,0);
  v3.SetX(v4.X());
  v3.SetY(v4.Y());
  v3.SetZ(v4.Z());
  return v3;
}

TVector3 toVector(LorentzVectorXYZE v4){

  TVector3 v3(0,0,0);
  v3.SetX(v4.X());
  v3.SetY(v4.Y());
  v3.SetZ(v4.Z());
  return v3;
}

LorentzVectorXYZE toXYZ(LorentzVector v4){

  LorentzVectorXYZE v4_new(0,0,0,0);
  v4_new.SetPx(v4.Px());
  v4_new.SetPy(v4.Py());
  v4_new.SetPz(v4.Pz());
  v4_new.SetE(v4.E());
  return v4_new;
}

LorentzVector toPtEtaPhi(LorentzVectorXYZE v4){

  LorentzVector v4_new(0,0,0,0);
  v4_new.SetPt(v4.Pt());
  v4_new.SetEta(v4.Eta());
  v4_new.SetPhi(v4.Phi());
  v4_new.SetE(v4.E());
  return v4_new;
}

double deltaR(LorentzVector v1, LorentzVector v2){

  Particle p1;
  p1.set_v4(v1);
  Particle p2;
  p2.set_v4(v2);
  return p1.deltaR(p2);
}

double double_infinity(){
  return std::numeric_limits<double>::infinity() ;
}

int int_infinity(){
  return std::numeric_limits<int>::max() ;
}

int myPow(int x, unsigned int p) {
  int i = 1;
  for (unsigned int j = 1; j <= p; j++)  i *= x;
  return i;
}
