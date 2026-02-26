#include "resource_cache.h"

#include <cassert>
#include <iostream>

int main() {
    ResourceCache cache;

    {
        auto a1 = cache.get("asset1");
        auto a2 = cache.get("asset1");

        // Both should point to the same cached resource.
        assert(a1 == a2);

        // Load a second asset just to populate the cache a bit.
        [[maybe_unused]] auto b = cache.get("asset2");

        std::cout << "Cache entries (in scope): " << cache.size() << '\n';
    }

    std::cout << "Cache entries (after scope): " << cache.size() << '\n';

    // Remove expired entries from the cache.
    cache.prune();
    std::cout << "Cache entries (after prune): " << cache.size() << '\n';

    // Load again after unload (resource is recreated).
    auto a3 = cache.get("asset1");
    assert(a3);

    std::cout << "Done.\n";
    return 0;
}

