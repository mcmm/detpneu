	#include <cekeikon.h>


int main(int argc, char** argv) {
	Mat_<FLT> entrada;
	le(entrada, argv[1]);
	Mat_<FLT> binarizada(entrada.size());
	for (int l=0; l<entrada.rows; l++){
		for (int c=0; c<entrada.cols; c++) {
			if (entrada(l,c)<=0.2) binarizada(l,c)=0;
			else binarizada(l,c)=255;
		}
	}
	imp(binarizada, "binarizada.png");
}