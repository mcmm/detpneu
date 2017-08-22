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

	//Para capturar quadros (referências):
		//apostila "video.pdf"
			//códigos: "leimp.cpp" (pos-2014), "levideo.cpp" (pos-2013)
 		//http://answers.opencv.org/question/5768/how-can-i-get-one-single-frame-from-a-video-file/

	//Lê vídeo desejado em vi
 VideoCapture vi("teste.avi");
 if (!vi.isOpened()) erro("Erro abertura video entrada");

 int total_frames = vi.get(CV_CAP_PROP_FRAME_COUNT); //numero total de quadros
 int num_frames = 0; //variavel que guarda número de quadros
 
 	for(int aux = 0; aux < total_frames; aux+=150){ //deseja-se percorrer todas as frames, 
 													//adiciona-se 150 para pegar mais ou menos 10 quadros
 		 //cria  matriz para salvar quadros
 		Mat_<GRY> frame;
 		vi.set(CV_CAP_PROP_POS_FRAMES, aux); //VideoCapture::set set the index to the next frame to be captured
 		vi >> frame; // pega o proximo quadro do video - de indice definido no set
 	
 		//Para pegar o índice do quadro e salvar no nome da imagem
 		std::stringstream ss;
		ss << num_frames;
		std::string i = ss.str();
		std::string label = "gera_quad"+i+".png"; //concatena strings para salvar o nome certo
		std::string label_canny = "canny"+i+".png"; //concatena strings para salvar o nome certo
 		
 		imp(frame, label); //salva os quadros gerados
 		
 		//Chama a função canny para detectar arestas
 		Mat_<GRY> canny_frame=canny(frame);

 		//Faz o negativo da imagem canny antes de salvar - necessário para aplicar a transformada de Hough
 		//código da apostila "basico.pdf"
 		 // inv_cek.cpp grad2015
 		for (int l=0; l<canny_frame.rows; l++)
 			for (int c=0; c<canny_frame.cols; c++)
 				if (canny_frame(l,c)==0) canny_frame(l,c)=255;
 				else canny_frame(l,c)=0;

 		imp(canny_frame, label_canny);

 		num_frames +=1;
 	}
}
 
