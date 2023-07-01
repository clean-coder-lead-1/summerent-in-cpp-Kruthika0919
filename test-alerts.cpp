#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  REQUIRE(inferBreach(12, 20, 30) == TOO_LOW);
  REQUIRE(inferBreach(31, 20, 30) == TOO_HIGH);
  REQUIRE(inferBreach(25, 20, 30) == NORMAL);
}

TEST_CASE("classify Temperature Breach") {
  BatteryCharacter batteryCharacter;
  batteryCharacter.coolingType = PASSIVE_COOLING;
  checkAndAlert(TO_CONTROLLER, batteryCharacter, 5);
  checkAndAlert(TO_EMAIL, batteryCharacter, 5);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 5) == NORMAL);

  checkAndAlert(TO_CONTROLLER, batteryCharacter, -1);
  checkAndAlert(TO_EMAIL, batteryCharacter, -1);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, -1) == TOO_LOW);

  checkAndAlert(TO_CONTROLLER, batteryCharacter, 36);
  checkAndAlert(TO_EMAIL, batteryCharacter, 36);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 36) == TOO_HIGH);
  
  batteryCharacter.coolingType = HI_ACTIVE_COOLING;
  checkAndAlert(TO_CONTROLLER, batteryCharacter, 5);
  checkAndAlert(TO_EMAIL, batteryCharacter, 5);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 5) == NORMAL);
  
  checkAndAlert(TO_CONTROLLER, batteryCharacter, -1);
  checkAndAlert(TO_EMAIL, batteryCharacter, -1);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, -1) == TOO_LOW);

  checkAndAlert(TO_CONTROLLER, batteryCharacter, 46);
  checkAndAlert(TO_EMAIL, batteryCharacter, 46);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46) == TOO_HIGH);

  batteryCharacter.coolingType = MED_ACTIVE_COOLING;
  checkAndAlert(TO_CONTROLLER, batteryCharacter, 5);
  checkAndAlert(TO_EMAIL, batteryCharacter, 5);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 5) == NORMAL);

  checkAndAlert(TO_CONTROLLER, batteryCharacter, -1);
  checkAndAlert(TO_EMAIL, batteryCharacter, -1);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1) == TOO_LOW);

  checkAndAlert(TO_CONTROLLER, batteryCharacter, 41);
  checkAndAlert(TO_EMAIL, batteryCharacter, 41);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41) == TOO_HIGH);

  REQUIRE(classifyTemperatureBreach(static_cast<CoolingType>(-1), 5) == NORMAL);
  checkAndAlert(static_cast<AlertTarget>(-1), batteryCharacter, 41);
}