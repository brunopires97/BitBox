/* BitBox - 
Jogo 2D com OpenGl desenvolvido por Bruno Pires Louren�o.
*/
// baseado em:
// CODIGO-FONTE BASICO PARA ANIMACAO 2D

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <vector>
#include <iostream>

#define TAMJANELA 600
#define TAMUTIL 500
#define POSJANELA 10

using namespace std;

//Declaracao de estruras

struct CompXY{				//estrutura de componentes x e y
	int x;						//componente x
	int y;						//componente y
};

struct Ret{					//estrutura retangulo
	CompXY centro;				//coordenadas do centro do Ret
	CompXY vel;					//componentes da velocidade do Ret
	CompXY esqinf;				//coordenadas do canto esquerdo inferior do Ret
	CompXY dirinf;				//coordenadas do canto direito inferior do Ret
	CompXY dirsup;				//coordenadas do canto esquerdo superior do Ret
	CompXY esqsup;				//coordenadas do canto direito superior do Ret
	int altura;				//coordenadas do centro do Ret
	int largura;				//coordenadas do centro do Ret
	GLfloat cor[4];				//coordenadas do centro do Ret
};

//Declaracao de variaveis globais

int movex=250;//=0;
int movey=250;//=0;

bool parado=true;			//indica se o jogo esta pausado
int level=1;				//nivel 1 no inicio do jogo
long long int highscore=0;

int pontos=0;
char ns[11];
int mostraAviso_count=0;

bool corFundo=false;
bool mostraAviso_flag=true;
bool gameOver_flag=false;
bool inicio_flag=false;


Ret obstaculo;				//retangulo auxiliar para a:
vector <Ret> VObstaculos;	//lista com os obstaculos

Ret alimento;				//retangulo auxiliar para a:
vector <Ret> VAlimentos;	//lista com os alimentos

Ret cursor;					//retangulo do cursor






//===Funcao que desenha um retangulo de acordo com as caracteristicas dele
bool desenhaRet(Ret *retang){
	glColor4f(retang->cor[0], retang->cor[1], retang->cor[2], retang->cor[3]);

	glBegin(GL_POLYGON);
    	glVertex2i(retang->esqinf.x, retang->esqinf.y);
    	glVertex2i(retang->dirinf.x, retang->dirinf.y);
   	 	glVertex2i(retang->dirsup.x, retang->dirsup.y);
    	glVertex2i(retang->esqsup.x, retang->esqsup.y);
    	glVertex2i(retang->esqinf.x, retang->esqinf.y);
	glEnd();

	return true;
}

//===Funcao que calcula as coordenadas dos cantos dos retangulos
bool calculaCantos(Ret *retang){

	retang->esqinf.x=retang->centro.x-retang->largura/2;
	retang->esqinf.y=retang->centro.y-retang->altura/2;

	retang->dirinf.x=retang->centro.x+retang->largura/2;
	retang->dirinf.y=retang->centro.y-retang->altura/2;

	retang->dirsup.x=retang->centro.x+retang->largura/2;
	retang->dirsup.y=retang->centro.y+retang->altura/2;

	retang->esqsup.x=retang->centro.x-retang->largura/2;
	retang->esqsup.y=retang->centro.y+retang->altura/2;
}


//===Funcao que define os parametros iniciais
void Inicializa(void) {
  glClearColor(0, 0, 0, 0);
  glColor3f(1,0,0);

inicio_flag=true;

//parametros do retangulo do cursor

cursor.largura=20;
cursor.altura=20;

cursor.cor[0]=1;
cursor.cor[1]=1;
cursor.cor[2]=1;
cursor.cor[3]=0.5;

//preenchimento do vector de obstaculos, de acordo com o nivel
for(int j=0; j<level; j++){
obstaculo.centro.x=rand()%500+30;
obstaculo.centro.y=rand()%500+30;
obstaculo.vel.x=rand()%(level+2)-level/2;
obstaculo.vel.y=rand()%(level+2)-level/2;
obstaculo.largura=20;
obstaculo.altura=20;
obstaculo.cor[0]=0;
obstaculo.cor[1]=0;
obstaculo.cor[2]=1;
obstaculo.cor[3]=0;
VObstaculos.push_back(obstaculo);}

//preenchimento do vector de alimentos, de acordo com o nivel
for(int i=0; i<level; i++){
alimento.centro.x=rand()%500+30;
alimento.centro.y=rand()%500+30;
alimento.vel.x=rand()%(level+2)-level/2;
alimento.vel.y=rand()%(level+2)-level/2;
alimento.largura=20;
alimento.altura=20;
alimento.cor[0]=0;
alimento.cor[1]=1;
alimento.cor[2]=0;
alimento.cor[3]=0;
VAlimentos.push_back(alimento);}

}

