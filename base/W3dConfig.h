#ifndef W3D_CONFIG_H
#define W3D_CONFIG_H
// ============================================================================
//  CONFIGURACION PERSISTENTE del motor (clave = valor).
//
//  Guarda ajustes que tienen que SOBREVIVIR a cerrar la app: volumen, mute,
//  idioma, calidad grafica, ultimo juego jugado... La app NO se entera de donde
//  se guarda: cada plataforma usa lo suyo.
//
//    · Web (emscripten) -> localStorage del navegador (una sola entrada).
//    · Desktop / Symbian / consolas -> un archivo de texto (w3dconfig.txt).
//
//  Formato en disco: lineas "clave=valor" en UTF-8. Simple a proposito: se puede
//  abrir con un editor y no arrastra dependencias (ni JSON ni XML).
//
//  Uso tipico:
//      w3dEngine::ConfigLoad();                          // al arrancar
//      float v = w3dEngine::ConfigGetFloat("vol", 0.6f);
//      w3dEngine::ConfigSetInt("mute", 1);
//      w3dEngine::ConfigSave();                          // al cambiar algo
//
//  Limites (a proposito, sin memoria dinamica: sirve igual en Symbian):
//  W3D_CFG_MAX entradas, claves de W3D_CFG_KEY y valores de W3D_CFG_VAL chars.
// ============================================================================

namespace w3dEngine {

enum { W3D_CFG_MAX = 48, W3D_CFG_KEY = 32, W3D_CFG_VAL = 96 };

// Carga la configuracion del almacenamiento de la plataforma. false si no habia
// nada guardado todavia (no es un error: son los valores por defecto).
bool ConfigLoad();

// Escribe la configuracion. Llamala cuando cambie algo que quieras conservar.
bool ConfigSave();

// Lectura con valor POR DEFECTO (lo que devuelve si la clave no existe).
const char* ConfigGetStr(const char* clave, const char* porDefecto);
float       ConfigGetFloat(const char* clave, float porDefecto);
int         ConfigGetInt(const char* clave, int porDefecto);

// Escritura EN MEMORIA (no guarda sola: llama ConfigSave cuando corresponda).
void ConfigSetStr(const char* clave, const char* valor);
void ConfigSetFloat(const char* clave, float valor);
void ConfigSetInt(const char* clave, int valor);

// Borra todo (en memoria). Util para un "restablecer ajustes".
void ConfigClear();

// Cambia el nombre del archivo/entrada donde se guarda (por defecto "w3dconfig").
// Llamalo ANTES de ConfigLoad si tu app quiere su propio archivo.
void ConfigSetNombre(const char* nombre);

} // namespace w3dEngine

#endif // W3D_CONFIG_H
