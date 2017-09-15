//houghgr.cpp
#include <cekeikon.h>
Mat_<CPX> anguloMaisMenosPi2(Mat_<CPX> a) {
 Mat_<CPX> d(a.size());
 for (unsigned i=0; i<a.total(); i++) {
 float rad=a(i).imag();
 if (rad>M_PI) rad -= M_PI;
 if (rad<=-M_PI) rad += M_PI;
 assert(-M_PI<rad && rad<=M_PI);
 if (rad<0) rad+=M_2PI;
 assert(0<=rad && rad<=2*M_PI);
 if (rad>M_PI) rad-=M_PI;
 assert(0<=rad && rad<=M_PI);

 if (rad>M_PI_2) rad-=M_PI;
 assert(-M_PI_2<=rad && rad<=M_PI_2);
 // rad entre -pi/2 e +pi/2
 d(i)=CPX( a(i).real(), rad );
 }
 return d;
}
void subtrai(ImgXyb<FLT>& sai, int x, int y, FLT val, int dx, int dy) {
 for (int x2=-dx; x2<=dx; x2++)
 for (int y2=-dy; y2<=dy; y2++)
 sai(x+x2,y+y2) -= val;
}
ImgXyb<FLT> houghGr(ImgXyb<GRY> ent, double resRho=1.0, double resTheta=1.0,
 double GaussDev=2.0, double minGrad=0.05) {
// Detecta retas usando gradiente da imagem, sem binarizar.
// ent = imagem em niveis de cinza (e nao binaria). Retas sao as arestas da imagem.
// resRho = pix_ent/pix_y_sai = pepys = distance resolution of the accumulator in pixels.
// resTheta = graus_theta/pix_x_sai = gtpxs = angle resolution of the accumulator in degrees.
// GaussDev = sigma ou desvio-padrao do filtro gaussiano (para calcular gradiente).
// minGrad = menor valor do modulo de gradiente para que seja processado.
// se for zero, todos os pixels sao processados.
// O maior modulo de gradiente e' 1.
// saida = imagem no espaco de Hough, normalizado entre 0 e 1.
 ent.centro();
 GaussianBlur(ent,ent,Size(0,0),GaussDev,GaussDev);

 ImgXyb<CPX> ret=gradienteScharr(ent,true); // y para cima
 ret.centro();
 ImgXyb<CPX> pol=ret2pol(ret); // pol com modulo e angulo em radianos
 // -pi<arg<=+pi
 pol=anguloMaisMenosPi2(pol); // -pi/2<arg<=+pi/2
 pol.centro();
 int nl2=teto( sqrt(double( elev2(ent.lc)+elev2(ent.cc)))/resRho );
 int nl=2*nl2+1;
 // indexacao de rho vai de -nl2 ent +nl2.
 // y=rho/resRho;
 int nc2=teto(90.0/resTheta);
 int nc=2*nc2+1;
 // indexacao de theta vai de -nc2 ent +nc2
 // x=theta/resTheta;
 ImgXyb<FLT> sai(nl,nc,0.0);
 sai.centro(nl2,nc2);
 for (int xa=pol.minx; xa<=pol.maxx; xa++) {
 for (int ya=pol.miny; ya<=pol.maxy; ya++) {
 if (pol(xa,ya).real()>=minGrad) {
 double rad=pol(xa,ya).imag();
 int rho=round((xa*cos(rad)+ya*sin(rad))/resRho);
 assert(-nl2<=rho && rho<=nl2);
 int theta=chao(rad2deg(rad)/resTheta);
 assert(-nc2<=theta && theta<=nc2);
 subtrai(sai,theta,rho,pol(xa,ya).real(),round(resTheta),round(resRho));
 //sai(theta,rho) -= pol(xa,ya).real();
 }
 }
 }
 sai=normaliza(sai);
 return sai;
}
int main(int argc, char** argv) {
 const char* keys =
 " curto| longo |default| explicacao\n"
 " -r | -rho | 1 | resolucao de distancia em pixels\n"
 " -t | -theta | 1 | resolucao de angulo em graus\n"
 " -d | -desvio | 2 | desvio do filtro Gaussiano\n"
 " -m | -mingrad | 0.05 | modulo do gradiente minimo para processar\n";
 if (argc<=1) {
 printf("HoughGr: Detecta retas usando gradiente da imagem, sem binarizar.\n");
 printf("HoughGr ent.pgm sai.img [opcoes]\n");
 printf("%s",keys);
 erro("Erro: Numero de argumentos invalido");
 }
 ArgComando cmd(argc,argv);
 string nomeent=cmd.getCommand(0);
 if (nomeent=="") erro("Erro: Nao especificou imagem de entrada");
 Mat_<GRY> ent; le(ent,nomeent);

 string nomesai=cmd.getCommand(1);
 if (nomesai=="") erro("Erro: Nao especificou imagem de saida");

 double resRho=cmd.getDouble("-r","-rho",1.0);
 double resTheta=cmd.getDouble("-t","-theta",1.0);
 double GaussDev=cmd.getDouble("-d","-desvio",2.0);
 double minGrad=cmd.getDouble("-m","-mingrad",0.05);
 Mat_<FLT> sai=houghGr(ent,resRho,resTheta,GaussDev,minGrad);
 imp(sai,nomesai);
}