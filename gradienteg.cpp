	//<<<<<<<< Programa GradienG (incluído na KCEK > 5.26) <<<<<<<<<
	//Funciona para Cekeikon > 5.26
	//gradieng.cpp
#include <cekeikon.h>
int main(int argc, char** argv) {
 if (argc!=3 && argc!=4) {
 printf("GradienG: Calcula gradiente de Mat_<GRY> como Mat_<CPX>\n");
 printf("GradienG ent.pgm sai.img [gaussDev]\n");
 xerro1("Erro: Numero de argumentos invalido");
 }
 	//aplica o filtro gaussiano e deixa usando brilho como magnitude e cor
		//como direção do gradiente. A direção do gradiente é mais ou menos constante numa aresta.
 Mat_<GRY> a;
 le(a,argv[1]);
 if (argc==4) {
 double gaussDev;
 convArg(gaussDev,argv[3]);
 GaussianBlur(a,a,Size(0,0),gaussDev,gaussDev);
 }
 Mat_<CPX> b=gradienteScharr(a,true); // true=y para cima
 imp(b,argv[2]);
}
/*
//<<<<<<<< Função gradienteScharr (incluído na Cekeikon > 5.26) <<<<<<<<<
Mat_<CPX> criaCPX(Mat_<FLT> dc, Mat_<FLT> dl)
{ if (dc.size()!=dl.size()) erro("Erro criaCPX");
 Mat_<CPX> b(dc.size());
 for (unsigned i=0; i<b.total(); i++)
 b(i)=CPX( dc(i), dl(i) );
 return b;
}
Mat_<CPX> gradienteScharr(Mat_<GRY> a, bool yParaCima)
// x ->
// y |
// V
{ Mat_<FLT> gradientex;
 Scharr(a, gradientex, CV_32F, 1, 0);
 // gradientex entre -4080 e +4080 (255*16)?
 gradientex=(1.0/4080)*gradientex;
 Mat_<FLT> gradientey;
 Scharr(a, gradientey, CV_32F, 0, 1);
 // bl entre -4080 e +4080?
 if (yParaCima) gradientey=(-1.0/4080)*gradientey;
 else gradientey=(1.0/4080)*gradientey;
 Mat_<CPX> b=criaCPX(gradientex,gradientey);
 return b; // b.real entre -1 e +1. b.imag tambem
}
*/