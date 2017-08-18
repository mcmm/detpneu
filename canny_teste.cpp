#include <cekeikon.h>
Mat_<GRY> canny(Mat_<GRY> a)
{ Mat_<GRY> b;
 Canny(a, b, 160, 70);
 return b;
}
int main(int argc, char** argv)
{ if (argc!=3) erro("Canny entrada.jpg saida.jpg");
 Mat_<GRY> a; le(a,argv[1]);
 Mat_<GRY> m=canny(a);
 imp(m,argv[2]);
} 