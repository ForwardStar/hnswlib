# Hnswlib (ForwardStar version)

Original Hnswlib repo: [https://github.com/nmslib/hnswlib](https://github.com/nmslib/hnswlib)

Update of this version: optimize the library for constructing multiple graphs over subsets of vectors.
- The original Hnswlib will store materialized vectors in each ``hnswlib::HierarchicalNSW`` class. However, what if multiple HNSW graphs share the same set of vectors (and only involves subsets of them)? This will lead to substantial memory waste since the class will store each vector multiple times. In this version, we change the class to support referring vectors without materialized store.
- The original Hnswlib will maintain 65536 mutex locks in each ``hnswlib::HierarchicalNSW`` class, which incurs substantial memory overhead when you are under single-threaded environments and need to construct multiple HNSW graphs. We change the lock number to 1. If you still want to support multi-threaded environments, revise ``static const tableint MAX_LABEL_OPERATION_LOCKS = 1;`` in ``hnswlib/hnswalg.h`` to the number of threads needed to support.

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