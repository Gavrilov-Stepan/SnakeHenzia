#include "main.cpp"
#include <doctest.h>

bool gameover = true;
int score = 10;

TEST_SUITE_BEGIN("file main");

TEST_CASE("InfoChecks") {
Setup();
REQUIRE(score == 0);
REQUIRE(gameover == true);
REQUIRE(main() == 0);
}

TEST_SUITE_END();
