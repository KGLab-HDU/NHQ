
#include <vector>
#include "gtest/gtest.h"

#include "n2/hnsw.h"
#include "n2/distance.h"
#include "n2/min_heap.h"

class CppApiTest : public::testing::Test {
    protected:
        virtual void SetUp() {}
        virtual void TearDown() {}
};

TEST_F(CppApiTest, SearchByVectorTest) {
    n2::Hnsw index(3, "angular");
    index.AddData(std::vector<float>{0, 0, 1});
    index.AddData(std::vector<float>{0, 1, 0});
    index.AddData(std::vector<float>{0, 0, 1});
    index.Build(5, 10);
    std::vector<std::pair<int, float> > result;
    int ef_search = 3*10;
    index.SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result);    
    EXPECT_EQ(3, result.size());
    n2::Hnsw index2(3, "angular");
    index2.AddData(std::vector<float>{0, 0, 1});
    index2.AddData(std::vector<float>{0, 1, 0});
    index2.AddData(std::vector<float>{0, 0, 1});
    index2.Build(5, 10);
    std::vector<std::pair<int, float> > result2;
    index2.SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result2);
    EXPECT_EQ(3, result2.size());    
}

TEST_F(CppApiTest, SearchByIdTest) {
    n2::Hnsw index(3);
    index.AddData(std::vector<float>{2, 1, 0});
    index.AddData(std::vector<float>{1, 2, 0});
    index.AddData(std::vector<float>{0, 0, 1});
    index.Build(5, 10, 150, 1, 0, n2::NeighborSelectingPolicy::HEURISTIC, n2::GraphPostProcessing::SKIP);
    std::vector<std::pair<int, float> > result;
    int ef_search = 3*10;
    index.SearchById(0, 3, ef_search, result);
    EXPECT_EQ(3, result.size());
    EXPECT_EQ(0, result[0].first);
    EXPECT_EQ(1, result[1].first);
    EXPECT_EQ(2, result[2].first);
    result.clear();
    index.SearchById(1, 3, ef_search, result);
    EXPECT_EQ(3, result.size());
    EXPECT_EQ(0, result[1].first);
    EXPECT_EQ(1, result[0].first);
    EXPECT_EQ(2, result[2].first);
    result.clear();
}

TEST_F(CppApiTest, CopyOperatorTest) {
    n2::Hnsw* origin = new n2::Hnsw(3, "angular");
    origin->AddData(std::vector<float>{0, 0, 1});
    origin->AddData(std::vector<float>{0, 1, 0});
    origin->AddData(std::vector<float>{0, 0, 1});
    origin->Fit();
    std::vector<std::pair<int, float> > result;
    int ef_search = 3*10;
    origin->SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result);
    EXPECT_EQ(3, result.size());
    n2::Hnsw* copy = new n2::Hnsw(*origin);
    delete origin;
    std::vector<std::pair<int, float> > result2;
    copy->SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result2);
    EXPECT_EQ(3, result2.size());
    delete copy;
}

TEST_F(CppApiTest, AssignOperatorTest) {
    n2::Hnsw origin(3, "angular");
    origin.AddData(std::vector<float>{0, 0, 1});
    origin.AddData(std::vector<float>{0, 1, 0});
    origin.AddData(std::vector<float>{0, 0, 1});
    origin.Fit();
    std::vector<std::pair<int, float> > result;
    int ef_search = 3*10;
    origin.SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result);
    EXPECT_EQ(3, result.size());
    n2::Hnsw copy(3, "angular");
    copy = origin;
    std::vector<std::pair<int, float> > result2;
    copy.SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result2);
    EXPECT_EQ(3, result2.size());
}

TEST_F(CppApiTest, MoveAssignOperatorTest) {
    n2::Hnsw origin(3, "angular");
    origin.AddData(std::vector<float>{0, 0, 1});
    origin.AddData(std::vector<float>{0, 1, 0});
    origin.AddData(std::vector<float>{0, 0, 1});
    origin.Fit();
    std::vector<std::pair<int, float> > result;
    int ef_search = 3*10;
    origin.SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result);
    EXPECT_EQ(3, result.size());
    n2::Hnsw copy(3, "angular");
    copy = std::move(origin);
    std::vector<std::pair<int, float> > result2;
    copy.SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result2);
    EXPECT_EQ(3, result2.size());
}

TEST_F(CppApiTest, MoveOperatorTest) {
    n2::Hnsw* origin = new n2::Hnsw(3, "angular");
    origin->AddData(std::vector<float>{0, 0, 1});
    origin->AddData(std::vector<float>{0, 1, 0});
    origin->AddData(std::vector<float>{0, 0, 1});
    origin->Fit();
    std::vector<std::pair<int, float> > result;
    int ef_search = 3*10;
    origin->SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result);
    EXPECT_EQ(3, result.size());
    n2::Hnsw* copy = new n2::Hnsw(std::move(*origin));
    std::vector<std::pair<int, float> > result2;
    copy->SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result2);
    EXPECT_EQ(3, result2.size());
    delete copy;
    delete origin;
}

TEST_F(CppApiTest, LoadModelTest) {
    n2::Hnsw* origin = new n2::Hnsw();
    origin->LoadModel("../model/test.n2", false);
    std::vector<std::pair<int, float> > result;
    int ef_search = 3*10;
    origin->SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result);
    EXPECT_EQ(3, result.size());
    delete origin;
}

