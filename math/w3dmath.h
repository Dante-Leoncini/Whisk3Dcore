#ifndef W3DMATH_H
#define W3DMATH_H

/*
 * Helpers matematicos chicos COMPARTIDOS (4 OS, C++03: sin std::clamp).
 * Header-only y sin tipos propios
 */

inline int w3dClamp(int v, int mn, int mx) {
    if (v < mn) return mn;
    if (v > mx) return mx;
    return v;
}

inline float w3dClampf(float v, float mn, float mx) {
    if (v < mn) return mn;
    if (v > mx) return mx;
    return v;
}

#endif
