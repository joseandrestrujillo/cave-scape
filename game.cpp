#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;
#define TAMANO 1000
#define SPEED 100

//----------------------------------------------------------------------------------------------------------------------------
//ESTRUCTURA DE DATOS
struct Datos{
	//PARAMETROS
		//Coordenadas centro personaje
		int x=150,y=849;
		int vx=0, vy=0, ax=0, ay=0;
		//Coordenadas sprite personaje
		int bx=x-50,by=y+50;
		//Coordenadas personaje en matriz de colisiones
		int cx=(x/100), cy=((y)/100);
		//Parametros del contador de la plataforma
		int contador=0;
		int pasoContador=1;
		//Indica si el jugador está sobre la plataforma
		bool plataforma=false;
		//Indica si el jugador esta en una casilla de escalera
		bool escaleras=false;
		//Matriz con las entidades que hay en cada casilla
		int matrizCollides[10][10]=
			 {{1,1,1,1,1,1,1,1,1,1},
				{1,0,0,0,0,0,0,0,0,1},
				{1,0,0,3,1,0,0,0,0,7},
				{1,3,1,1,1,1,1,1,1,1},
				{1,3,0,0,0,0,0,0,0,1},
				{1,1,1,1,1,1,1,0,0,1},
				{1,0,0,0,0,0,0,0,2,1},
				{1,0,0,0,0,0,0,0,1,1},
				{1,0,0,2,0,0,0,1,1,1},
				{1,1,1,1,1,1,1,1,1,1}};
		//Indica si el jugador toca alguna pared en alguna dirección
		bool topCollider=false, bottomCollider=true, leftCollider=true, rightCollider=false, jumpCollider=false;
		//Controladores de las pantallas
		bool gameover=false;
		//Controla en que dirección se está moviendo el personaje
		int direction=1;
		//Para hacer que los movimientos del personaje cuadren con la cuadricula, debemos hacer que los movimientos en el mismo eje no se solapen
		//Para ello utilizaremos las siguientes variables, que nos indicarán cuantas iteraciones han de pasar para dar el siguiente paso
		int waitStepsX=0;
		int waitStepsY=0;
		//PARAMETROS PANTALLA VICTORIA
		bool win=false;
		int colorWin=0;
		//PARAMETROS PANTALLA INICIO
		bool start=true;
		int selectOption=-1;
		int iteracionFlecha=0;
		//PARAMETROS PANTALLA INICIO
		bool escape=false;

