#ifndef W3D_MUSIC_BACKEND_H
#define W3D_MUSIC_BACKEND_H
// ============================================================================
//  CONTRATO dispatcher <-> backend de MUSICA (interno del Core).
//  Lo incluyen W3dMusic.cpp y cada backend (W3dMusicWeb.cpp / ...): una firma
//  divergente se detecta al compilar, no como bug de link silencioso.
// ============================================================================
#include <stddef.h>

namespace w3dEngine {

class W3dMusic;

W3dMusic* W3dMusicPlayMemoryBackend(const void* bytes, size_t len, const char* mime, bool loop, float volume);
W3dMusic* W3dMusicPlayBackend(const char* path, bool loop, float volume);
void      W3dMusicUnlockBackend();

} // namespace w3dEngine

#endif // W3D_MUSIC_BACKEND_H
