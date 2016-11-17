#include <cstdlib>
#include <ctime>
#include "miniwin.h"

using namespace miniwin;

const int TAM= 25;
const int FILAS=20;
const int COLUMNAS=10;

struct Coord{
    int x,y;
};
struct Pieza{
    Coord orig; //bloque central
    Coord bloques[3]; //poiocion relativa
    int color;
    Coord Posicion(int n)const;
};
Coord Pieza::Posicion(int n)const{
    Coord ret= {orig.x, orig.y};
    if(n!=0){
        ret.x += bloques[n-1].x;
        ret.y += bloques[n-1].y;
    }
    return ret;
}
typedef int Tablero[COLUMNAS][FILAS];

void cuadrado(int x, int y){
    rectangulo_lleno(1+x*TAM, 1+y*TAM, x*TAM+TAM, y*TAM+TAM); //((0,0)(25,25) 1(borde)
}
void pinta_pieza(const Pieza &P){
    color(P.color);
    for(int i=0; i<4; i++){
        Coord c= P.Posicion(i);
        cuadrado(c.x, c.y);
    }
    /*cuadrado(P.orig.x, P.orig.y);//bloque central
    for(int i=0; i<3; i++){
         cuadrado(P.orig.x + P.bloques[i].x,P.orig.y + P.bloques[i].y);
    } */

}
Coord rota_derecha(const Coord &c){
    Coord ret={-c.y, c.x};
    return ret;
}
Coord rota_izquierda(const Coord &c){
    Coord ret={c.y, -c.x};
    return ret;
}
void rota_derecha(Pieza &P){
    for(int i=0; i<3; i++)
        P.bloques[i]=rota_derecha(P.bloques[i]);
}
void rota_izquierda(Pieza &P){
    for(int i=0; i<3; i++)
        P.bloques[i]=rota_izquierda(P.bloques[i]);
}
void tableroVacio(Tablero& T){
    for(int i=0; i< COLUMNAS; i++){
        for(int j=0; j<FILAS; j++){
            T[i][j]=NEGRO;//bloque vavio
        }
    }
}
void tablero_pinta(const Tablero &T){
     for(int i=0; i<COLUMNAS; i++){
        for(int j=0; j<FILAS; j++){
                color(T[i][j]);
                cuadrado(i,j);
        }
     }
}
void tablero_incrustraPieza(Tablero &T, const Pieza &P){
    for (int i=0; i<4; i++){
        Coord c = P.Posicion(i);
        T[c.x][c.y]=P.color;
    }
}
bool tablero_colision(const Tablero &T, const Pieza &P){
    for (int i=0; i<4; i++){
        Coord c=P.Posicion(i);
        //comprobar limites
        if(c.x<0 || c.x>COLUMNAS)
            return true;
        if(c.y < 0 || c.y > FILAS)
            return true;
        if(T[c.x][c.y] != NEGRO)
            return true;
    }
    return false;
}
/*
void pieza_nueva(Pieza &P){
    P.orig.x=5;
    P.orig.y=3;
    P.bloques[0].x=1; P.bloques[0].y=0;
    P.bloques[1].x=1; P.bloques[1].y=1;
    P.bloques[2].x=0; P.bloques[2].y=1;
    P.color=1+rand() % 6;
 }*/

int main(){
    vredimensiona(TAM*COLUMNAS,TAM*FILAS);
   // srand(time(0)); //inizializar numeros al azar
    Tablero T;
    tableroVacio(T);
    T[0][19]=VERDE;
    tablero_pinta(T);
    //Pieza c;
    //pieza_nueva(c);
    Pieza c={{5,10},{{1,0},{1,1},{0,1}}, MAGENTA};
    pinta_pieza(c);
    refresca();

    int t =tecla();
    while( t != ESCAPE){
    //copiar la pos actual
        int x=c.orig.x;
        int y=c.orig.y;
        if(t == DERECHA){
            c.orig.x++;
        }
        else if(t==IZQUIERDA){
            c.orig.x--;
        }
        else if( t==ARRIBA){
            c.orig.y--;
        }
        else if(t==ABAJO){
            c.orig.y++;
        }
    //mirar si hay colisioj
        if(tablero_colision(T,c)){
            c.orig.x=x;
            c.orig.y=y;
        }
    //repintar
        if(t !=NINGUNA){
            borra();
            tablero_pinta(T);
            pinta_pieza(c);
            refresca();
        }
        t=tecla();
    }
    vcierra();
}
