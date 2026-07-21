#ifndef W3D_PANTALLA_H
#define W3D_PANTALLA_H
// ============================================================================
//  PANTALLA — tamanio y ORIENTACION actuales, como valor GLOBAL del motor.
//
//  Muchisimas decisiones de un juego dependen de si estamos en VERTICAL (retrato)
//  u HORIZONTAL (apaisado): que arte se usa (portada cuadrada vs 4:3), como se
//  reparte el HUD, cuantas columnas entran... En vez de que cada modulo se pase
//  el ancho y el alto y saque su propia conclusion, el Core lo guarda UNA vez.
//
//  La regla es simple y vale en todas las plataformas (PC, celular, N95, consola):
//      HORIZONTAL  <=>  ancho > alto
//
//  La app llama PantallaSetTam(w, h) cuando arma el frame (es baratisimo: dos
//  enteros) y despues cualquier parte del motor puede preguntar sin dependencias.
// ============================================================================

namespace w3dEngine {

enum Orientacion {
    OrientVertical = 0,     // retrato: mas alto que ancho (o cuadrado)
    OrientHorizontal = 1    // apaisado: mas ancho que alto
};

// La setea la app cada frame (o cuando cambia el tamanio de la ventana).
void PantallaSetTam(int ancho, int alto);

int   PantallaAncho();
int   PantallaAlto();
float PantallaAspecto();          // ancho / alto (1 si todavia no se seteo)

Orientacion PantallaOrientacion();
bool  PantallaEsHorizontal();     // atajo: ancho > alto
bool  PantallaEsVertical();

// true SOLO en el frame en que la orientacion cambio (para recargar arte, rearmar
// el layout, etc). Se calcula en PantallaSetTam.
bool  PantallaCambioOrientacion();

} // namespace w3dEngine

#endif // W3D_PANTALLA_H
