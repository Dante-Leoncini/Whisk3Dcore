// ============================================================================
//  W3dMusicWeb.cpp — backend WEB (emscripten) de MUSICA. Solo con -DW3D_ENABLE_MUSIC.
//
//  El NAVEGADOR decodifica (mp3/ogg/aac, normalmente por hardware): metemos los bytes
//  en un elemento <audio> y el reproduce en loop por su cuenta. No se decodifica a PCM
//  ni se mezcla en la CPU -> un tema de varios minutos no cuesta RAM ni tamanio de asset.
//
//  ASSETS PROTEGIDOS: los bytes salen descifrados de un .w3dpack; se arma un Blob INTERNO,
//  el <audio> NO se agrega al DOM y la URL del blob se REVOCA al cargar -> no queda ningun
//  archivo con nombre ni URL plana que copiar desde el inspector.
//
//  Autoplay: los navegadores exigen un gesto del usuario. Llamalo desde el toque/click
//  (o despues de uno); si lo bloquean, el play() falla en silencio y la app sigue igual.
// ============================================================================
#ifdef W3D_ENABLE_MUSIC

#include "W3dMusic.h"
#include <emscripten.h>

// --- glue JS: pool de <audio> indexado por handle entero ---
// DESBLOQUEO: estrena un <audio> con un wav de silencio DENTRO del gesto del usuario y lo
// deja reservado. Un elemento ya estrenado puede cambiar de src y volver a sonar SIN gesto:
// asi la musica que todavia se estaba descargando suena igual cuando llega (iOS/Safari).
// Ademas de estrenar el <audio>, RE-ACTIVA el AudioContext de WebAudio (el que usa el mixer
// de efectos por SDL). En iOS nace SUSPENDIDO y solo se puede reanudar DENTRO de un gesto del
// usuario: si no, los efectos (ej: el clic al pasar de portada) no suenan nunca.
EM_JS(void, w3dMusicJS_Unlock, (), {
    // SOLO registra la funcion que estrena el <audio>; NO lo estrena aca, porque esto se llama al
    // arrancar (fuera de cualquier gesto) y ese play() lo bloquea el navegador. Quien la llama de
    // verdad es el listener de gesto de W3dClip (touchstart/click sobre el documento).
    // (Antes se creaba el elemento aca igual: quedaba "reservado" pero NUNCA estrenado, y despues
    //  el estreno del gesto lo salteaba por ya existir -> en iOS la musica no sonaba.)
    Module.__w3dPrimeMusica = function () {
        var a = Module.__w3dPrimed;
        if (!a) {
            a = new Audio();
            a.setAttribute('playsinline', '');
            a.preload = 'auto';
            Module.__w3dPrimed = a;
        }
        if (a.__w3dOk) return;                       // ya esta estrenado
        a.src = 'data:audio/wav;base64,UklGRiwAAABXQVZFZm10IBAAAAABAAEAQB8AAEAfAAABAAgAZGF0YQgAAACAgICAgICAgA==';
        var p = a.play();
        if (p && p.then) p.then(function () { a.__w3dOk = true; }).catch(function () {});
        else a.__w3dOk = true;
    };
    // si el gesto del usuario ya paso, se estrena en el acto
    if (Module.__w3dGestoOk) Module.__w3dPrimeMusica();
});

