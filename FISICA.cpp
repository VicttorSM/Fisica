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
	//cout << "sT = " << sT << endl << "sX = " << sX << endl << "sY = " << sY << endl << endl;
	*T = stringToDouble(sT);
	*X = stringToDouble(sX);
	*Y = stringToDouble(sY);
}

//FUNÇÃO DE X
//y = -0.005x² + 0.5x + 1

//DERIVADA (Velocidade X)
//y = -0.01x + 0.5

//DERIVADA DE DERIVADA (Aceleração X)
//y = -0.01

//FUNÇÃO DE Y
//y = -0.008x² + 0.4x + 0.5

//DERIVADA (Velocidade Y)
//y = -0.016x + 0.4

//DERIVADA DE DERIVADA (Aceleração Y)
//y = -0.016

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

double cotan(double num){
	return(1 / tan(num));
}

void movimentoDoRobo(int t){
	double vMax = 2; // m/s
	double aMax = 0.2; // m/s²
	double tempoAce = vMax/aMax;
	double tempoDesace = tempoAce;
}

int sorvetao(double S0, double V0, double t, double a){
	double S = S0 + V0*t + (a*t*t)/2;
	cout << "S = " << S << "; em t = " << t << endl;
}

int tempoSorvetao(double S0, double V0, double S, double a){
	double t = sqrt((2*(S-S0))/(2*(V0+a)));
	cout << "t = " << t << "; em S = " << S << endl;
}

// Km Hm Dam m Dm Cm Mm
int main(){
	//A max = 0.05 m/s²
	//V max = 0.10 m/s
	//Raio de interceptação: 0.20 m
	//Coef Angular = deltaY / deltaX
	//

	vector<double> t, x, y, vx, vy, ax, ay, Rx, Ry, Rvx, Rvy, Rax, Ray;
	map<int, double> indexToSec;
	map<double, int> secToIndex;
	int i;
	double sec;
	string linha;
	double Amax = 0.05, Vmax = 0.10;
	double RX, RY, RAIO=0.2;
	srand (time(NULL));
	//cout << "Digite as cordenadas X e Y do robo:" << endl;
	//cin >> RX >> RY;
	double total;
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

			/*
			vax =
			vay =
			xa = vax * t;
			ya = vay * t;
			vbx =
			vby =
			d =
			xb = vbx * t * d;
			yb = vby * t;

			xintersecao = ((-vby/vbx)*d)/((vay/vax)-(vby/vbx));

			yintersecao = (vay/vax) * xintersecao;
			*/
			double T, X, Y;
			double deltaX, deltaY, distancia;
			secToIndex[sec] = i;
			indexToSec[i] = sec;
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
			tempoSorvetao(x[0], vx[0], x[i], ax[i]);
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