//===Funcao que eh executada quando o perde
void gameOver(){

cout<<"loser"<<endl;
level=1;
VObstaculos.clear();
VAlimentos.clear();
parado=true;
gameOver_flag=true;
Inicializa();
pontos=0;

}


//===Funcao que eh executada quando o Ret do cursor encosta em um obstaculo
void colisaoCursor(){

cursor.cor[0]=1;
cursor.cor[1]=0.1;
cursor.cor[2]=0.1;
cursor.largura+=20;
cursor.altura+=20;
if(cursor.largura>400)
gameOver();
if(pontos>10)
pontos-=10;
else if(pontos>0)
pontos=0;
}

//===Funcao que eh executada quando todos os alimentos sao comidos
bool levelUP(){
level++;
VObstaculos.clear();
VAlimentos.clear();
pontos+=10;
Inicializa();
parado=true;


}

//===Funcao que eh executada quando o Ret do cursor encosta em um alimento
void comeu(){
cursor.cor[0]=0.2;
cursor.cor[1]=0.7;
cursor.cor[2]=0.2;
pontos+=1;

if(cursor.largura>=20){
cursor.largura-=5;
cursor.altura-=5;
}

}


//===Funcao que faz, a partir do nivel 20, a tela piscar
void TrocaFundo(int passo){
if(level>20){
if(corFundo) {
	corFundo=false;
glClearColor(0, 0, 0, 0);
}
else{
	corFundo=true;		
glClearColor(1, 1, 1, 0);
}

}
if(level>19)
glutTimerFunc(30000/level,TrocaFundo,1);
} 



//===Funcao que move os centro dos obstaculos e dos alimentos de acordo com as componentes de velocidade de cada um dos retangulos
void TrocaQuadro(int passo) {

	if(!parado){				//não atualiza posicao se o jogo esta pausado
	
	for(int i=0; i<VObstaculos.size();  i++){
	//inverte sentido do movimento ao encostar nos limites da janela
	if(VObstaculos[i].dirsup.x>=(TAMJANELA)||VObstaculos[i].esqinf.x<=0)
		VObstaculos[i].vel.x=-VObstaculos[i].vel.x;

	if(VObstaculos[i].dirsup.y>=(TAMJANELA-30)||VObstaculos[i].esqinf.y<=0)
		VObstaculos[i].vel.y=-VObstaculos[i].vel.y;

	//incrementa a posição do centro de uma vez a velocidade
		VObstaculos[i].centro.x+=VObstaculos[i].vel.x;
		VObstaculos[i].centro.y+=VObstaculos[i].vel.y;
	}

	//repete o mesmo procedimento para o vector de alimentos
	for(int j=0; j<VAlimentos.size();  j++){
	if(VAlimentos[j].dirsup.x>=(TAMJANELA)||VAlimentos[j].esqinf.x<=0)
		VAlimentos[j].vel.x=-VAlimentos[j].vel.x;

	if(VAlimentos[j].dirsup.y>=(TAMJANELA-30)||VAlimentos[j].esqinf.y<=0)
		VAlimentos[j].vel.y=-VAlimentos[j].vel.y;

		VAlimentos[j].centro.x+=VAlimentos[j].vel.x;
		VAlimentos[j].centro.y+=VAlimentos[j].vel.y;
	}

	}
	glutPostRedisplay();
	glutTimerFunc(100,TrocaQuadro,1);

}




