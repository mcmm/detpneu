#include <cekeikon.h>

int main(int argc, char** argv){

	Mat_<COR> colorida; le(colorida, argv[1]);
	int centro_l = atoi(argv[2]);
	int centro_c = atoi(argv[3]);
	int raio = atoi(argv[4]);
	circulo(colorida, centro_l, centro_c, raio, COR(255, 0, 0),3);
	ponto(colorida, centro_l, centro_c,COR(0,255,0), 5);
	imp(colorida, "colorida.png");

}