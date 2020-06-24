#include "gtest/gtest.h"

#include "main/token.h"

TEST(myGrpc_test,token)
{
  EXPECT_NE(buildToken(),"123456");
}
