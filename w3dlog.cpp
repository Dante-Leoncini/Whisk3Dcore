// ============================================================================
//  Whisk3DCore (engine) — log de diagnostico. Ver w3dlog.h.
// ============================================================================
#include "w3dlog.h"

#if W3D_DEV_LOG

#include <stdio.h>
#include <stdarg.h>

#ifdef W3D_SYMBIAN
// ---------------------------------------------------------------------------
//  Backend Symbian: RFile a e:\whisk3d.log (la tarjeta de memoria). El N95 no
//  tiene stdio a un archivo confiable; ademas asi sobrevive a un cuelgue.
// ---------------------------------------------------------------------------
#include <e32std.h>
#include <f32file.h>
#include "fscompat.h" // cerrar RFs sin importar efsrv@390 (Symbian^3)

static void EscribirSymbian(const char* aMsg, bool aTruncar) {
    RFs fs;
    if (fs.Connect() != KErrNone) {
        return; // sin tarjeta E: u otro problema: no hay log, pero no molesta
    }
    RFile f;
    _LIT(KPath, "e:\\whisk3d.log");
    TInt err;
    if (aTruncar) {
        err = f.Replace(fs, KPath, EFileWrite | EFileShareAny);
    } else {
        err = f.Open(fs, KPath, EFileWrite | EFileShareAny);
        if (err == KErrNotFound) {
            err = f.Create(fs, KPath, EFileWrite | EFileShareAny);
        }
    }
    if (err == KErrNone) {
        TInt pos = 0;
        f.Seek(ESeekEnd, pos);
        TBuf8<256> line;
        line.AppendNum((TUint)User::NTickCount()); // timestamp en ms
        line.Append(' ');
        TPtrC8 m((const TUint8*)(aMsg ? aMsg : ""));
        line.Append(m.Left(200));
        line.Append(_L8("\r\n"));
        f.Write(line);
        f.Flush(); // a disco YA, por si lo proximo es el cuelgue
        f.Close();
    }
    FsCloseCompat(fs);
}

void w3dLogReset() { EscribirSymbian("=== Whisk3D DEV LOG: inicio de sesion ===", true); }
void w3dLog(const char* aMsg) { EscribirSymbian(aMsg, false); }

#else
// ---------------------------------------------------------------------------
//  Backend escritorio (PC/Android): whisk3d.log via stdio (append + flush).
// ---------------------------------------------------------------------------
#include <time.h>

static void EscribirStdio(const char* aMsg, bool aTruncar) {
    FILE* f = fopen("whisk3d.log", aTruncar ? "w" : "a");
    if (!f) return;
    unsigned long ms = (unsigned long)(clock() * 1000.0 / CLOCKS_PER_SEC);
    fprintf(f, "%lu %s\r\n", ms, aMsg ? aMsg : "");
    fflush(f);
    fclose(f);
}

void w3dLogReset() { EscribirStdio("=== Whisk3D DEV LOG: inicio de sesion ===", true); }
void w3dLog(const char* aMsg) { EscribirStdio(aMsg, false); }

#endif

// formato printf comun a las dos plataformas (el sink ya es por-backend)
void w3dLogf(const char* aFmt, ...) {
    char buf[256];
    va_list ap;
    va_start(ap, aFmt);
    vsnprintf(buf, sizeof(buf), aFmt ? aFmt : "", ap);
    va_end(ap);
    w3dLog(buf);
}

#endif // W3D_DEV_LOG
