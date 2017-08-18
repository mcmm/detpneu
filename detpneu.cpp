#include <cekeikon.h>
#include <sstream> //biblioteca do opencv para converter float para string

int main() {

	//Lê vídeo desejado em vi
 VideoCapture vi("teste.avi");
 if (!vi.isOpened()) erro("Erro abertura video entrada");

 //Link de referência:
 //http://answers.opencv.org/question/5768/how-can-i-get-one-single-frame-from-a-video-file/

 int total_frames = vi.get(CV_CAP_PROP_FRAME_COUNT); //numero total de quadros
 //int step = vi.get(CV_CAP_PROP_FRAME_COUNT)/total_frames; //distancia necessaria para pegar 10 quadros
 int num_frames = 0;
 
 
 	for(int aux = 0; aux < total_frames; aux+=150){ //deseja-se pegar 10 quadros
 		 //cria  matriz colorida para salvar quadros
 		Mat_<COR> a;
 		vi.set(CV_CAP_PROP_POS_FRAMES, aux); //VideoCapture::set set the index to the next frame to be captured
 		vi >> a; // pega o proximo quadro do video
 		

 		//Para pegar o índice do quadro e salvar no nome da imagem
 		std::stringstream ss;
		ss << num_frames;
		std::string i = ss.str();
		std::string label = "gera_quad"+i+".jpg"; //concatena strings para salvar o nome certo
 		
 		imp(a, label);
 		num_frames +=1;
 	}
 
} 
