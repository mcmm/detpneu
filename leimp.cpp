//leimp.cpp: Le e grava video.
//pos-2014
#include <cekeikon.h>
int main() {
 VideoCapture vi("teste.avi");
 if (!vi.isOpened()) erro("Erro abertura video entrada");
 VideoWriter vo("saida.avi",
CV_FOURCC('X','V','I','D'),
30,Size(320,240));
 //if (!vo.isOpened()) erro("Erro abertura video saida");
 Mat_<COR> a;
 //namedWindow("janela",0);
 while (true) {
 vi >> a;
 //imshow("janela",a);
 //vo << a;
 	for(int i = 0; i<)
 	le(a, "gera quad %d", i)

 //if (waitKey(30)>=0) break;
 }
} 