//===Funcao que escreve um texto em uma posicao na tela
void escreveTexto(int posx, int posy, char ns[40], GLfloat Red, GLfloat Green, GLfloat Blue, GLfloat Alpha, int type) {
	glColor4f(Red,Green,Blue,Alpha);
	glRasterPos2i (posx, posy);
	char *p= (char*) ns;
	while (*p!= '\0')
		switch(type){
			case 1:

		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
		break;
			case 2:
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *p++);
		break;
			case 3:
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p++);
		break;

    /* Estilos de texto disponíveis:
     * GLUT_BITMAP_8_BY_13
     * GLUT_BITMAP_9_BY_15
     * GLUT_BITMAP_TIMES_ROMAN_10
     * GLUT_BITMAP_TIMES_ROMAN_24
     * GLUT_BITMAP_HELVETICA_10
     * GLUT_BITMAP_HELVETICA_12
     * GLUT_BITMAP_HELVETICA_18
    */
    
}
}


//===Funcao que desenha os objetos que se movem sozinhos
void DesenhaObjetoMovel(void) {

//desenha todos os elementos do vector de obstaculos
for(int i=0; i<VObstaculos.size();  i++){
		calculaCantos(&VObstaculos[i]);
		desenhaRet(&VObstaculos[i]);
}

//desenha todos os elementos do vector de alimentos
for(int j=0; j<VAlimentos.size();  j++){
		calculaCantos(&VAlimentos[j]);
		desenhaRet(&VAlimentos[j]);
}
	
}
 

//===Funcao que desenha o retangulo do cursor e testa a colisao com os obstaculos e os alimentos
void DesenhaObjetoControlado(int coordX, int coordY) {

//Desenho do Ret do cursor
glColor3f(0,1,0);
cursor.centro.x=coordX;
cursor.centro.y=coordY;
calculaCantos(&cursor);
desenhaRet(&cursor);


if(!parado){
//Testa colisao com obstaculos
for(int i=0; i<VObstaculos.size();  i++){

if(VObstaculos[i].esqinf.x>=cursor.esqinf.x && VObstaculos[i].esqinf.x<=cursor.dirsup.x){
	if(VObstaculos[i].esqinf.y>=cursor.esqinf.y && VObstaculos[i].esqinf.y<=cursor.dirsup.y){
		VObstaculos.erase(VObstaculos.begin()+i);
		colisaoCursor();
		goto TAlim;
	}
}

if(VObstaculos[i].dirinf.x>=cursor.esqinf.x && VObstaculos[i].dirinf.x<=cursor.dirsup.x){
	if(VObstaculos[i].dirinf.y>=cursor.esqinf.y && VObstaculos[i].esqinf.y<=cursor.dirsup.y){
		VObstaculos.erase(VObstaculos.begin()+i);
		colisaoCursor();
		goto TAlim;
	}
}

if(VObstaculos[i].dirsup.x>=cursor.esqinf.x && VObstaculos[i].dirsup.x<=cursor.dirsup.x){
	if(VObstaculos[i].dirsup.y>=cursor.esqinf.y && VObstaculos[i].dirsup.y<=cursor.dirsup.y){
		VObstaculos.erase(VObstaculos.begin()+i);
		colisaoCursor();
		goto TAlim;
	}
}

if(VObstaculos[i].esqsup.x>=cursor.esqinf.x && VObstaculos[i].esqsup.x<=cursor.dirsup.x){
	if(VObstaculos[i].esqsup.y>=cursor.esqinf.y && VObstaculos[i].esqsup.y<=cursor.dirsup.y){
		VObstaculos.erase(VObstaculos.begin()+i);	
		colisaoCursor();
		goto TAlim;
	}
}

}

TAlim:
//Testa colisao com alimentos

for(int i=0; i<VAlimentos.size();  i++){
if(VAlimentos[i].esqinf.x>=cursor.esqinf.x && VAlimentos[i].esqinf.x<=cursor.dirsup.x){
	if(VAlimentos[i].esqinf.y>=cursor.esqinf.y && VAlimentos[i].esqinf.y<=cursor.dirsup.y){
		VAlimentos.erase(VAlimentos.begin()+i);		
		comeu();
		goto TLevelUp;
	}
}

if(VAlimentos[i].dirinf.x>=cursor.esqinf.x && VAlimentos[i].dirinf.x<=cursor.dirsup.x){
	if(VAlimentos[i].dirinf.y>=cursor.esqinf.y && VAlimentos[i].esqinf.y<=cursor.dirsup.y){
		VAlimentos.erase(VAlimentos.begin()+i);		
		comeu();
		goto TLevelUp;
	}
}

if(VAlimentos[i].dirsup.x>=cursor.esqinf.x && VAlimentos[i].dirsup.x<=cursor.dirsup.x){
	if(VAlimentos[i].dirsup.y>=cursor.esqinf.y && VAlimentos[i].dirsup.y<=cursor.dirsup.y){
		VAlimentos.erase(VAlimentos.begin()+i);		
		comeu();
		goto TLevelUp;
	}
}

if(VAlimentos[i].esqsup.x>=cursor.esqinf.x && VAlimentos[i].esqsup.x<=cursor.dirsup.x){
	if(VAlimentos[i].esqsup.y>=cursor.esqinf.y && VAlimentos[i].esqsup.y<=cursor.dirsup.y){
		VAlimentos.erase(VAlimentos.begin()+i);
		comeu();
		goto TLevelUp;
	}
}

}

if(pontos>highscore)
		highscore=pontos;
}

TLevelUp:
//testa se todos os alimentos foram comidos
if(VAlimentos.size()==0){
levelUP();
}


}


