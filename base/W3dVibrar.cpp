// ============================================================================
//  W3dVibrar.cpp — ver W3dVibrar.h.
//
//  WEB: dos caminos, porque no hay uno solo que sirva en todos lados.
//    · navigator.vibrate(ms): Android. Preciso, se elige la duracion.
//    · iOS/Safari: navigator.vibrate NO EXISTE. Lo unico que dispara el motor
//      haptico desde una pagina es activar un <input type="checkbox" switch>
//      (iOS 17.4+): el sistema hace su "tic" al cambiarlo. No se puede elegir
//      intensidad ni duracion, y Apple lo puede cerrar cuando quiera; por eso
//      todo esto vive detras de una API que promete poco.
//  SYMBIAN/otros: el hueco esta listo (ver el #else) para engancharlo con la
//  API de vibracion del sistema o con el rumble de un mando.
// ============================================================================
#include "W3dVibrar.h"
#include "W3dConfig.h"

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

namespace w3dEngine {

static bool gActiva = true;
static bool gListo  = false;

#ifdef __EMSCRIPTEN__

// 1 = el navegador tiene navigator.vibrate (Android). 0 = hay que usar el truco (iOS).
EM_JS(int, w3dVibJS_Nativa, (), {
    return (navigator && typeof navigator.vibrate === 'function') ? 1 : 0;
});

// Prepara el interruptor escondido que usa el truco de iOS. Tiene que estar EN el documento
// y no puede estar display:none (si no, iOS no lo considera interactuable).
EM_JS(void, w3dVibJS_Init, (), {
    if (Module.__w3dVibSw) return;
    try {
        var l = document.createElement('label');
        l.style.cssText = 'position:fixed;left:-100px;top:-100px;width:1px;height:1px;' +
                          'opacity:0.001;pointer-events:none;';
        var i = document.createElement('input');
        i.type = 'checkbox';
        i.setAttribute('switch', '');        // iOS 17.4+: lo dibuja como interruptor -> haptica
        l.appendChild(i);
        document.body.appendChild(l);
        Module.__w3dVibSw = i;
    } catch (e) {}
});

EM_JS(void, w3dVibJS_Vibrar, (int ms), {
    try {
        if (navigator && typeof navigator.vibrate === 'function') { navigator.vibrate(ms); return; }
        // iOS: el "tic" sale de ALTERNAR el interruptor. Se alterna y listo; el valor da igual.
        var i = Module.__w3dVibSw;
        if (i) { i.checked = !i.checked; i.dispatchEvent(new Event('change', { bubbles: false })); }
    } catch (e) {}
});

EM_JS(int, w3dVibJS_Disponible, (), {
    if (navigator && typeof navigator.vibrate === 'function') return 1;
    return Module.__w3dVibSw ? 1 : 0;      // el truco: al menos se puede intentar
});

void VibrarInit() {
    if (gListo) return;
    gListo = true;
    ConfigLoad();
    gActiva = ConfigGetInt("vibracion", 1) != 0;
    w3dVibJS_Init();
}

void Vibrar(VibFuerza f) {
    if (!gActiva) return;
    if (!gListo) VibrarInit();
    int ms = (f == VibGolpe) ? 32 : (f == VibToque ? 18 : 10);
    w3dVibJS_Vibrar(ms);
}

bool VibrarDisponible() { if (!gListo) VibrarInit(); return w3dVibJS_Disponible() != 0; }

#else   // ---------------- escritorio / Symbian / consolas ----------------

void VibrarInit() {
    if (gListo) return;
    gListo = true;
    ConfigLoad();
    gActiva = ConfigGetInt("vibracion", 1) != 0;
}

// Hueco a completar por plataforma: en Symbian con la API de vibracion del telefono, en
// escritorio con el rumble del mando. Mientras tanto no hace nada y nadie se rompe.
void Vibrar(VibFuerza /*f*/) { if (!gListo) VibrarInit(); }

bool VibrarDisponible() { return false; }

#endif

void VibrarSetActiva(bool activa) {
    if (!gListo) VibrarInit();
    gActiva = activa;
    ConfigSetInt("vibracion", activa ? 1 : 0);
    ConfigSave();
}

bool VibrarActiva() { if (!gListo) VibrarInit(); return gActiva; }

} // namespace w3dEngine
