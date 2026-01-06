#include "../../hnswlib/hnswlib.h"


int main() {
    int dim = 16;               // Dimension of the elements
    int max_elements = 10000;   // Maximum number of elements, should be known beforehand
    int M = 16;                 // Tightly connected with internal dimensionality of the data
                                // strongly affects the memory consumption
    int ef_construction = 200;  // Controls index search speed/build speed tradeoff

    // Initing index
    hnswlib::L2Space space(dim);
    float* data = new float[dim * max_elements];
    hnswlib::HierarchicalNSW<float>* alg_hnsw = new hnswlib::HierarchicalNSW<float>(&space, max_elements, data, M, ef_construction);

    // Generate random data
    std::mt19937 rng;
    rng.seed(47);
    std::uniform_real_distribution<> distrib_real;
    for (int i = 0; i < dim * max_elements; i++) {
        data[i] = distrib_real(rng);
    }

    // Add data to index
    std::vector<int> added_points;
    for (int i = 0; i < 100; i++) {
        added_points.emplace_back(rand() % max_elements);
        alg_hnsw->addPoint(added_points.back());
    }
    
    for (int i = 0; i < 100; i++) {
        if ((float*)(alg_hnsw->getDataByLabel(added_points[i])) != data + added_points[i] * dim) {
            std::cout << "getDataByLabel correctness test failed!" << std::endl;
            std::cout << "Debug info: " << std::endl;
            std::cout << "-- Trying to get the " << added_points[i] << "-th vector;" << std::endl;
            std::cout << "-- Internal ID of the vector should be " << i << ";" << std::endl;
            std::cout << "-- Label of internal ID " << i << " is: " << alg_hnsw->getExternalLabel(i) << ", should be: " << added_points[i] << std::endl;
            std::cout << "-- Pointer of the vector in HNSW is: " << (float*)alg_hnsw->getDataByLabel(added_points[i]) << ", should be: " << data + added_points[i] * dim << std::endl;
            delete[] data;
            delete alg_hnsw;
            return 0;
        }
    }

    delete[] data;
    delete alg_hnsw;

    std::cout << "getDataByLabel correctness test passed!" << std::endl;
    return 0;
}