EM_JS(int, w3dMusicJS_OpenMem, (const unsigned char* ptr, int len, const char* mimePtr, int loop, float vol), {
    if (!Module.__w3dMusic) Module.__w3dMusic = [];
    var blob = new Blob([HEAPU8.subarray(ptr, ptr + len)], { type: UTF8ToString(mimePtr) });
    var url = URL.createObjectURL(blob);
    var a = (Module.__w3dPrimed && Module.__w3dPrimed.__w3dOk) ? Module.__w3dPrimed : new Audio();
    if (a === Module.__w3dPrimed) Module.__w3dPrimed = null;   // se consume el estrenado
    a.setAttribute('playsinline', '');
    a.src = url; a.loop = !!loop; a.volume = vol;
    a.addEventListener('loadeddata', function(){ URL.revokeObjectURL(url); }); // el blob deja de resolverse
    var p = a.play(); if (p && p.catch) p.catch(function(){});                 // si el autoplay se bloquea, silencio
    Module.__w3dMusic.push(a);
    return Module.__w3dMusic.length - 1;
});
EM_JS(int, w3dMusicJS_OpenUrl, (const char* pathPtr, int loop, float vol), {
    if (!Module.__w3dMusic) Module.__w3dMusic = [];
    var a = new Audio();
    a.src = UTF8ToString(pathPtr); a.loop = !!loop; a.volume = vol;
    var p = a.play(); if (p && p.catch) p.catch(function(){});
    Module.__w3dMusic.push(a);
    return Module.__w3dMusic.length - 1;
});
EM_JS(void, w3dMusicJS_Stop, (int h), {
    var a = Module.__w3dMusic && Module.__w3dMusic[h];
    if (a) { try { a.pause(); a.src = ''; } catch(e){} Module.__w3dMusic[h] = null; }
});
// OJO iOS: en iPhone/iPad la propiedad .volume de un <audio> es de SOLO LECTURA (el volumen
// lo maneja el usuario con los botones del aparato): asignarla NO hace nada y la musica se
// escucha a todo volumen. Lo que SI se respeta es .muted -> el silencio se hace con muted, y
// el volumen fino queda para las plataformas que lo permiten (desktop/Android).
EM_JS(void, w3dMusicJS_Vol, (int h, float v), {
    var a = Module.__w3dMusic && Module.__w3dMusic[h];
    if (!a) return;
    a.volume = v;
    a.muted  = (v <= 0.001);
});
EM_JS(void, w3dMusicJS_Pausa, (int h, int pausar), {
    var a = Module.__w3dMusic && Module.__w3dMusic[h];
    if (!a) return;
    try {
        if (pausar) a.pause();
        else { var p = a.play(); if (p && p.catch) p.catch(function(){}); }
    } catch (e) {}
});
EM_JS(int, w3dMusicJS_Playing, (int h), {
    var a = Module.__w3dMusic && Module.__w3dMusic[h];
    return (a && !a.paused) ? 1 : 0;
});

namespace w3dEngine {

class W3dMusicWeb : public W3dMusic {
public:
    int h;
    explicit W3dMusicWeb(int handle) : h(handle) {}
    virtual ~W3dMusicWeb() { Stop(); }
    virtual void Stop() { if (h >= 0) { w3dMusicJS_Stop(h); h = -1; } }
    virtual void AplicarVolumen(float v) { if (h >= 0) w3dMusicJS_Vol(h, v); }
    virtual bool Playing() const { return h >= 0 && w3dMusicJS_Playing(h) != 0; }
    virtual void Pausar()   { if (h >= 0) w3dMusicJS_Pausa(h, 1); }
    virtual void Reanudar() { if (h >= 0) w3dMusicJS_Pausa(h, 0); }
};

W3dMusic* W3dMusicPlayMemoryBackend(const void* bytes, size_t len, const char* mime, bool loop, float volume) {
    int h = w3dMusicJS_OpenMem((const unsigned char*)bytes, (int)len,
                               mime ? mime : "audio/mpeg", loop ? 1 : 0, volume);
    return (h < 0) ? 0 : new W3dMusicWeb(h);
}
void W3dMusicUnlockBackend() { w3dMusicJS_Unlock(); }

W3dMusic* W3dMusicPlayBackend(const char* path, bool loop, float volume) {
    int h = w3dMusicJS_OpenUrl(path, loop ? 1 : 0, volume);
    return (h < 0) ? 0 : new W3dMusicWeb(h);
}

} // namespace w3dEngine

#endif // W3D_ENABLE_MUSIC