		bool quit=false;
	//FUNCIONES
	void actualizarCoordenadas(){
		bx=x-50;
		by=y+50;
		cx=(x/100); 
		cy=((y)/100);
	}
	//El contador irá desde 0 hasta 20, desde 20 hasta 0 y así sucesivamente.
	//Se elige el numero 20 para que la plataforma suba de manera fluida, dando 20 pasos de 10px 
	void actualizarContador(){
		contador+=pasoContador;
		//Si se llega a uno de los extremos se invierte el sentido del paso
		if((contador==20)||(contador==0)){pasoContador*=-1;}
		//Si se llega al extremo superior y el jugador está sobre la platafoma, el movimiento de la plataforma y el avance del contador se detendran
		if((contador==20)&&(plataforma)){pasoContador=0;}
		//Si el jugador ya no esta en la plataforma, el contador vuelve a avanzar en sentido descendente
		if((contador==20)&&(!plataforma)&&(pasoContador==0)){pasoContador=-1;}
	}
	//Resetea todos los parametros, haciendo reiniciar el juego desde el principio
	void reset(){
		x=150;
		y=849;
		vx=0;
		vy=0;
		ax=0;
		ay=0;
		bx=x-50,by=y+50;
		cx=(x/100);
		cy=((y)/100);
		contador=0;
		pasoContador=1;
		plataforma=false;
		escaleras=false;
		for(int i=5;i<8;i++){matrizCollides[i][7]=0;}
		topCollider=false; 
		bottomCollider=true;
		leftCollider=true;
		rightCollider=false;
		jumpCollider=false;
		gameover=false;
		direction=1;
		waitStepsX=0;
		waitStepsY=0;
		win=false;
		colorWin=0;
		start=false;
		selectOption=-1;
		escape=false;
	}
};
//FIN ESTRUCTURA DE DATOS
//----------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------
//GRAFICOS
//Función para hacer un cuadrado que ocupe desde y=a hasta y=b y desde x=c hasta x=d de un color especifico
//Esta función se usará durante todo el programa para crear los sprites y graficos
void rellenar(char m[TAMANO][TAMANO], int a, int b, int c, int d, char color);
//SPRITES: Todos los sprites tienen unas coordenadas (a,b) que corresponden a las coordenadas (y,x) de su esquina inferior izquierda
void letraP(char m[TAMANO][TAMANO],int a, int b, char color);
void letraE(char m[TAMANO][TAMANO],int a, int b, char color);
void letraR(char m[TAMANO][TAMANO],int a, int b, char color);
void letraD(char m[TAMANO][TAMANO],int a, int b, char color);
void letraI(char m[TAMANO][TAMANO],int a, int b, char color);
void letraS(char m[TAMANO][TAMANO],int a, int b, char color);
void letraT(char m[TAMANO][TAMANO],int a, int b, char color);
void letraF(char m[TAMANO][TAMANO],int a, int b, char color);
void letraN(char m[TAMANO][TAMANO],int a, int b, char color);
void letraU(char m[TAMANO][TAMANO],int a, int b, char color);
void letraL(char m[TAMANO][TAMANO],int a, int b, char color);
void letraH(char m[TAMANO][TAMANO],int a, int b, char color);
void letraO(char m[TAMANO][TAMANO],int a, int b, char color);
void letraA(char m[TAMANO][TAMANO],int a, int b, char color);
void letraC(char m[TAMANO][TAMANO],int a, int b, char color);
void letraV(char m[TAMANO][TAMANO],int a, int b, char color);
void bloque(char m[TAMANO][TAMANO],int a, int b);
void bloqueSombra(char m[TAMANO][TAMANO],int a, int b);
void pincho(char m[TAMANO][TAMANO],int a, int b);
void pinchoOscuro(char m[TAMANO][TAMANO],int a, int b);
void escalera(char m[TAMANO][TAMANO],int a, int b);
void plataforma(char m[TAMANO][TAMANO], int a, int b);
void character(char m[TAMANO][TAMANO], int a, int b);

//PANTALLAS
void start(char m[TAMANO][TAMANO], Datos &datos);
void escape(char m[TAMANO][TAMANO], Datos &datos);
void gameover(char m[TAMANO][TAMANO], int k , Datos &datos);
void win(char m[TAMANO][TAMANO], int k ,Datos &datos);

//FIN GRAFICOS
//----------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------
//ACTUALIZACIONES
void actualizarPlataforma(char m[TAMANO][TAMANO], Datos &datos){
	//Actualiza el contador y lo convierte a numero de pasos de 10px (multiplicandolo por 10)
	datos.actualizarContador();
	int diferenciaY=datos.contador*10;
	//Dibuja la plataforma, añadiendole los pasos (diferenciaY) que haya dado
	plataforma(m, 740-diferenciaY,700);
	//Detecta si el jugador está sobre la casilla en la que se encuentra la plataforma
	if(datos.matrizCollides[datos.cy][datos.cx]==4){
		datos.x=749;
		datos.y=700-diferenciaY-50;
		datos.actualizarCoordenadas();
		datos.plataforma=true;
	}	else{datos.plataforma=false;}
	//ACTUALIZA LA MATRIZ DE COLISIONES CON LA NUEVA POSICIÓN DE LA PLATAFORMA
	for(int i=5;i<8;i++){datos.matrizCollides[i][7]=0;}
	datos.matrizCollides[(700-diferenciaY-50)/100][7]=4;
}

