// ============================================================================
//  w3dPantalla.cpp — tamanio y orientacion globales. Ver w3dPantalla.h.
//  Sin dependencias: son cuatro enteros. C++03 puro (sirve igual en Symbian).
// ============================================================================
#include "w3dPantalla.h"

namespace w3dEngine {

static int  gW = 0, gH = 0;
static Orientacion gOri = OrientVertical;
static bool gCambio = false;

void PantallaSetTam(int ancho, int alto) {
    if (ancho < 1) ancho = 1;
    if (alto  < 1) alto  = 1;
    Orientacion nueva = (ancho > alto) ? OrientHorizontal : OrientVertical;
    gCambio = (gW != 0 || gH != 0) && (nueva != gOri);   // el primer seteo no cuenta como cambio
    gW = ancho; gH = alto; gOri = nueva;
}

int   PantallaAncho()   { return gW; }
int   PantallaAlto()    { return gH; }
float PantallaAspecto() { return (gH > 0) ? (float)gW / (float)gH : 1.0f; }

Orientacion PantallaOrientacion() { return gOri; }
bool PantallaEsHorizontal()       { return gOri == OrientHorizontal; }
bool PantallaEsVertical()         { return gOri == OrientVertical; }
bool PantallaCambioOrientacion()  { return gCambio; }

} // namespace w3dEngine
