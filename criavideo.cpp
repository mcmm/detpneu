//criavideo.cpp
//pos-2014
#include <cekeikon.h>
int main() {
 VideoWriter vo("teste.avi",
 CV_FOURCC('X','V','I','D'),
 30,
Size(320,240));
 Mat_<COR> a(240,320);
 for (int q=0; q<200; q++) {
 for (int l=0; l<a.rows; l++)
 for (int c=0; c<a.cols; c++)
 a(l,c)=COR((2*q)%256,(l+q)%256,(c+q)%256);
 vo << a;
 }
}