void actualizarPinchoOscuro(char m[TAMANO][TAMANO], Datos &datos){
	if(datos.contador<10){
		datos.matrizCollides[1][4]=5;
		datos.matrizCollides[2][6]=0;
	}else{
		datos.matrizCollides[1][4]=0;
		datos.matrizCollides[2][6]=5;
	}
}
void actualizarCharacter(char m[TAMANO][TAMANO], Datos &datos){character(m, datos.by,datos.bx);}

void actualizarFondo(char m[TAMANO][TAMANO], Datos &datos){
	rellenar(m,0,TAMANO-1,0,TAMANO-1,0);
	for(int i=0;i<1000;i+=100){
		for(int j=0;j<1000;j+=100){
			switch(datos.matrizCollides[(i/100)][j/100]){
				case 1:{
					if((datos.matrizCollides[(i/100)][((j-100)/100)])==0){bloqueSombra(m,i+99,j);}
					else if(datos.matrizCollides[(i/100)][(j-100)/100]==2){
						bloqueSombra(m,i+99,j);
						pincho(m,i+99,j-100);
					}else{bloque(m,i+99,j);}
				}break;
				case 2:{pincho(m,i+99,j);}break;
				case 3:{escalera(m,i+99,j);}break;
				case 5:{pinchoOscuro(m,i+99,j);}break;
			}
		}
	}
}
//FIN ACTUALIZACIONES
//----------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------
//COMPROBACIONES DEL ENTORNO DEL PERSONAJE
//Comprueba el estado de la posición del jugador, tanto las colisiones con los muros, como si esta en una casilla de escalera o sobre la plataforma
void comprobarEstado(char m[TAMANO][TAMANO], Datos &datos){
	//IZQUIERDA
	datos.leftCollider=false;
	if(datos.plataforma){
		//En caso de estar sobre la plataforma, al no ser estatica, debemos asegurarnos de que siga detectando el muro de la izquierda aunque el jugador no esté completamente centrado verticalmente con dicho muro
		//Para ello, en lugar de comprobar solo la casilla que le corresponde al centro del jugador, comprobaremos la casilla que le corresponde a cada pixel de la vertical del jugador
		for(int i=-50;i<50;i++){
			if(datos.matrizCollides[(datos.y+i)/100][(datos.x-100)/100]==1){datos.leftCollider=true;}
		}
	}else if(datos.matrizCollides[datos.cy][datos.cx-1]==1){datos.leftCollider=true;}
	//ARRIBA
	datos.topCollider=false;
		if(datos.matrizCollides[datos.cy-1][datos.cx]==1){datos.topCollider=true;}
	//SALTO
	datos.jumpCollider=false;
		if(datos.matrizCollides[datos.cy-2][datos.cx]==1){datos.jumpCollider=true;}
	//DERECHA
	datos.rightCollider=false;
		if(datos.matrizCollides[datos.cy][datos.cx+1]==1){datos.rightCollider=true;}
	//ABAJO
	datos.bottomCollider=false;
	//Si en la casilla de abajo hay un bloque (==1), una escalera (==3) o el jugador está sobre la plataforma 
	if((datos.matrizCollides[datos.cy+1][datos.cx]==1)||(datos.matrizCollides[datos.cy+1][datos.cx]==3)||(datos.plataforma)){
		datos.bottomCollider=true;
		datos.waitStepsY=0;
	}
	//GAMEOVER
	if((datos.matrizCollides[datos.cy][datos.cx]==2)||(datos.matrizCollides[datos.cy][datos.cx]==5)){datos.gameover=true;}
	//WIN
	if(datos.matrizCollides[datos.cy][datos.cx]==7){datos.win=true;}
	//ESCALERAS
	datos.escaleras=false;
	if(datos.matrizCollides[datos.cy][datos.cx]==3){datos.escaleras=true;}
}