n2::Hnsw load_n2_model(std::string path, bool use_mmap) {
    auto n2 = n2::Hnsw();
    n2.LoadModel(path, use_mmap);
    return std::move(n2);
}

TEST_F(CppApiTest, LoadModelMoveTestWithMmap) {
    auto origin = std::move(load_n2_model("../model/test.n2", true));
    n2::Hnsw other = std::move(origin);
    int ef_search = 3*10;
  
    for(size_t i = 0; i < 100000; i++ ) { 
        std::vector<std::pair<int, float> > result; 
        other.SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result);
        EXPECT_EQ(3, result.size());
    }
}

TEST_F(CppApiTest, LoadModelMoveTestWithInMemory) {
    auto origin = std::move(load_n2_model("../model/test.n2", false));
    n2::Hnsw other = std::move(origin);
    int ef_search = 3*10;

    for(size_t i = 0; i < 100000; i++ ) {
        std::vector<std::pair<int, float> > result;
        other.SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result);
        EXPECT_EQ(3, result.size());
    }
}

TEST_F(CppApiTest, LoadModelMoveTestAndUnloadWithInMmap) {
    auto origin = std::move(load_n2_model("../model/test.n2", true));
    n2::Hnsw other = n2::Hnsw();
    other  = std::move(origin);
    int ef_search = 3*10;

    std::vector<std::pair<int, float> > result;
    other.SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result);
    EXPECT_EQ(3, result.size());
    origin.UnloadModel();
}

TEST_F(CppApiTest, LoadModelMoveTestAndUnloadWithInMemory) {
    auto origin = std::move(load_n2_model("../model/test.n2", false));
    n2::Hnsw other = std::move(origin);
    int ef_search = 3*10;

    std::vector<std::pair<int, float> > result;
    other.SearchByVector(std::vector<float>{3, 2, 1}, 3, ef_search, result);
    EXPECT_EQ(3, result.size());
    origin.UnloadModel();
}

TEST_F(CppApiTest, UnloadMmapModelTest) {
    n2::Hnsw* origin = new n2::Hnsw(3);
    origin->LoadModel("../model/test.n2", true);
    origin->UnloadModel();
    delete origin;
}

TEST_F(CppApiTest, UnloadModelTest) {
    n2::Hnsw* origin = new n2::Hnsw(3);
    origin->LoadModel("../model/test.n2", false);
    origin->UnloadModel();
    delete origin;
}

TEST_F(CppApiTest, L2DistanceTest) {
    n2::L2Distance dist_func;
    float vec1[] = {0.0, 0.0, 0.0};
    float vec2[] = {1.0, 0.0, 1.0};
    float vec3[] = {0.0, 0.75, 1.0};

    float res1 = dist_func(vec1, vec2, 3);
    EXPECT_FLOAT_EQ(2, res1);
    float res2 = dist_func(vec2, vec3, 3);
    EXPECT_FLOAT_EQ(1.5625, res2);
}

TEST_F(CppApiTest, AngularDistanceTest) {
    n2::AngularDistance dist_func;
    float vec1[] = {0.1, 0.2, 0.3, 0.4};
    float vec2[] = {0.5, 0.6, 0.7, 0.8};
    float res1 = dist_func(vec1, vec2, 4);
    EXPECT_FLOAT_EQ(0.3, res1);
}

TEST_F(CppApiTest, DotDistanceTest) {
    n2::DotDistance dist_func;
    float vec1[] = {1.0, 0.0, 0.0};
    float vec2[] = {1.0, 1.0, 1.0};
    float vec3[] = {0.0, 1.0, 2.0};
    float res1 = dist_func(vec1, vec2, 3);
    EXPECT_FLOAT_EQ(-1, res1);
    float res2 = dist_func(vec2, vec3, 3);
    EXPECT_FLOAT_EQ(-3, res2);
}

TEST_F(CppApiTest, MinHeapTest) {
    n2::MinHeap<int, float>* minheap = new n2::MinHeap<int, float>();
    minheap->push(3, 3.5);
    minheap->push(2, 7.5);
    minheap->push(1, 2.4);
    EXPECT_EQ(minheap->size(), 3);
    auto item = minheap->top(); minheap->pop();
    EXPECT_FLOAT_EQ(item.data, 2.4);
    minheap->pop();
    minheap->pop();
    EXPECT_EQ(minheap->size(), 0);
    delete minheap;
}

TEST_F(CppApiTest, DimensionErrorTest) {
    n2::Hnsw* origin = new n2::Hnsw(0, "angular");
    EXPECT_THROW(origin->AddData(std::vector<float>{0, 0, 1}),
    std::runtime_error);
    EXPECT_NO_THROW(origin->AddData(std::vector<float>{}));
    delete origin;
}

TEST_F(CppApiTest, LoadUnknownModelTest) {
    n2::Hnsw* origin = new n2::Hnsw(0);
    origin->LoadModel("../model/test.n2", false);
    EXPECT_THROW(origin->AddData(std::vector<float>{0, 0, 1}),
    std::runtime_error);
    EXPECT_THROW(origin->AddData(std::vector<float>{}),
    std::runtime_error);
    delete origin;
}
