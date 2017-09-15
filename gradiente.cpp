#include <cekeikon.h>

int main(int argc, char** argv) {
 if (argc!=3 && argc!=4) {
 printf("Gradiente: Calcula gradiente de Mat_<COR> como Mat_<CPX>\n");
 printf("GradienG ent.pgm sai.img [gaussDev]\n");
 xerro1("Erro: Numero de argumentos invalido");
 } 

 Mat_<COR> a;
 le(a,argv[1]);

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


	//Passar cada componente RGB para níveis de cinza para aplicar o gradiente
Mat_<GRY> red_gry(a.rows, a.cols);
Mat_<GRY> blue_gry(a.rows, a.cols);
Mat_<GRY> green_gry(a.rows, a.cols);

cvtColor(red,red_gry,CV_BGR2GRAY); //função pronta do OpenCV que converte imagem colorida para níveis de cinza
cvtColor(blue,blue_gry,CV_BGR2GRAY);
cvtColor(green,green_gry,CV_BGR2GRAY);

//aplicar o gradiente: apostila "hough.pdf", programa: gradieng.cpp 	
 if (argc==4) {
 double gaussDev;
 convArg(gaussDev,argv[3]);

 //aplica desvio gaussinaoo e calcula o gradiente de cada componente da imagem
 	//red
 GaussianBlur(red_gry,red_gry,Size(0,0),gaussDev,gaussDev);
 Mat_<CPX> red_cpx=gradienteScharr(red_gry,true); // true=y para cima
 Mat_<COR> redc = campox(red_cpx, 31, 15);
 imp(redc,"red_cpx.png");

  	//blue
 GaussianBlur(blue_gry,blue_gry,Size(0,0),gaussDev,gaussDev);
 Mat_<CPX> blue_cpx=gradienteScharr(blue_gry,true); // true=y para cima
 Mat_<COR> bluec = campox(blue_cpx, 31, 15);
 imp(bluec,"blue_cpx.png");

 	//green
 GaussianBlur(green_gry,green_gry,Size(0,0),gaussDev,gaussDev);
 Mat_<CPX> green_cpx=gradienteScharr(green_gry,true); // true=y para cima
 Mat_<COR> greenc = campox(green_cpx, 31, 15);
 imp(greenc,"green_cpx.png");

 //compara os gradientes de cada uma das componentes e cria uma imagem com os maiores gradientes encontrados
 Mat_<CPX> gradiente(a.rows, a.cols);

 for(int l=0; l<a.rows; l++){
	for(int c=0; c<a.cols; c++){
		if(abs(red_cpx(l,c)) > abs(blue_cpx(l,c)) && abs(red_cpx(l,c)) > abs(green_cpx(l,c))){ gradiente(l,c) = abs(red_cpx(l,c));}
		else if(abs(blue_cpx(l,c)) > abs(red_cpx(l,c)) && abs(blue_cpx(l,c)) > abs(green_cpx(l,c))) gradiente(l,c) = abs(blue_cpx(l,c));
		else gradiente(l,c) = abs(green_cpx(l,c));
	}
 }

 Mat_<COR> gradc = campox(gradiente, 31, 15);
 imp(gradc,argv[2]);



}
 
}	