//===Funcao que elenca os textos que serao escritos na tela
void desenhaTexto(){

	char LEVEL[11];	
  	sprintf (LEVEL, "%d", level); 
	escreveTexto(125, TAMJANELA-22, (char*)"LEVEL:",1,0,0.7,0.2,3);     
  	escreveTexto(182, TAMJANELA-22, LEVEL,1,0.7,0,0.2,3);

	char RECORDE[21];
	sprintf (RECORDE, "%llu", highscore);	
	escreveTexto(250, TAMJANELA-22, (char*)"HIGH SCORE:",1,0,0.7,0.2,3);     
  	escreveTexto(350, TAMJANELA-22, RECORDE,1,0.7,0,0.2,3);

	char PONTOS[21];
  	sprintf (PONTOS, "%d", pontos);   
	escreveTexto(430, TAMJANELA-22, (char*)"YOUR SCORE:",1,0,0.7,0.2,3);
	escreveTexto(530, TAMJANELA-22, PONTOS,1,0.7,0,0.8,3);


	if(!corFundo)
	escreveTexto(29, TAMJANELA-22, (char*)"BitBox",1,1,1,0.3,3);
	else
	escreveTexto(29, TAMJANELA-22, (char*)"BitBox",0,0,0,0.3,3);

	if(gameOver_flag)
	{
	escreveTexto(TAMJANELA/2-70, TAMJANELA/2+9, (char*)"GAME OVER",1,0,0,0.9,3);
	escreveTexto(TAMJANELA/2-130, TAMJANELA/2-9, (char*)"Clique para continuar . . .",0.8,0,0,0.9,3);
	}

}

//===Funcao que controla quando o aviso de instrucao de inicio do jogo deve ser mostrado
void MostraAviso(){

  	if(mostraAviso_count==2)
	mostraAviso_flag=true;

	if(mostraAviso_count==15){
	mostraAviso_flag=false;
	mostraAviso_count=0;}

	mostraAviso_count++;

  	if(parado){
			if(mostraAviso_flag){
			if(!corFundo)
			escreveTexto(TAMJANELA/2-90, TAMJANELA/2, (char*)"Clique para iniciar...",1,1,0,0.9,3);
			else
			escreveTexto(TAMJANELA/2-90, TAMJANELA/2, (char*)"Clique para iniciar...",1,0,0,0.9,3);
			
			}
			
}}

