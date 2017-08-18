#include <cekeikon.h>
#include <sstream> //biblioteca do opencv para converter float para string


//Função canny utilizada para detectar arestas
Mat_<GRY> canny(Mat_<GRY> a)
{ Mat_<GRY> b;
 Canny(a, b, 160, 70);
 return b;
}

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
 		Mat_<GRY> frame;
 		vi.set(CV_CAP_PROP_POS_FRAMES, aux); //VideoCapture::set set the index to the next frame to be captured
 		vi >> frame; // pega o proximo quadro do video
 		

 		//Para pegar o índice do quadro e salvar no nome da imagem
 		std::stringstream ss;
		ss << num_frames;
		std::string i = ss.str();
		std::string label = "gera_quad"+i+".jpg"; //concatena strings para salvar o nome certo
		std::string label_canny = "canny"+i+".jpg"; //concatena strings para salvar o nome certo
 		
 		imp(frame, label); //salva os quadros gerados
 		
 		//Chama a função canny para detectar arestas
 		Mat_<GRY> canny_frame=canny(frame);
 		imp(canny_frame, label_canny);

 		num_frames +=1;
 	}
 
} 
