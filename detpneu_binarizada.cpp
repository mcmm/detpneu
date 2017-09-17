#include <cekeikon.h>
#include <sstream> //biblioteca do opencv para converter float para string

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

	//Para capturar quadros (referências):
		//apostila "video.pdf"
			//códigos: "leimp.cpp" (pos-2014), "levideo.cpp" (pos-2013)
 		//http://answers.opencv.org/question/5768/how-can-i-get-one-single-frame-from-a-video-file/

	//Lê vídeo desejado em vi
 VideoCapture vi(argv[1]);
 if (!vi.isOpened()) erro("Erro abertura video entrada");
 
 int nc=vi.get(CV_CAP_PROP_FRAME_WIDTH);
 int nl=vi.get(CV_CAP_PROP_FRAME_HEIGHT);
 int fps=vi.get(CV_CAP_PROP_FPS);
 
 VideoWriter vo("saida_.avi",
 CV_FOURCC('X','V','I','D'),
 fps,Size(nc,nl));
 if (!vo.isOpened()) erro("Erro abertura video saida");


int total_frames = vi.get(CV_CAP_PROP_FRAME_COUNT); //numero total de quadros
int passo = 1;
int terminar=0;
while(true){
 		if (terminar>= total_frames) break;
 	for(int aux = 0; aux < total_frames; aux+=passo){ //deseja-se percorrer todas as frames, 
 		terminar += passo;
 													//adiciona-se 150 para pegar mais ou menos 10 quadros
 		 //cria  matriz para salvar quadros
 		Mat_<COR> frame;
 		vi.set(CV_CAP_PROP_POS_FRAMES, aux); //VideoCapture::set set the index to the next frame to be captured
 		vi >> frame; // pega o proximo quadro do video - de indice definido no set
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
		int rmax = 70;
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


		int num_escuros =6;
		int num_escuros_atual=0;
		int melhor_raio;
		float min_limiar = 0.2;
		float max_linear = 0.8;

		for(int i=0; i<sai.size(); i++){
			num_escuros_atual =0;
    		normalize( sai[i], sai[i], 0, 1, NORM_MINMAX, -1, Mat() ); // Normalizes the norm or value range of an array.
    		Mat_<FLT> binarizada(sai[i].size());
			for (int l=0; l<sai[i].rows; l++){
				for (int c=0; c<sai[i].cols; c++) {
					if (sai[i](l,c)<=min_limiar) binarizada(l,c)=0;
					else binarizada(l,c)=255;
				}
			}
   		for (int l=0; l<nl; l++){
 			for (int c=0; c<nc; c++){
 				//printf("pixel: %f\n", sai[i](l,c));
 				if(l!=0&&c!=0&&l!=nl&&c!=nc){ //para não pegar fora da imagem
 					if(binarizada(l,c) == 0) { 

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

		if(num_escuros<6 && melhor_raio != rmin && melhor_raio != rmax){
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

			printf("centro: linha %d coluna %d pixel %f raio %d escuro: %d\n", centro_l, centro_c, sai[melhor_raio](centro_l, centro_c), melhor_raio+rmin, num_escuros);
			//printf(" );

			circulo(frame, centro_l, centro_c, melhor_raio+rmin, COR(0, 255, 255),3);
			ponto(frame, centro_l, centro_c,COR(0,255,0), 5);
		}
		vo << frame;
 	}
 }
}
 
