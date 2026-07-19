#pragma once
#include <map>
#include <set>
#include <def.hpp>

class HandleRegistry {
public:
    UHandle allocate();
    void free(UHandle handle);

    bool check(UHandle handle);

private:
    Fingerprint generateFingerprint();
    void removeFingerprint(Fingerprint fp);

    Handle nextHandle = 0;

    std::map<Handle, Fingerprint> usingHandles;
    std::set<Fingerprint> usingFingerprints;

    std::set<Handle> freeHandles;
};
