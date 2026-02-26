#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/*
Resource represents a loaded asset.
In a real engine this might be a texture, sound, or mesh.
*/

struct Resource {

    std::string id;

    std::vector<unsigned char> data;

    Resource(std::string rid, std::vector<unsigned char> bytes)
        : id(std::move(rid)), data(std::move(bytes)) 
        
    {

    // Logging resource creation for demonstration purposes   
    std::cout << "Resource " << id << " created with " << data.size() << " bytes\n";

    }

    ~Resource() {
        // Logging resource destruction for demonstration purposes
        std::cout << "Resource " << id << " destroyed\n";
    }

    // Disable copy to prevent accidental copying of resources
    Resource(const Resource&) = delete; 
    Resource& operator=(const Resource&) = delete; 
};

/* 
ResourceCache manages loaded resources. 
It uses weak_ptr to allow resources to be automatically removed from the cache when they are no longer in use.
*/

class ResourceCache {

public:

    // Retrieves a resource by ID, loading it if it's not already cached or if the cached version has expired.
    std::shared_ptr<Resource> get(const std::string& id) {

        auto it = cache_.find(id);

        if (it != cache_.end()) {

            if (auto existing = it->second.lock()) {
                std::cout << "Resource " << id << " retrieved from cache\n";
                return existing;
            }

            std::cout << "Resource " << id << " expired, reloading\n";
        } else {
            std::cout << "Resource " << id << " not found in cache, loading\n";
        }


        // Simulate loading resource data (in a real engine this would read from disk or another source).
        std::vector<unsigned char> bytes;
        bytes.reserve(id.size() + 1);

        for (char c : id) {
            bytes.push_back(static_cast<unsigned char>(c));
        }

        bytes.push_back(0xFF);
        
        auto resource = std::make_shared<Resource>(id, std::move(bytes));
        cache_[id] = resource;

        return resource;
    }


    // Removes expired resources from the cache.
    void prune() {

        for (auto it = cache_.begin(); it != cache_.end();) {

            if (it->second.expired()) {

                std::cout << "Pruning expired resource: "

                          << it->first << '\n';

                it = cache_.erase(it);

            } else {
                ++it;
            }
        }
    }

    // Returns the number of entries in the cache (including expired ones).
    std::size_t size() const {
        return cache_.size();
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Resource>> cache_;
};