//FIN COMPROBACIONES DEL ENTORNO DEL PERSONAJE
//----------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------
//FISICAS DEL PERSONAJE
//Comprueba si la aceleración en el eje vertical de la gravedad debe estar activa o no
void gravedad(char m[TAMANO][TAMANO], Datos &datos){
	//Si no hay colisión debajo del personaje, la gravedad actuará, en caso contrario, será cero
	if((!datos.bottomCollider)&&(!datos.plataforma)){datos.ay=100;}
	else{
		datos.ay=0;
		datos.vy=0;
	}
}
//Comprueba la fricción en el eje x
void friccion(char m[TAMANO][TAMANO], Datos &datos){
	//Si la velocidad es negativa tendrá una fricción positiva
	if(datos.vx<0){datos.ax=50;}
	//Si la velocidad es positiva tendrá una fricción negativa
	else if(datos.vx>0){datos.ax=-50;}
	//Si la velocidad es cero no tendrá fricción
	else{datos.ax=0;}
}
void fisicas(char m[TAMANO][TAMANO], Datos &datos){
	//Primero comprueba si el personaje está cayendo (vy>0) y hay un bloque abajo, en ese caso, pone la velocidad a cero
	if(((datos.bottomCollider)||(datos.plataforma))&&(datos.vy>0)){datos.vy=0;}
	//Actualiza la posición y la velocidad según las leyes de la cinematica
	datos.x=datos.x+datos.vx+((datos.ax)/2);
	datos.y=datos.y+datos.vy+((datos.ay)/2);
	datos.actualizarCoordenadas();
	datos.vx=datos.vx+datos.ax;
	datos.vy=datos.vy+datos.ay;
}
//FIN FISICAS DEL PERSONAJE
//----------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------
//REAJUSTE POSICIÓN PERSONAJE

//Centrar jugador en casilla

