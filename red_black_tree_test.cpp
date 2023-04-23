

#include <gtest/gtest.h>
#include "tree_map.h"

using namespace collections;

#define N 10


class MapTest : public ::testing::Test {

protected:
    void SetUp() override {
        map = new TreeMap<int, int>();
        for (int i = 0; i < N; i++) {
            map->put(i, i);
        }
    }

    void TearDown() override {
        delete map;
        map = nullptr;
    }

    Map<int, int> *map{};
};


TEST_F(MapTest, InsertTest) {

    for (int i = 0; i < N; i++) {
        EXPECT_EQ(map->containsKey(i), true);
        EXPECT_EQ((*map)[i], i);
    }
    EXPECT_EQ(map->size(), N);
}

TEST_F(MapTest, InsertSameTest) {

    for (int i = 0; i < N; i++) {
        auto old = map->put(i, i);
        EXPECT_EQ(old, i);
    }

    for (int i = 0; i < N; i++) {
        EXPECT_EQ(map->containsKey(i), true);
        EXPECT_EQ((*map)[i], i);
    }

    EXPECT_EQ(map->size(), N);
}

TEST_F(MapTest, InsertOtherTest) {

    for (int i = 0; i < N; i++) {
        auto old = map->put(i, N - i - 1);
        EXPECT_EQ(old, i);
    }

    for (int i = 0; i < N; i++) {
        EXPECT_EQ(map->containsKey(i), true);
        EXPECT_EQ((*map)[i], N - i - 1);
    }

    EXPECT_EQ(map->size(), N);
}


TEST_F(MapTest, ContainsTest) {

    for (int i = 0; i < N; i++) {
        EXPECT_EQ(map->containsKey(i), true);
    }
}

TEST_F(MapTest, GetTest) {

    for (int i = 0; i < N; i++) {
        EXPECT_EQ((*map)[i], i);
    }
}

TEST_F(MapTest, SizeTest) {

    EXPECT_EQ(map->size(), N);
}

TEST_F(MapTest, EmptyTest) {

    EXPECT_EQ(map->isEmpty(), false);

}

TEST_F(MapTest, EmptyNewTest) {
    TreeMap<int, int> emptyMap;

    EXPECT_EQ(emptyMap.isEmpty(), true);

}


TEST_F(MapTest, ClearTest) {

    map->clear();

    EXPECT_EQ(map->size(), 0);
    EXPECT_EQ(map->isEmpty(), true);
}

TEST_F(MapTest, DeleteTest) {

    for (int i = 0; i < N; i++) {
        map->remove(i);
    }

    for (int i = 0; i < N; i++) {
        EXPECT_EQ(map->containsKey(i), false);
    }

    EXPECT_EQ(map->size(), 0);
    EXPECT_EQ(map->isEmpty(), true);
}

TEST_F(MapTest, PartDeleteTest) {

    for (int i = 0; i < N; i++) {
        if (i % 2 == 0) {
            map->remove(i);
        }
    }

    for (int i = 0; i < N; i++) {
        if (i % 2 != 0) {
            EXPECT_EQ((*map)[i], i);
        }
        EXPECT_EQ(map->containsKey(i), i % 2 != 0);
    }

    EXPECT_EQ(map->size(), N / 2);
    EXPECT_EQ(map->isEmpty(), false);
}


TEST_F(MapTest, PartDeleteThenInsertTest) {

    for (int i = 0; i < N; i++) {
        if (i % 2 == 0) {
            map->remove(i);
        }
    }

    for (int i = 0; i < N; i++) {
        if (i % 2 != 0) {
            EXPECT_EQ((*map)[i], i);
        }
        EXPECT_EQ(map->containsKey(i), i % 2 != 0);
    }

    EXPECT_EQ(map->size(), N / 2);
    EXPECT_EQ(map->isEmpty(), false);

    for (int i = 0; i < N; i += 2) {
        map->put(i, i);
    }

    for (int i = 0; i < N; i++) {

        EXPECT_EQ(map->containsKey(i), true);
    }

    EXPECT_EQ(map->size(), N);
    EXPECT_EQ(map->isEmpty(), false);
}
