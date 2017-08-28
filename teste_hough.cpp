//houghcir.cpp - grad2017
#include <cekeikon.h>
void subt(vector< Mat_<FLT> >& b, int s, int l, int c) {
 if (0<=s && s<int(b.size()) && 0<=l && l<b[0].rows && 0<=c && c<b[0].cols)
 b[s](l,c)-=1.0;
}
void subtraicirculo(vector< Mat_<FLT> >& b, int s, int l, int c, int r) {
 int x=0; int y=r; int limite=chao(r/1.414213562);
 while (x<=limite) {
 subt(b,s,l+x,c+y); subt(b,s,l-x,c+y);
 subt(b,s,l+x,c-y); subt(b,s,l-x,c-y);
 subt(b,s,l+y,c+x); subt(b,s,l-y,c+x);
 subt(b,s,l+y,c-x); subt(b,s,l-y,c-x);
 int mh=abs(elev2(x+1)+elev2(y)-elev2(r));
 int md=abs(elev2(x+1)+elev2(y-1)-elev2(r));
 if (mh<=md) x++;
 else { x++; y--; }
 }
}
int main(int argc, char** argv)
{ 
	if (argc!=6) {
 printf("HoughCir ent.bmp saida rmin rmax colorida.png\n");
 printf(" ent.bmp e' imagem binaria, com backg=255 foreg=0.\n");
 erro("Erro: Numero de argumentos invalido");
 }
 int rmin;
 if (sscanf(argv[3],"%d",&rmin)!=1) erro("Erro: Leitura rmin");
 int rmax;
 if (sscanf(argv[4],"%d",&rmax)!=1) erro("Erro: Leitura rmax");
 Mat_<GRY> ent; le(ent,argv[1]);
 int nl=ent.rows;
 int nc=ent.cols;
 int ns=rmax-rmin+1;
 vector< Mat_<FLT> > sai(ns);
 for (int i=0; i<ns; i++) {
 sai[i].create(nl,nc);
 sai[i].setTo(0.0);
 }
 Mat_<COR> colorida; le(colorida,argv[5]);

 for (int l=0; l<nl; l++)
 	for (int c=0; c<nc; c++)
 		if (ent(l,c)==0) {
 			for (int r=rmin; r<=rmax; r++){
 				subtraicirculo(sai,r-rmin,l,c,r);			
 			}


 		}

//Fazer a média para achar a imagem mais branca e, portanto, assumi-la como de melhor raio
 float media =0;
 int raio=0;
for(int i=0; i<sai.size(); i++){
  float media_i =0;
  for (int l=0; l<nl; l++){
 	for (int c=0; c<nc; c++){
 		media_i += abs(sai[i](l,c));
 	}
 }
 media_i = (media_i/(nl*nc));
 if(media_i> media) {
 	media = media_i;
 	raio = i;
 }
 }
 
int  centro_l=0;
int centro_c =0;
float centro =0;
printf("melhor raio %d\n", raio );

//Procurar pixel mais escuro para definir centro
  for (int l=0; l<nl; l++)
 	for (int c=0; c<nc; c++){
 		if(l!=0&&c!=0&&l!=nl&&c!=nc){ //para não pegar fora da imagem
 			if(abs(sai[raio](l,c)) >centro) { //vai procurando qual o pixel mais escuro e salvando
 											//teria que achar um jeito melhor
 				centro = abs(sai[raio](l,c));
 				centro_l = l;
 				centro_c = c;
 			}
 		}
 	}
 printf("centro: linha %d coluna %d pixel %f\n", centro_l, centro_c, abs(sai[raio](centro_l, centro_c)));

circulo(colorida, centro_l, centro_c, raio, COR(255, 0, 0),3);
ponto(colorida, centro_l, centro_c,COR(0,255,0), 5);
imp(colorida, "colorida.png");
  for (int s=0; s<ns; s++) {
  string st=format("%s%03d.png",argv[2],s+rmin);
  imp(normaliza(sai[s]),st);
  }
}