void centrarX(Datos &datos){
		if(datos.x!=((datos.cx*100)+50)){
			datos.x=(datos.cx*100)-50;
			datos.actualizarCoordenadas();
		}
}
void centrarY(Datos &datos){
		if(datos.y!=((datos.cy*100)+49)){
			datos.y=(datos.cy*100)+49;
			datos.actualizarCoordenadas();
		}
}
void reajusteCharacter(char m[TAMANO][TAMANO], Datos &datos){
	//Ajusta la posición del jugador si se encuentra en una casilla de bloque, posicionandolo en la casilla anterior, segun la dirección en la que se estuviera moviendo
	if(datos.matrizCollides[datos.cy][datos.cx]==1){
		switch(datos.direction){
			case 1:{//ABAJO
				datos.cy=datos.cy-1;
				centrarY(datos);
			}break;
			case 2:{//ARRIBA
				datos.cy=datos.cy+1;
				centrarY(datos);
			}break;
			case 3:{//IZQUIERDA
					datos.cx=datos.cx+1;
					centrarX(datos);
			}break;
			case 4:{//DERECHA
				datos.cx=datos.cx-1;
				centrarX(datos);
			}break;
		}
	}
	//A veces al caer, el personaje no se queda centrado en la casilla, para solucionar esto, lo centraremos cuando termine su movimiento
	if((datos.waitStepsY==0)&&(datos.waitStepsX==0)&&(!datos.plataforma)&&(datos.bottomCollider)){
		centrarY(datos);
	}

}
//FIN REAJUSTE POSICIÓN PERSONAJE
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//CONTROL DEL PERSONAJE
void characterControl(char m[TAMANO][TAMANO], int k , Datos &datos){
		if((k==81)&&(!datos.leftCollider)&&(datos.waitStepsX==0)){
			datos.vx=-100;
			datos.direction=3;
			datos.waitStepsX=2;
		}else if((datos.bottomCollider)&&(k==32)&&(!datos.jumpCollider)&&(!datos.plataforma)&&(!datos.escaleras)){
			datos.vy=-200;
			datos.ay=100;
			datos.direction=2;
			datos.waitStepsY=4;
		}else if((k==83)&&(!datos.rightCollider)&&(datos.waitStepsX==0)){
			datos.vx=100;
			datos.direction=4;
			datos.waitStepsX=2;
		}else if((datos.escaleras)&&(k==82)&&(!datos.topCollider)){
			datos.y-=100;
			datos.direction=2;
		}else if(k==27){datos.escape=true;}
		else{datos.direction=1;}
}
//FIN CONTROL DEL PERSONAJE
//----------------------------------------------------------------------------------------------------------------------------
void process(char screen[TAMANO][TAMANO], int k , Datos &datos){
	if(!datos.start){
		if((!datos.gameover)&&(!datos.win)&&(!datos.escape)){
			if(datos.waitStepsX>0){datos.waitStepsX--;}
			if(datos.waitStepsY>0){datos.waitStepsY--;}
			actualizarFondo(screen,datos);
			actualizarPinchoOscuro(screen,datos);
			actualizarPlataforma(screen,datos);
			comprobarEstado(screen,datos);
			if((datos.gameover)||(datos.win)){return;}
			gravedad(screen,datos);
			characterControl(screen,k,datos);
			friccion(screen,datos);
			fisicas(screen,datos);
			reajusteCharacter(screen,datos);
			actualizarCharacter(screen,datos);
		}else if(datos.win){win(screen,k,datos);}
		else if(datos.gameover){gameover(screen, k,datos);}
		else if(datos.escape){
			if((k==81)||(k==83)){
				datos.selectOption*=-1;
				datos.iteracionFlecha=0;
			}
			escape(screen,datos);
			if((datos.selectOption==-1)&&(k==13)){datos.escape=false;}
			if((datos.selectOption==1)&&(k==13)){datos.quit=true;}
		}
	}else {
		if((k==81)||(k==83)){
			datos.selectOption*=-1;
			datos.iteracionFlecha=0;
		}
		start(screen,datos);
		if((datos.selectOption==-1)&&(k==13)){datos.start=false;}
		if((datos.selectOption==1)&&(k==13)){datos.quit=true;}
	}
}
//no tocar a partir de aqui

void processAndShow(cv::Mat im, char k, Datos &datos){
    char screen[TAMANO][TAMANO];
    for(int i=0;i<TAMANO;i++){
        auto ptr=im.ptr<uchar>(i);
        for(int j=0;j<TAMANO;j++)
            screen[i][j]=ptr[j];
    }
    process(screen,k,datos);
    for(int i=0;i<TAMANO;i++){
        auto ptr=im.ptr<uchar>(i);
        for(int j=0;j<TAMANO;j++)
            ptr[j]=screen[i][j];
    }
    cv::Mat im2;
    cv::resize(im,im2,{500,500});
    cv::imshow("game",im2);
}
int main(int argc,char **argv){
    cv::Mat im=cv::Mat::zeros(TAMANO,TAMANO,CV_8UC1);
    Datos datos;
    processAndShow(im,-1,datos);
    int k;
    do{
        k=cv::waitKey(SPEED);
        processAndShow(im,k,datos);
    }while(!datos.quit);
}


//----------------------------------------------------------------------------------------------------------------------------
//GRAFICOS
//Función para hacer un cuadrado que ocupe desde y=a hasta y=b y desde x=c hasta x=d de un color especifico
//Esta función se usará durante todo el programa para crear los sprites y graficos
void rellenar(char m[TAMANO][TAMANO], int a, int b, int c, int d, char color){
	for(int i=a;i<=b;i++){
		for(int j=c;j<=d;j++){m[i][j]=color;}
	}
}

