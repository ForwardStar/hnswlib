# Hnswlib (ForwardStar version)

Original Hnswlib repo: [https://github.com/nmslib/hnswlib](https://github.com/nmslib/hnswlib)

Update of this version: the original Hnswlib will store materialized vectors in each ``hnswlib::HierarchicalNSW`` class. However, what if you want to construct multiple HNSW subgraphs over the same set of vectors? This will lead to substantial memory waste since the class will store each vector multiple times. In this version, we change the class to support referring vectors without materialized store.