#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <mathimf.h>
#include <mkl.h>

std::random_device rd;
VSLStreamStatePtr stream;
int status = vslNewStream(&stream,VSL_BRNG_MT19937,rd());
double qadran;
void get_random() {
    int status = vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD,stream,1,&qadran,0.0,1.0);
}

const int SIZE=100;
int Lattice[SIZE][SIZE][SIZE];
int OldSpin;
int Reflection=-1;
int reps_eq=10000;
int reps=100000;
int reps_total;
int count;
double T; //Tc is 4.512
double binder;

void BuildCluster();
void AddSpin(int n, int m, int k);
void TakeImage(std::ofstream& outfile);

int main(int argc, char** argv) {
    std::string value(argv[1]);
    T=stof(value);
    std::string filename("T_");
    filename+=value;
    filename+=".csv";
    filename[3]='_';
    std::ofstream outfile;
    outfile.open(filename);
    
    reps_total=reps+reps_eq;
    int i,j,k; double spin;
    for (i=0; i<SIZE; i++) {
        for (j=0; j<SIZE; j++) {
            for (k=0; k<SIZE; k++) {
                get_random();
                spin=qadran;
                if (spin<0.50) {
                    Lattice[i][j][k] = -1;
                }
                else {
                    Lattice[i][j][k] = 1;
                }
            }
        }
    }
    for (count=0; count<reps_total; count++) {
        if (count>=reps_eq && (count-reps_eq)%10000==0) {
            std::cout << "This is temp " << T << " and iteration " <<(count-reps_eq)<<std::endl;
        }
        BuildCluster();
        if (count>=reps_eq && count%100==0) {
            TakeImage(outfile);
        }
    }
    outfile.close();
    
    return 0;
}

void BuildCluster() {
    int n,m,k;
    
    get_random();
    n = int(qadran*double(SIZE));
    get_random();
    m = int(qadran*double(SIZE));
    get_random();
    k = int(qadran*double(SIZE));
    OldSpin=Lattice[n][m][k];
    AddSpin(n,m,k);
}

void AddSpin(int n, int m, int k)
{
    int nPrev, nNext, mPrev, mNext, kPrev, kNext;
    double P;
    
    Lattice[n][m][k] *= Reflection;
    nPrev = (n==0 ? SIZE-1 : n-1); nNext = (n==SIZE-1 ? 0 : n+1);
    mPrev = (m==0 ? SIZE-1 : m-1); mNext = (m==SIZE-1 ? 0 : m+1);
    kPrev = (k==0 ? SIZE-1 : k-1); kNext = (k==SIZE-1 ? 0 : k+1);
    P = 1.0-exp(-2.0/T);
    
    get_random();
    if (Lattice[nPrev][m][k]==OldSpin && P>qadran) {AddSpin(nPrev, m, k);}
    get_random();
    if (Lattice[nNext][m][k]==OldSpin && P>qadran) {AddSpin(nNext, m, k);}
    get_random();
    if (Lattice[n][mPrev][k]==OldSpin && P>qadran) {AddSpin(n, mPrev, k);}
    get_random();
    if (Lattice[n][mNext][k]==OldSpin && P>qadran) {AddSpin(n, mNext, k);}
    get_random();
    if (Lattice[n][m][kPrev]==OldSpin && P>qadran) {AddSpin(n, m, kPrev);}
    get_random();
    if (Lattice[n][m][kNext]==OldSpin && P>qadran) {AddSpin(n, m, kNext);}
}

void TakeImage(std::ofstream& outfile) {
    int i,j,k;
    for (k=0; k<SIZE; k+=10) {
        for (i=0; i<SIZE; i++) {
            for (j=0; j<SIZE; j++) {
                if (Lattice[i][j][k]==-1) {
                    outfile << 0 << ',';
                }
                else {
                    outfile << 1 << ',';
                }
            }
        }
        outfile << "0,0,1,0,0" << std::endl;
    }
}

