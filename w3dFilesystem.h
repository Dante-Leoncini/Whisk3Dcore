#pragma once

#include <string>
#include <vector>

namespace w3dFileSystem {
    // Inicializa rutas globales
    void Init();

    // rutas cacheadas
    const std::string& GetExeDir();
    const std::string& GetResDir();

    // ------------------------------------------------------------------
    //  Navegacion de archivos (la usa el File browser compartido). El
    //  backend es por plataforma: PC/escritorio = std::filesystem; Symbian
    //  (cuando se porte) = RDir. Las rutas usan '/' como separador.
    // ------------------------------------------------------------------
    struct DirEntry {
        std::string name;
        bool isDir;
    };

    // lista 'path' en 'out' (carpetas primero, alfabetico). false si no se pudo.
    bool ListDir(const std::string& path, std::vector<DirEntry>& out);

    // carpeta del usuario (HOME / USERPROFILE) como punto de partida
    std::string GetHomeDir();

    // accesos rapidos del panel lateral (Home, Escritorio, Documentos, unidades)
    //esto lo voy a quitar y mover a Whisk3D editor... es que en realidad es algo del editor 3D. no hace falta un
    //bookmak en un juego por ejemplo
    struct Bookmark {
        std::string name;
        std::string path;
        bool user;   // true = lo agrego el usuario (con "+") -> se PERSISTE en bookmarks.txt
        Bookmark() : user(false) {}
    };

    void GetBookmarks(std::vector<Bookmark>& out); // auto (Home/drives) + los del usuario
    // guarda los bookmarks con user==true en disco (GetResDir()/bookmarks.txt). Lo llaman el
    // "+" y el "-" del explorador para que queden persistidos entre sesiones.
    void SaveUserBookmarks(const std::vector<Bookmark>& all);

    // helpers de ruta (no tocan disco)
    std::string ParentPath(const std::string& path); // sube un nivel
    std::string JoinPath(const std::string& dir, const std::string& name);
}