//SPRITES: Todos los sprites tienen unas coordenadas (a,b) que corresponden a las coordenadas (y,x) de su esquina inferior izquierda
//Letras que se utilizarán en las pantallas del juego
void letraP(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a,b,b+10,color);
	rellenar(m, a-59, a-49,b,b+29,color);	
	rellenar(m, a-29, a-19,b,b+29,color);
	rellenar(m, a-49, a-29,b+29,b+39,color);	
}
void letraE(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a,b,b+10,color);
	rellenar(m, a-59, a-49,b,b+29,color);
	rellenar(m, a-9, a,b,b+29,color);
	rellenar(m, a-29, a-19,b,b+19,color);
}
void letraR(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a,b,b+10,color);
	rellenar(m, a-59, a-49,b,b+29,color);	
	rellenar(m, a-29, a-19,b,b+29,color);
	rellenar(m, a-49, a-29,b+29,b+39,color);	
	rellenar(m, a-19, a-10,b+20,b+29,color);	
	rellenar(m, a-9, a,b+30,b+39,color);
}
void letraD(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a,b,b+10,color);
	rellenar(m, a-59, a-49,b,b+29,color);
	rellenar(m, a-9, a,b,b+29,color);
	rellenar(m, a-49, a-9,b+29,b+39,color);	
}
void letraI(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a,b,b+10,color);
}
void letraS(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-19, a-10,b,b+9,color);
	rellenar(m, a-19, a-10,b+30,b+39,color);
	rellenar(m, a-49, a-40,b,b+9,color);
	rellenar(m, a-49, a-40,b+30,b+39,color);
	rellenar(m, a-39, a-30,b+10,b+19,color);
	rellenar(m, a-29, a-20,b+20,b+29,color);
	rellenar(m, a-9, a,b+10,b+29,color);
	rellenar(m, a-59, a-50,b+10,b+29,color);
}
void letraT(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a-50,b,b+29,color);
	rellenar(m, a-59, a,b+10,b+19,color);
}
void letraF(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a,b,b+10,color);
	rellenar(m, a-59, a-49,b,b+29,color);
	rellenar(m, a-29, a-19,b,b+19,color);
}
void letraN(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a,b,b+10,color);
	rellenar(m, a-59, a,b+50,b+59,color);
	rellenar(m, a-49, a-40,b+10,b+19,color);
	rellenar(m, a-39, a-30,b+20,b+29,color);
	rellenar(m, a-29, a-20,b+30,b+39,color);
	rellenar(m, a-19, a-10,b+40,b+49,color);
}
void letraU(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a-10,b,b+10,color);
	rellenar(m, a-59, a-10,b+30,b+39,color);
	rellenar(m, a-9, a,b+10,b+29,color);
}
void letraL(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a,b,b+10,color);
	rellenar(m, a-9, a,b+10,b+29,color);
}
void letraH(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a,b,b+10,color);
	rellenar(m, a-59, a,b+30,b+39,color);
	rellenar(m, a-29, a-20,b+10,b+29,color);
}
void letraO(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-49, a-10,b,b+10,color);
	rellenar(m, a-49, a-10,b+30,b+39,color);
	rellenar(m, a-9, a,b+10,b+29,color);
	rellenar(m, a-59, a-50,b+10,b+29,color);
}
void letraA(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-49, a,b,b+10,color);
	rellenar(m, a-49, a,b+30,b+39,color);
	rellenar(m, a-29, a-20,b+10,b+29,color);
	rellenar(m, a-59, a-50,b+10,b+29,color);
}
void letraC(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-49, a-10,b,b+10,color);
	rellenar(m, a-9, a,b+10,b+29,color);
	rellenar(m, a-59, a-50,b+10,b+29,color);
}
void letraV(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a,b,b+10,color);
	rellenar(m, a-19, a-10,b+10,b+19,color);
	rellenar(m, a-29, a-20,b+20,b+29,color);
	rellenar(m, a-59, a-30,b+30,b+39,color);
}
void letraQ(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-49, a-10,b,b+10,color);
	rellenar(m, a-49, a-10,b+30,b+39,color);
	rellenar(m, a-9, a,b+10,b+29,color);
	rellenar(m, a-59, a-50,b+10,b+29,color);
	rellenar(m, a-19, a-10,b+20,b+29,color);	
	rellenar(m, a-9, a,b+40,b+49,color);
}
void letraY(char m[TAMANO][TAMANO],int a, int b, char color){
	rellenar(m, a-59, a-40,b,b+10,color);
	rellenar(m, a-9, a,b,b+10,color);
	rellenar(m, a-19, a-10,b+10,b+19,color);
	rellenar(m, a-39, a-30,b+10,b+19,color);
	rellenar(m, a-29, a-20,b+20,b+29,color);
	rellenar(m, a-59, a-30,b+30,b+39,color);
}
//BLOQUES
void bloque(char m[TAMANO][TAMANO],int a, int b){
	rellenar(m, a-99, a,b,b+99,125);
}
void bloqueSombra(char m[TAMANO][TAMANO],int a, int b){
	rellenar(m, a-99, a,b-10,b+99,90);
	rellenar(m, a-99, a,b,b+99,125);
}
//PINCHOS
/*
Para hacer los pinchos utilizaremos un bucle de manera que va haciendo lineas de 2px de altura y 100px de anchura,
la cual irá reduciendose conforme avanza el bucle hasta 0, haciendo asi una forma puntiaguda. Haremos otro bucle 
para la sombra, el cual se detendrá antes y cuya posición en x estará desplazada -10px.
*/
void pincho(char m[TAMANO][TAMANO],int a, int b){
	//Sombra
	for(int i=0;i<50;i++){rellenar(m,a-1-(i*2),a-(i*2),b-10+i,b+99-i,90);}
	//Pincho
	for(int i=0;i<100;i++){rellenar(m,a-1-(i*2),a-(i*2),b+i,b+99-i,125);}
}
void pinchoOscuro(char m[TAMANO][TAMANO],int a, int b){
	//Sombra
	for(int i=0;i<50;i++){rellenar(m,a-1-(i*2),a-(i*2),b-10+i,b+99-i,25);}
	//Pincho
	for(int i=0;i<100;i++){rellenar(m,a-1-(i*2),a-(i*2),b+i,b+99-i,80);}
}

