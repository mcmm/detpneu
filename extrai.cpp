#include <cekeikon.h>
#include <sstream> //biblioteca do opencv para converter float para string
int main(int argc, char** argv) {
	double ti, tf, time; //variáveis para medir tempo inicial e final
	ti =getTickCount(); //tempo inicial
	//Para capturar quadros (referências):
		//apostila "video.pdf"
			//códigos: "leimp.cpp" (pos-2014), "levideo.cpp" (pos-2013)
 		//http://answers.opencv.org/question/5768/how-can-i-get-one-single-frame-from-a-video-file/

	//Lê vídeo desejado em vi
 VideoCapture vi(argv[1]);
 if (!vi.isOpened()) erro("Erro abertura video entrada");

 int total_frames = vi.get(CV_CAP_PROP_FRAME_COUNT); //numero total de quadros
 int num_frames = 0; //variavel que guarda número de quadros
 int passo = total_frames/10;

 	for(int aux = 0; aux < total_frames; aux+=passo){ //deseja-se percorrer todas as frames, 
 													//adiciona-se 150 para pegar mais ou menos 10 quadros
 		 //cria  matriz para salvar quadros
 		Mat_<GRY> frame;
 		vi.set(CV_CAP_PROP_POS_FRAMES, aux); //VideoCapture::set set the index to the next frame to be captured
 		vi >> frame; // pega o proximo quadro do video - de indice definido no set
 	
 		//Para pegar o índice do quadro e salvar no nome da imagem
 		std::stringstream ss, ss1;
		ss << num_frames;
		ss1 << argv[2]; 
		std::string i = ss.str();
		std::string j = ss1.str();
		std::string label = j+i+".png"; //concatena strings para salvar o nome certo
 		
 		imp(frame, label); //salva os quadros gerados
 		num_frames +=1;
 	}
	tf =getTickCount(); //tempo final
	time = (tf - ti)/getTickFrequency();
	printf("%fs para executar",time);
 }