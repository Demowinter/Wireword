#include <random>
#include <cstdint>
#include <def.hpp>
#include <handlereg.hpp>

UHandle HandleRegistry::allocate() {
    Fingerprint fp = generateFingerprint();

    if (freeHandles.size()) {
        auto it = freeHandles.begin();

        UHandle handle{*it, fp};

        freeHandles.erase(it);

        return handle;
    }

    if (nextHandle < 0) return invalidUHandle;

    Handle handle = nextHandle++;

    usingHandles[handle] = fp;

    return {handle, fp};
}

void HandleRegistry::free(UHandle handle) {
    if (!check(handle)) return;

    usingHandles.erase(handle.handle);
    freeHandles.insert(handle.handle);

    removeFingerprint(handle.fp);
}

bool HandleRegistry::check(UHandle handle) {
    if (handle == invalidUHandle) return false;

    auto it = usingHandles.find(handle.handle);

    return it != usingHandles.end() && it->second == handle.fp;
}

UHandle HandleRegistry::getUnique(Handle handle) {
    return {handle, usingHandles.at(handle)};
}

Fingerprint HandleRegistry::generateFingerprint() {
    static std::random_device rd;
    static std::mt19937_64 mt(rd());
    static std::uniform_int_distribution<Fingerprint> fpgen{1};

    Fingerprint fp;

    do { fp = fpgen(mt); } while (usingFingerprints.find(fp) != usingFingerprints.end());

    usingFingerprints.insert(fp);

    return fp;
}

void HandleRegistry::removeFingerprint(Fingerprint fp) {
    usingFingerprints.erase(fp);
}