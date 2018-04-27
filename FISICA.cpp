#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <windows.h>
#include <time.h>
#include <cmath>

//Velocidade em m/s
//Aceleração em m/s²
//Raio em m
#define VMAX 2
#define AMAX 0.5
#define RAIO 0.2

using namespace std;


double stringToDouble (string Text){
	double Result;
	stringstream convert(Text);

	if ( !(convert >> Result) )
	    Result = 0;
	return Result;
}

void formatar(string linha, double* T, double* X, double* Y){
	string sT="", sX="", sY="";
	int i, final=4, cont=0;
	char car;
	if(linha[2]==',') final=5;
	for(i=0; i<final; i++){
		car = linha[i];
		if(car==',') car='.';
		sT.push_back(car);
	}
	i++; // pula o tab
	cont=0;
	for(; cont<5; i++){
		car = linha[i];
		if(car==',') car='.';
		sX.push_back(car);
		cont++;
	}
	i++; // pula o tab
	cont=0;
	for(; cont<5; i++){
		car = linha[i];
		if(car==',') car='.';
		sY.push_back(car);
		cont++;
	}
	*T = stringToDouble(sT);
	*X = stringToDouble(sX);
	*Y = stringToDouble(sY);
}

double posX(double t){
	double x = -0.005*pow(t,2)+0.5*t+1;
	return x;
}

double posY(double t){
	double y = -0.008*pow(t,2)+0.4*t+0.5;
	return y;
}

double velX(double t){
	double vx = -0.01*t + 0.5;
	return vx;
}

double velY(double t){
	double vy = -0.016*t + 0.4;
	return vy;
}

double movimentoDoRobo(double t, double tempoAce, double tempoDesace){
	double altura, base, BASE, areaTriangulo, areaTrapezio;
	if(t <= (tempoAce+tempoDesace)){
		altura = (AMAX*t)/2;
		base = t;
		areaTriangulo = (altura*base)/2;
		return areaTriangulo;
	}
	else{
		altura = VMAX;
		BASE = t;
		base = t-(tempoAce+tempoDesace);
		
		areaTrapezio = ((BASE+base)*altura)/2;
		
		return areaTrapezio;
	}
}

double sorvetao(double S0x, double S0y, double V0x, double V0y, double ax, double ay, double t, double RX, double RY, double P){
	double Sx = S0x + V0x*t + (ax*t*t)/2;
	double Sy = S0y + V0y*t + (ay*t*t)/2;
	double dX = abs(abs(Sx)-abs(RX));
	double dY = abs(abs(Sy)-abs(RY));
	double S = sqrt((dX*dX)+(dY*dY));
	
	dist = abs(abs(S)-abs(P));
	cout << "S = " << S << "; em t = " << t << endl;
	if(dist<=RAIO) return t;
	else return -1;
}

int tempoSorvetao(double S0, double V0, double S, double a){
	double t = sqrt((2*(S-S0))/(2*(V0+a)));
	cout << "t = " << t << "; em S = " << S << endl;
}

// Km Hm Dam m Dm Cm Mm
int main(){
	vector<double> t, x, y, vx, vy, ax, ay, Rx, Ry, Rvx, Rvy, Rax, Ray, Spotencial;
	map<int, double> indexToSec;
	map<double, int> secToIndex;
	int i=0;
	double sec;
	string linha;
	double RX, RY;
	
	double tempoAce = VMAX/AMAX;
	double tempoDesace = tempoAce;
	for(sec=0; sec<=20; sec+=0.02){
		secToIndex[sec] = i;
		indexToSec[i] = sec;
		cout << fixed << "sec = " << sec << " S = " << movimentoDoRobo(sec, tempoAce, tempoDesace) << endl;
		Spotencial.push_back(movimentoDoRobo(sec, tempoAce, tempoDesace));
		i++;
	}
	srand (time(NULL));
	double total;
	/*
	do{
		cout << "Digite as cordenadas X e Y do robo:" << endl;
		cin >> RX >> RY;
		double restoX = abs(1.000-abs(RX)), restoY = abs(0.500-abs(RY));
		total = sqrt((abs(restoX)*abs(restoX))+(abs(restoY)*abs(restoY)));
	}while(total>1.000);
	*/
	do{
		int random;
		random = rand() % 2000;
		RX = (random)/1000.0;
		random = rand() % 2000;
		RY = (random-500)/1000.0;
		double restoX = abs(1.000-abs(RX)), restoY = abs(0.500-abs(RY));
		total = sqrt((abs(restoX)*abs(restoX))+(abs(restoY)*abs(restoY)));
	}while(total>1.000);
	

	
	cout << "RX = " << RX << "; RY = " << RY << endl;
	// LEITURA E PREENCHIMENTO DE VETORES EM TEMPO REAL
	ifstream myfile ("Ora_bolas-trajetoria _bola_oficial.dat");
	ofstream outfile;
	if (myfile.is_open()){
		cout << "O arquivo foi aberto" << endl;
		outfile.open("Excel_file.csv");
		outfile << "t/s;";
		outfile << "x/m;";
		outfile << "y/m;";
		outfile << "vx/m/s;";
		outfile << "vy/m/s;";
		outfile << "ax/m/s²;";
		outfile << "ay/m/s²;";
		outfile << "dist/m;";
		outfile << "Rx/m;";
		outfile << "Ry/m\n";
		
		getline(myfile,linha); //linha ignorada
		sec=0;
		i=0;
		while ( getline (myfile,linha) ){
			double T, X, Y;
			double deltaX, deltaY, distancia;
			formatar(linha, &T, &X, &Y);
			if(i > 0 && T==0) continue;

			t.push_back(T);
			x.push_back(X);
			y.push_back(Y);
			
			vx.push_back(velX(T));
			vy.push_back(velY(T));
			ax.push_back(-0.01);
			ay.push_back(-0.016);
			
			deltaX = abs(abs(x[i])-abs(RX));
			deltaY = abs(abs(y[i])-abs(RY));
			distancia = sqrt((pow(deltaX,2)+pow(deltaY,2)));

			//sorvetao(x[0], vx[0], t[i], ax[i]);
			//tempoSorvetao(x[0], vx[0], x[i], ax[i]);
			outfile << t[i] << ";";
			outfile << x[i] << ";";
			outfile << y[i] << ";";
			outfile << vx[i] << ";";
			outfile << vy[i] << ";";
			outfile << ax[i] << ";";
			outfile << ay[i] << ";";
			outfile << distancia << ";";
			outfile << RX << ";";
			outfile << RY << "\n";
			if(distancia <= RAIO) break;
			
			if(i==0){
				for(double tempo=0; tempo<=20; tempo+=0.02){
					sorvetao(x[0], y[0], vx[0], vy[0], ax[0], ay[0], tempo, RX, RY, Spotencial[secToIndex[tempo]]);
				}
			}
			//Sleep(20);
			i++;
			sec+=0.02;
		}
		myfile.close();
		outfile.close();
		cout << "O arquivo foi lido" << endl;
		cout << "Deseja abrir o arquivo Excel_file - S ou N" << endl;
		string abrir;
		cin >> abrir;
		if(abrir[0]=='S' || abrir[0]=='s') system("Excel_file.csv");
	}
	else cout << "Nao foi possivel abrir o arquivo" << endl;
	// FIM DA LEITURA

	system("pause");
	return 0;
}
