#include <cekeikon.h>

Mat_<GRY> canny(Mat_<GRY> a)
{ Mat_<GRY> b;
 Canny(a, b,200, 100);
 return b;
}

int main(int argc, char** argv) {
 double ti, tf, time; //variáveis para medir tempo inicial e final
ti =getTickCount(); //tempo inicial

Mat_<CPX> entrada;
le(entrada, argv[1]);
Mat_<GRY> saida(entrada.rows, entrada.cols);
double maior_modulo=0;

for(int l=0; l<entrada.rows; l++){
	for(int c=0; c<entrada.cols; c++){
		if (abs(entrada(l,c))>maior_modulo)	maior_modulo = abs(entrada(l,c)); 
	}
}


double limiar_min = (0.2*maior_modulo); 
double limiar_max = (0.4*maior_modulo); 
//xdebug;

for(int l=0; l<entrada.rows; l++){
	for(int c=0; c<entrada.cols; c++){
		if (abs(entrada(l,c))>limiar_min && abs(entrada(l,c))<limiar_max){
			saida(l,c)=0;
			//xdebug;
		}
		else {
			saida(l,c)=255;
			//xdebug;
		}  
	}
}

imp(saida, argv[2]);

tf =getTickCount(); //tempo final
time = (tf - ti)/getTickFrequency();
printf("%fs para executar",time);
}
 