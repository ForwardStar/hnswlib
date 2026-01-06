# Hnswlib (ForwardStar version)

Original Hnswlib repo: [https://github.com/nmslib/hnswlib](https://github.com/nmslib/hnswlib)

Update of this version: the original Hnswlib will store materialized vectors in each ``hnswlib::HierarchicalNSW`` class. However, what if you want to construct multiple HNSW subgraphs over the same set of vectors? This will lead to substantial memory waste since the class will store each vector multiple times. In this version, we change the class to support referring vectors without materialized store.

Specifically, the original API is:
```cpp
void addPoint(const void *data_point, labeltype label, bool replace_deleted = false);
```

We change it to:
```cpp
void addPoint(labeltype label, bool replace_deleted = false);
```

You can insert a vector to the HNSW graph, by directing using its ID as label.

When initializing HNSW class, the API has changed to:
```cpp
HierarchicalNSW(
        SpaceInterface<dist_t> *s,
        size_t max_elements,
        const void* external_data,
        size_t M = 16,
        size_t ef_construction = 200,
        size_t random_seed = 100,
        bool allow_replace_deleted = false);
```

where an extra argument ``external_data`` is needed. This should be the pointer to where you store the actual vectors. Specifically:
```cpp
external_data[i] == the vector with label i
```

To debug:
```sh
mkdir build && cd build
cmake -S .. -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
```