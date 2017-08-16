#include <cekeikon.h>
#include <sstream> //biblioteca do opencv para converter float para string

int main() {

	//Lê vídeo desejado em vi
 VideoCapture vi("teste.avi");
 if (!vi.isOpened()) erro("Erro abertura video entrada");

 //cria  matriz colorida para salvar quadros
 Mat_<COR> a;
 
 int total_frames = 10; //numero total de quadros
 int step = vi.get(CV_CAP_PROP_FRAME_COUNT)/total_frames; //distancia necessaria para pegar 10 quadros
 
 while (true) {
 	for(int num_frames = 0; num_frames < total_frames; num_frames++){
 		vi >> a; // pega o proximo quadro do video
 		imp(a, "quad.jpg");
 	}
 }
} 