//ESCALERAS
void escalera(char m[TAMANO][TAMANO],int a, int b){
	rellenar(m,a-99,a,b+80,b+90,20);
	rellenar(m,a-20,a-10,b+10,b+82,60);
	rellenar(m,a-50,a-40,b+10,b+82,60);
	rellenar(m,a-80,a-70,b+10,b+82,60);
	rellenar(m,a-99,a,b,b+10,80);
	rellenar(m,a-99,a,b+90,b+99,80);
}
void plataforma(char m[TAMANO][TAMANO], int a, int b){
	rellenar(m, a-40, a,b-10,b+99,25);
	rellenar(m, a-40, a,b,b+99,80);
	bloque(m,599,600);
}
//JUGADOR
void character(char m[TAMANO][TAMANO], int a, int b){
	rellenar(m, a-100, a,b,b+100,200);
}

//PANTALLAS
void start(char m[TAMANO][TAMANO], Datos &datos){
	rellenar(m,0,TAMANO-1,0,TAMANO-1,125); //FONDO GRIS
	rellenar(m, 100, TAMANO-101,100,TAMANO-101,90); //SOMBRA FONDO
	rellenar(m, 100, TAMANO-101,100,TAMANO-111,0); //FONDO NEGRO
	//CAVE SCAPE
	letraC(m,479,290,255);
	letraA(m,479,330,255);
	letraV(m,479,380,255);
	letraE(m,479,430,255);
	letraS(m,479,490,255);
	letraC(m,479,540,255);
	letraA(m,479,580,255);
	letraP(m,479,630,255);
	letraE(m,479,680,255);
	//START | QUIT
	letraS(m,579,245,255);
	letraT(m,579,295,255);
	letraA(m,579,335,255);
	letraR(m,579,385,255);
	letraT(m,579,435,255);
	letraI(m,579,525,255);
	letraQ(m,579,595,255);
	letraU(m,579,655,255);
	letraI(m,579,705,255);
	letraT(m,579,725,255);
	if(datos.iteracionFlecha<7){
		if(datos.selectOption==-1){
			//FLECHA START
			for(int i=0;i<25;i++){rellenar(m,649-1-(i*2),649-(i*2),330+i,330+49-i,255);}
		}else{
			//FLECHA START
			for(int i=0;i<25;i++){rellenar(m,649-1-(i*2),649-(i*2),640+i,640+49-i,255);}
		}
	}
	datos.iteracionFlecha++;
	if(datos.iteracionFlecha>10){datos.iteracionFlecha=0;}
}
void escape(char m[TAMANO][TAMANO], Datos &datos){
	rellenar(m,380, 669,165,835,90); //FONDO GRIS
	rellenar(m, 390, 659,175,825,20); //FONDO NEGRO
	//CAVE SCAPE
	letraC(m,479,290,255);
	letraA(m,479,330,255);
	letraV(m,479,380,255);
	letraE(m,479,430,255);
	letraS(m,479,490,255);
	letraC(m,479,540,255);
	letraA(m,479,580,255);
	letraP(m,479,630,255);
	letraE(m,479,680,255);
	//START | QUIT
	letraP(m,579,245,255);
	letraL(m,579,295,255);
	letraA(m,579,335,255);
	letraY(m,579,385,255);
	letraI(m,579,505,255);
	letraQ(m,579,595,255);
	letraU(m,579,655,255);
	letraI(m,579,705,255);
	letraT(m,579,725,255);
	if(datos.iteracionFlecha<7){
		if(datos.selectOption==-1){
			//FLECHA START
			for(int i=0;i<25;i++){rellenar(m,649-1-(i*2),649-(i*2),300+i,300+49-i,255);}
		}else{
			//FLECHA START
			for(int i=0;i<25;i++){rellenar(m,649-1-(i*2),649-(i*2),640+i,640+49-i,255);}
		}
	}
	datos.iteracionFlecha++;
	if(datos.iteracionFlecha>10){datos.iteracionFlecha=0;}
}
void gameover(char m[TAMANO][TAMANO], int k , Datos &datos){
		rellenar(m,0,TAMANO-1,0,TAMANO-1,0);
		//HAS PERDIDO
		letraH(m,430,265,255);
		letraA(m,430,315,255);
		letraS(m,430,365,255);
		letraP(m,430,435,255);
		letraE(m,430,485,255);
		letraR(m,430,525,255);
		letraD(m,430,575,255);
		letraI(m,430,625,255);
		letraD(m,430,645,255);
		letraO(m,430,695,255);
		//PULSA ENTER
		letraP(m,510,255,255);
		letraU(m,510,305,255);
		letraL(m,510,355,255);
		letraS(m,510,395,255);
		letraA(m,510,445,255);
		letraE(m,510,515,255);
		letraN(m,510,555,255);
		letraT(m,510,625,255);
		letraE(m,510,665,255);
		letraR(m,510,705,255);
		if(k==13){datos.reset();}
}
void win(char m[TAMANO][TAMANO],int k , Datos &datos){
		rellenar(m,0,TAMANO-1,0,TAMANO-1,255);
		letraF(m,529,440,255-datos.colorWin);
		letraI(m,529,480,255-datos.colorWin);
		letraN(m,529,500,255-datos.colorWin);
		if(datos.colorWin!=200){datos.colorWin+=10;}
		if(k!=-1){datos.quit=true;}
}
//FIN GRAFICOS
//----------------------------------------------------------------------------------------------------------------------------

