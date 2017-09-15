#include <cekeikon.h>
#include <sstream> //biblioteca do opencv para converter float para string

//Função canny utilizada para detectar arestas
	//código da apostila "corner.pdf"
	//última página
Mat_<GRY> canny(Mat_<GRY> a)
{ Mat_<GRY> b;
 Canny(a, b,200, 100);
 return b;
}

int main(int argc, char** argv) {
	double ti, tf, time; //variáveis para medir tempo inicial e final
	ti =getTickCount(); //tempo inicial
 	Mat_<GRY> frame;
 	le(frame,argv[1]);
 
	//Chama a função canny para detectar arestas
 	Mat_<GRY> canny_frame=canny(frame);

 	imp(canny_frame, argv[2]);
	tf =getTickCount(); //tempo final
	time = (tf - ti)/getTickFrequency();
	printf("%fs para executar",time);
}