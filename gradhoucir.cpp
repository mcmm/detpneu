#include <cekeikon.h>

void subt(vector< Mat_<FLT> >& b, int s, int l, int c) {
 if (0<=s && s<int(b.size()) && 0<=l && l<b[0].rows && 0<=c && c<b[0].cols)
 b[s](l,c)-=1.0;
}
void subtraicirculo(vector< Mat_<FLT> >& b, int s, int l, int c, int r) {
 int x=0; int y=r; int limite=r;
 while (y>0) {
 subt(b,s,l+x,c+y);
 subt(b,s,l+y,c-x);
 subt(b,s,l-x,c-y);
 subt(b,s,l-y,c+x);
 int mh=abs(elev2(x+1)+elev2(y)-elev2(r));
 int md=abs(elev2(x+1)+elev2(y-1)-elev2(r));
 int mv=abs(elev2(x)+elev2(y-1)-elev2(r));
 if (mh<=md && mh<=mv) x++;
 else if (md<=mv) { x++; y--; }
 else y--;
 }
}

int main(int argc, char** argv) {
 double ti, tf, time; //variáveis para medir tempo inicial e final
ti =getTickCount(); //tempo inicial

 Mat_<COR> frame;
 le(frame,argv[1]);

 //Dividir a imagem em componentes rgb
Mat_<COR> red(frame.rows, frame.cols);
Mat_<COR> blue(frame.rows, frame.cols);
Mat_<COR> green(frame.rows, frame.cols);

for(int l=0; l<frame.rows; l++){
	for(int c=0; c<frame.cols; c++){
		red(l,c) = COR(0, 0, frame(l,c)[2]);
		green(l,c) = COR(0, frame(l,c)[1], 0); 
		blue(l,c) = COR(frame(l,c)[0], 0, 0);
	}
}


	//Passar cada componente RGB para níveis de cinza para aplicar o gradiente
Mat_<GRY> red_gry(frame.rows, frame.cols);
Mat_<GRY> blue_gry(frame.rows, frame.cols);
Mat_<GRY> green_gry(frame.rows, frame.cols);

cvtColor(red,red_gry,CV_BGR2GRAY); //função pronta do OpenCV que converte imagem colorida para níveis de cinza
cvtColor(blue,blue_gry,CV_BGR2GRAY);
cvtColor(green,green_gry,CV_BGR2GRAY);

//aplicar o gradiente: apostila "hough.pdf", programa: gradieng.cpp 	
 
 double gaussDev;
 char desvio = '2';
 convArg(gaussDev,argv[2]);

 //aplica desvio gaussinaoo e calcula o gradiente de cada componente da imagem
 	//red
 GaussianBlur(red_gry,red_gry,Size(0,0),gaussDev,gaussDev);
 Mat_<CPX> red_cpx=gradienteScharr(red_gry,true); // true=y para cima
 Mat_<COR> redc = campox(red_cpx, 31, 15);
 //imp(redc,"red_cpx.png");

  	//blue
 GaussianBlur(blue_gry,blue_gry,Size(0,0),gaussDev,gaussDev);
 Mat_<CPX> blue_cpx=gradienteScharr(blue_gry,true); // true=y para cima
 Mat_<COR> bluec = campox(blue_cpx, 31, 15);
 //imp(bluec,"blue_cpx.png");

 	//green
 GaussianBlur(green_gry,green_gry,Size(0,0),gaussDev,gaussDev);
 Mat_<CPX> green_cpx=gradienteScharr(green_gry,true); // true=y para cima
 Mat_<COR> greenc = campox(green_cpx, 31, 15);
 //imp(greenc,"green_cpx.png");

 //compara os gradientes de cada uma das componentes e cria uma imagem com os maiores gradientes encontrados
 Mat_<CPX> gradiente(frame.rows, frame.cols);

 for(int l=0; l<frame.rows; l++){
	for(int c=0; c<frame.cols; c++){
		if(abs(red_cpx(l,c)) > abs(blue_cpx(l,c)) && abs(red_cpx(l,c)) > abs(green_cpx(l,c))){ gradiente(l,c) = abs(red_cpx(l,c));}
		else if(abs(blue_cpx(l,c)) > abs(red_cpx(l,c)) && abs(blue_cpx(l,c)) > abs(green_cpx(l,c))) gradiente(l,c) = abs(blue_cpx(l,c));
		else gradiente(l,c) = abs(green_cpx(l,c));
	}
 }

Mat_<GRY> hougrad(gradiente.rows, gradiente.cols);
double maior_modulo=0;

for(int l=0; l<gradiente.rows; l++){
	for(int c=0; c<gradiente.cols; c++){
		if (abs(gradiente(l,c))>maior_modulo)	maior_modulo = abs(gradiente(l,c)); 
	}
}


double limiar_min = (0.1*maior_modulo); 
double limiar_max = (0.2*maior_modulo); 
//xdebug;

for(int l=0; l<gradiente.rows; l++){
	for(int c=0; c<gradiente.cols; c++){
		if (abs(gradiente(l,c))>limiar_min && abs(gradiente(l,c))<limiar_max){
			hougrad(l,c)=0;
			//xdebug;
		}
		else {
			hougrad(l,c)=255;
			//xdebug;
		}  
	}
}

int rmin = 45;
int rmax = 80;
int nl=hougrad.rows;
int nc=hougrad.cols;
int ns=rmax-rmin+1;
 
 vector< Mat_<FLT> > sai(ns);
 
 for (int i=0; i<ns; i++) {
 	sai[i].create(nl,nc);
 	sai[i].setTo(0.0);
 }

 for (int l=0; l<nl; l++){
 	for (int c=0; c<nc; c++){
 		if (hougrad(l,c)==0) {
 			for (int r=rmin; r<=rmax; r++)
 				subtraicirculo(sai,r-rmin,l,c,r);
 		}
 	}
 }
imp(hougrad, "hougrad.png");

 int num_escuros =5;
 int num_escuros_atual=0;
 int melhor_raio;

for(int i=0; i<sai.size(); i++){
	num_escuros_atual =0;
    normalize( sai[i], sai[i], 0, 1, NORM_MINMAX, -1, Mat() ); // Normalizes the norm or value range of an array.
   for (int l=0; l<nl; l++){
 	for (int c=0; c<nc; c++){
 		//printf("pixel: %f\n", sai[i](l,c));
 		if(l!=0&&c!=0&&l!=nl&&c!=nc){ //para não pegar fora da imagem
 			if(sai[i](l,c)<0.2) { 

 				num_escuros_atual+=1;
 				//printf("ptos escuros: %d\n, num_escuros_atual");
 			}
 		}
 	}
 }
 	if (num_escuros_atual<num_escuros)
 	{
 		num_escuros = num_escuros_atual;
 		melhor_raio = i;
 	}
 }
 imp(normaliza(sai[melhor_raio]),"houmelhorhaeo.png");

int  centro_l=0;
int centro_c =0;
float centro =1;

//Procurar pixel mais escuro para definir centro
for (int l=0; l<nl; l++){
 	for (int c=0; c<nc; c++){
 		if(l!=0&&c!=0&&l!=nl&&c!=nc){ //para não pegar fora da imagem
 			if(sai[melhor_raio](l,c) <centro) { //vai procurando qual o pixel mais escuro e salvando
 											//teria que achar um jeito melhor
 				centro =sai[melhor_raio](l,c);
 				centro_l = l;
 				centro_c = c;
 			}
 		}
 	}
 }

printf("centro: linha %d coluna %d pixel %f raio %d \n", centro_l, centro_c, sai[melhor_raio](centro_l, centro_c), melhor_raio+rmin);
printf("escuro: %d\n", num_escuros );
circulo(frame, centro_l, centro_c, melhor_raio+rmin, COR(255, 0, 0),3);
ponto(frame, centro_l, centro_c,COR(0,255,0), 5);
imp(frame, "frame.png");

}