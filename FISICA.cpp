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


struct Robo{	
	vector<double> x, y, vx, vy, ax, ay, p;
};

struct Bola{
	vector<double> x, y, vx, vy, ax, ay;
};

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

double posicao(double S0, double V0, double a, double t){
	double S = S0 + V0*t + a*pow(t,2);
	return S;
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

double teste(double t, struct Robo r, struct Bola b, int i, double* dist, double* dX, double* dY){
	/*
	cout << "Dentro do SVT t=" << t << endl;
	cout << "bx0 = " << b.x[0] << endl;
	cout << "by0 = " << b.y[0] << endl;
	cout << "bvx0 = " << b.vx[0] << endl;
	cout << "bvy0 = " << b.vy[0] << endl;
	cout << "bax0 = " << b.ax[0] << endl;
	cout << "bay0 = " << b.ay[0] << endl;
	cout << "rx0 = " << r.x[0] << endl;
	cout << "ry0 = " << r.y[0] << endl;
	cout << "rp[" << i << "] = " << r.p[i] << endl;
	*/
	double Sx = posicao(b.x[0], b.vx[0], b.ax[0], t);
	double Sy = posicao(b.y[0], b.vy[0], b.ay[0], t);
	*dX = abs(abs(Sx)-abs(r.x[0]));
	*dY = abs(abs(Sy)-abs(r.y[0]));
	*dist = sqrt((*dX* *dX)+(*dY * *dY));
	//cout << "dist =" << *dist << endl;
	double dif = abs( abs(*dist) - abs(r.p[i]));
	
	if(Sx<r.x[0]) *dX = -1 * *dX;
	if(Sy<r.y[0]) *dY = -1 * *dY;
	
	//cout << "dif = " << dif << "; em t = " << t << endl;
	if(dif<=RAIO) return t;
	else return -1;
}

int tempoSorvetao(double S0, double V0, double S, double a){
	double t = sqrt((2*(S-S0))/(2*(V0+a)));
	cout << "t = " << t << "; em S = " << S << endl;
}

// Km Hm Dam m Dm Cm Mm
int main(){
	double tempoDeInt = -1;
	struct Bola b;
	struct Robo r;
	vector<double> t;
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
		//cout << fixed << "sec = " << sec << " S = " << movimentoDoRobo(sec, tempoAce, tempoDesace) << endl;
		r.p.push_back(movimentoDoRobo(sec, tempoAce, tempoDesace));
		i++;
	}
	srand (time(NULL));
	double total;
	do{
		cout << "Digite as cordenadas X e Y do robo:" << endl;
		cin >> RX >> RY;
		double restoX = abs(1.000-abs(RX)), restoY = abs(0.500-abs(RY));
		total = sqrt((abs(restoX)*abs(restoX))+(abs(restoY)*abs(restoY)));
	}while(total>1.000);
	/*
	do{
		int random;
		random = rand() % 2000;
		RX = (random)/1000.0;
		random = rand() % 2000;
		RY = (random-500)/1000.0;
		double restoX = abs(1.000-abs(RX)), restoY = abs(0.500-abs(RY));
		total = sqrt((abs(restoX)*abs(restoX))+(abs(restoY)*abs(restoY)));
	}while(total>1.000);
	*/
	
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
		double dist=0, distX=0, distY=0;
		double T, X, Y;
		double deltaX, deltaY, distanciaRB, razao=0.0;
		while ( getline (myfile,linha) ){
			formatar(linha, &T, &X, &Y);
			if(i > 0 && T==0) continue;

			t.push_back(T);
			
			b.x.push_back(X);
			b.y.push_back(Y);
			b.vx.push_back(velX(T));
			b.vy.push_back(velY(T));
			b.ax.push_back(-0.01);
			b.ay.push_back(-0.016);

			if(i==0){
				cout << "Entrou no i==0" << endl;
				r.x.push_back(RX);
				r.y.push_back(RY);
				r.vx.push_back(0);
				r.vy.push_back(0);
				int aux=0;
				for(double tempo=0; tempo<=20; tempo+=0.02){
					if(tempoDeInt=teste(tempo, r, b, aux, &dist, &distX, &distY)!=-1){
						cout << "ENCONTRADO PONTO DE INTERSECCAO" << endl;
						break;
					}
					aux++;
				}
				cout << "tempo de int = " << tempoDeInt << endl;
				cout << "disttt  =  " << dist << endl;
			}
			else{
				r.x.push_back(RX+(distX*razao));
				r.y.push_back(RY+(distY*razao));
			}
			deltaX = abs(abs(b.x[i])-abs(r.x[i]));
			deltaY = abs(abs(b.y[i])-abs(r.y[i]));
			distanciaRB = sqrt((pow(deltaX,2)+pow(deltaY,2)));
			
			if(i==0){
				if(distanciaRB<=RAIO){
					r.ax.push_back(0);
					r.ay.push_back(0);
				}
				else{
					r.ax.push_back(AMAX);
					r.ay.push_back(AMAX);
				}
			}
			
			razao = r.p[i+1]/dist;
			cout << r.p[i+1] << "/" << dist << endl;
			cout << "razao = " << razao << endl;
			//sorvetao(x[0], vx[0], t[i], ax[i]);
			//tempoSorvetao(x[0], vx[0], x[i], ax[i]);
			outfile << t[i] << ";";
			outfile << b.x[i] << ";";
			outfile << b.y[i] << ";";
			outfile << b.vx[i] << ";";
			outfile << b.vy[i] << ";";
			outfile << b.ax[i] << ";";
			outfile << b.ay[i] << ";";
			outfile << distanciaRB << ";";
			outfile << r.x[i] << ";";
			outfile << r.y[i] << "\n";
			if(distanciaRB <= RAIO) break;
			
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