//=================
void Desenha(void){
  	
  glClear(GL_COLOR_BUFFER_BIT);


  if(!gameOver_flag){
	if(!parado)
		glutSetCursor(GLUT_CURSOR_NONE);	//se o jogo esta "rodando" o cursor do mouse fica oculto
	else
		glutSetCursor(GLUT_CURSOR_WAIT);	//se o jogo esta parado o cursor do mouse eh um relogio

  DesenhaObjetoControlado(movex, movey);

  glPushMatrix();
  DesenhaObjetoMovel();
  glPopMatrix();

  glPushMatrix();
  MostraAviso();
  glPopMatrix();

  }
	
  desenhaTexto();
  glutSwapBuffers();

 }


//=========================================================
void Mouse(int botao, int estado, int xmouse, int ymouse) {

movex=xmouse;
movey=TAMJANELA-ymouse;



  if (botao == GLUT_LEFT_BUTTON)
   	 if (estado == GLUT_DOWN){
				if(gameOver_flag){
					gameOver_flag=false;
					pontos=0;
				}
				else{
    			if(parado)
					parado=false;
					inicio_flag=true;
	}
	}

  if (botao == GLUT_RIGHT_BUTTON)
    if (estado == GLUT_DOWN){
    
	}
      
  if (botao == GLUT_MIDDLE_BUTTON)
    if (estado == GLUT_DOWN)
     

  glutPostRedisplay();
  
}


//==============================================
void MoveMouse(int xmouse, int ymouse){


movex=xmouse;
movey=TAMJANELA-ymouse;
glutPostRedisplay();

}

//===============================================
void Teclado(int tecla, int xmouse, int ymouse) {

  switch (tecla) {
  
    case GLUT_KEY_LEFT:   if(movex>-250)
                            movex-=5;
                          break;

    case GLUT_KEY_RIGHT:  if(movex<300)
    											  movex+=5;
                          break;

    case GLUT_KEY_UP:     if(movey<300)
                            movey+=5;
                          break;

    case GLUT_KEY_DOWN:   if(movey>-250)
    											  movey-=5;
                          break;
                          
    case GLUT_KEY_HOME:   level+=10;
                          break;
                          
    case GLUT_KEY_PAGE_UP: if(movey<300)
                            movey+=5;
                           if(movex<300)
    											  movex+=5;
                           break;

		case GLUT_KEY_END: if(movex>-250)
                         movex-=5;
    									 if(movey>-250)
    										 movey-=5;
                       break;
                       
		case GLUT_KEY_PAGE_DOWN: if(movex<300)
    											     movex+=5;                       
                             if(movey>-250)
    										       movey-=5;
    										     break;
    										     
  }
  
  glutPostRedisplay();
  
}

//==========================================================
void Teclado2(unsigned char tecla, int xmouse, int ymouse) {

  switch (tecla) {
  
  	// Tecla [Esc]
  	case 27: exit(0);
             break;
                          
   }
   
  glutPostRedisplay();
  
}                       
  
//==============================================
void AlteraTamanhoJanela(GLsizei w, GLsizei h) { 
  if(h == 0)
    h = 1;
  
  glViewport(0, 0, w, h);
     
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
     
  if (w <= h)
    gluOrtho2D(0, TAMJANELA, 0, TAMJANELA*h/w);
  else
    gluOrtho2D(0, TAMJANELA*w/h, 0, TAMJANELA);
} 



//===============================
int main(int argc, char** argv) {
  glutInit(&argc, argv); 

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(TAMJANELA, TAMJANELA);
  glutInitWindowPosition(10,10);
  glutCreateWindow("BitBox");
  glutDisplayFunc(Desenha);
  glutReshapeFunc(AlteraTamanhoJanela);
  glutTimerFunc(500,TrocaQuadro,1);
  glutTimerFunc(500,TrocaFundo,1);
  setbuf(stdin,NULL);
  glutSpecialFunc(Teclado);
  glutKeyboardFunc(Teclado2);
  glutPassiveMotionFunc(MoveMouse);
  glutMouseFunc(Mouse);

	glutKeyboardFunc(Teclado2);
	glutMouseFunc(Mouse);

  Inicializa();

  glutMainLoop();
  return 0;
}
