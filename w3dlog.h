#pragma once
// ============================================================================
//  Whisk3DCore (engine) — log de diagnostico (modo DEV), comun a los 4 OS.
//
//  Toda app deberia poder prender/apagar un log: es una facilidad BASICA, asi
//  que el SINK (abrir el archivo, el timestamp, el on/off) vive en el core. Lo
//  que NO es del core es COMO cada plataforma arma lo que loguea (p.ej. Symbian
//  formatea sus descriptors y termina llamando a w3dLog) — eso queda en el shell.
//
//  Backend por plataforma (ver w3dlog.cpp): Symbian escribe a e:\whisk3d.log via
//  RFile; PC/escritorio a whisk3d.log via stdio. Cada linea se abre/escribe/
//  cierra con flush: si la app muere, el log queda completo hasta el final.
//
//  Poner W3D_DEV_LOG en 0 para builds de release: las llamadas quedan vacias.
// ============================================================================
#define W3D_DEV_LOG 1

#if W3D_DEV_LOG
void w3dLogReset();                  // trunca el log (al iniciar la app)
void w3dLog(const char* aMsg);       // una linea (timestamp + texto)
void w3dLogf(const char* aFmt, ...); // idem, printf-style
#else
inline void w3dLogReset() {}
inline void w3dLog(const char*) {}
inline void w3dLogf(const char*, ...) {}
#endif
