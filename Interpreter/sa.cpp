#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
typedef vector<pair<int,int>> vecii;

void FindAndReplace(string &strTemp, string f, string r){
  for (size_t p = strTemp.find( f ); p != string::npos; p = strTemp.find( f, p ) ){
      strTemp.replace( p, f.length(),  r);
  }
}

string Operons(vecii &p){
  ifstream filein("./sa/genes.txt");
  string operons;
  string strTemp;

  for(int i = 0; i < p.size(); i++){
    while(getline(filein, strTemp)){
        FindAndReplace(strTemp,"XYZ", to_string(i+1));
        strTemp += "\n";
        operons += strTemp;
    }
    operons += "\n";
    filein.clear();
    filein.seekg(0, filein.beg);
  }

  return operons;
}

string Plasmids(vecii &p){
    ifstream filein("./sa/plasmids.txt");
    string plasmids;
    string strTemp;

    for(int i = 0; i < p.size(); i++){
        while(getline(filein, strTemp)){
            FindAndReplace(strTemp,"XYZ", to_string(i+1));
            plasmids += strTemp;
        }
        filein.clear();
        filein.seekg(0, filein.beg);
        plasmids += ",\n    ";
    }
    plasmids.resize(plasmids.size()-6);
    return plasmids;
}

string Conjugations(vecii &p, string c){
    ifstream filein("./sa/conjugations.txt");
    string conjugation;
    string strTemp;

    for(int i = 0; i < p.size(); i++){
        while(getline(filein, strTemp)){
            FindAndReplace(strTemp,"XYZ", to_string(i+1));
            FindAndReplace(strTemp,"RATE", c);
            strTemp += "\n";
            conjugation += strTemp;
        }
        filein.clear();
        filein.seekg(0, filein.beg);
    }

    return conjugation;
}

string QS(vecii &p){
  ifstream filein("./sa/qs.txt");
  string conjugation;
  string strTemp;

  for(int i = 0; i < p.size(); i++){
      while(getline(filein, strTemp)){
          FindAndReplace(strTemp,"RATE", to_string(p[i].second));
          FindAndReplace(strTemp,"XYZ", to_string(i+1));
          strTemp += "\n";
          conjugation += strTemp;
      }
      filein.clear();
      filein.seekg(0, filein.beg);
  }

  return conjugation;
}

string AmountBacteria(vecii &p){
    ifstream filein("./sa/bacteria.txt");
    string bacteria;
    string strTemp;
    int amount = 0;

    for(int i = 0; i < p.size(); i++){
        while(getline(filein, strTemp)){
            FindAndReplace(strTemp,"XYZ", to_string(i+1));
            FindAndReplace(strTemp,"AMOUNT", to_string(p[i].first));
            strTemp += "\n";
            bacteria += strTemp;
        }
        amount += p[i].first;
        filein.clear();
        filein.seekg(0, filein.beg);
        bacteria += "    ";
    }

    string total = "\n    c_ecolis(TOTAL_AMOUNT, 0, 0, 200, {}, program p());";

    FindAndReplace(total, "TOTAL_AMOUNT", to_string(amount));

    bacteria += total;

    return bacteria;
}

void MakeTemplate(ofstream &fout, vecii &p, string c, string pop_max){
  ifstream filein("./sa/template.txt");

  string operons = Operons(p);
  string plasmids = Plasmids(p);
  string conjugations = Conjugations(p, c);
  string qs = QS(p);
  string bacterias = AmountBacteria(p);

  string strTemp;
  while(getline(filein, strTemp)) {
    FindAndReplace(strTemp, "POP_MAX", pop_max);
    FindAndReplace(strTemp, "//ADD_THE_OPERONS", operons);
    FindAndReplace(strTemp, "//ADD_PLASMIDS", plasmids);
    FindAndReplace(strTemp, "//ADD CONJUGATION", conjugations);
    FindAndReplace(strTemp, "//QS", qs);
    FindAndReplace(strTemp, "//BACTERIA", bacterias);
    fout << strTemp;
  }
}

int main() {

    /* READING PARAMETERS */
    int size;
    cout << "SIMULATED ANNEALING GENERATOR\n\nPlease enter the number of proteins needed for the simulation: ";
    cin >> size;

    cout << "\nPlease say for each protein, the initial amount and de Quorum Sensing Rate\n";
    vecii proteins(size);
    for (int i = 0; i < size; i++) {
        cout << "Protein " << i + 1 << ": ";
        cin >> proteins[i].first;
        cin >> proteins[i].second;
    }

    string conjugation;
    cout << "\nNow please type the percentage Now please type the percentage of Conjution:\n";
    cout << "conjugation: "; cin >> conjugation;

    // mutation /= 100;
    // crossover /= 100;

    string POP_MAX;
    cout <<"\nFinally, please enter the population max of the simulation: ";
    cin >> POP_MAX;
    /* PARAMETERS FINISHED */

    /* STAR MAKING THE gro FILE */
    ofstream output;
    output.open("output_sa.gro");

    MakeTemplate(output, proteins, conjugation, POP_MAX);

    /* GRO FILE FINISHED */
    output.close();
}
