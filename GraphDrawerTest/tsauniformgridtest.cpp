#include "gtest/gtest.h"

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

class TSAUniformGridTest : public ::testing::Test {
protected:
    TSAUniformGridTest() {

    }

    virtual ~TSAUniformGridTest() {

    }

    virtual void SetUp() {

    }

    virtual void TearDown() {

    }

private:
    ogdf::Graph* G;
    ogdf::GraphAttributes* GA;
};

TEST_F(TSAUniformGridTest, GridCorrectness) {

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
