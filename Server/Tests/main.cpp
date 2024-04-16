#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "FirstMoveFixture.h"


int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}
