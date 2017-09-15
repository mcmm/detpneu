	//<<<<<<<< Programa GradienG (incluído na KCEK > 5.26) <<<<<<<<<
	//Funciona para Cekeikon > 5.26
	//gradieng.cpp
#include <cekeikon.h>
int main(int argc, char** argv) {
 /*if (argc!=3 && argc!=4) {
 printf("GradienG: Calcula gradiente de Mat_<GRY> como Mat_<CPX>\n");
 printf("GradienG ent.pgm sai.img [gaussDev]\n");
 xerro1("Erro: Numero de argumentos invalido");
 } */
 	//aplica o filtro gaussiano e deixa usando brilho como magnitude e cor
		//como direção do gradiente. A direção do gradiente é mais ou menos constante numa aresta.
 Mat_<COR> a;
 le(a,"fillcircle.png");

 //Dividir a imagem em componentes rgb
Mat_<COR> red(a.rows, a.cols);
Mat_<COR> blue(a.rows, a.cols);
Mat_<COR> green(a.rows, a.cols);

for(int l=0; l<a.rows; l++){
	for(int c=0; c<a.cols; c++){
		red(l,c) = COR(0, 0, a(l,c)[2]);
		green(l,c) = COR(0, a(l,c)[1], 0); 
		blue(l,c) = COR(a(l,c)[0], 0, 0);
	}
}

//aplicar o gradiente  	

 double gaussDev;
 int desvio =2; //2 é o desvio Gaussiano (tirado da apostila)
 convArg(gaussDev,argv[1]); 

 GaussianBlur(a,a,Size(10,10),gaussDev,gaussDev);
 Mat_<CPX> a_cpx=gradienteScharr(a,true); // true=y para cima
 imp(a_cpx,"a_cpx.png");

/*
 //calcula o gradiente de cada componente da imagem
 	//red
 GaussianBlur(red,red,Size(0,0),gaussDev,gaussDev);
 Mat_<CPX> red_cpx=gradienteScharr(red,true); // true=y para cima
 imp(red_cpx,"red_cpx.png");

 	//green
 GaussianBlur(green,green,Size(0,0),gaussDev,gaussDev);
 Mat_<CPX> green_cpx=gradienteScharr(green,true); // true=y para cima
 imp(green_cpx,"green_cpx.png");

  	//blue
 GaussianBlur(blue,blue,Size(0,0),gaussDev,gaussDev);
 Mat_<CPX> blue_cpx=gradienteScharr(blue,true); // true=y para cima
 imp(blue_cpx,"blue_cpx.png"); */
 
}	
