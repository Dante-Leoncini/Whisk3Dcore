#ifndef W3D_AUDIO_BACKEND_H
#define W3D_AUDIO_BACKEND_H
// ============================================================================
//  CONTRATO dispatcher <-> backend del MIXER de efectos (interno del Core).
//
//  W3dAudio.cpp (el mixer portable) llama estas cuatro; cada plataforma las
//  implementa (W3dAudioSDL.cpp / W3dAudioSymbian.cpp / ...). Este header lo
//  incluyen LOS DOS lados: si una firma diverge, el error salta al COMPILAR
//  (antes estaban declaradas a mano en cada .cpp y una divergencia solo se
//  descubria como simbolo sin resolver al linkear... o nunca).
// ============================================================================

namespace w3dEngine {

bool W3dAudioBackendInit(int sampleRate);   // abre el dispositivo y arranca a pedir W3dAudioMix
void W3dAudioBackendShutdown();
void W3dAudioBackendLock();                 // exclusion con el hilo de audio
void W3dAudioBackendUnlock();

} // namespace w3dEngine

#endif // W3D_AUDIO_BACKEND_H
