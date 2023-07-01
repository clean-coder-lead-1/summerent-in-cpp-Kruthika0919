#include "typewise-alert.h"
#include <stdio.h>
#include <map>
#include <iostream>
#include <iterator>


BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

std::map<CoolingType, std::pair<int,int>> coolingTypeLimits = {
{CoolingType::PASSIVE_COOLING, std::make_pair(0,35)},
{CoolingType::HI_ACTIVE_COOLING, std::make_pair(0,45)},
{CoolingType::MED_ACTIVE_COOLING, std::make_pair(0,40)}
};

BreachType classifyTemperatureBreach(
    CoolingType coolingType, double temperatureInC) {
  int lowerLimit = 0;
  int upperLimit = 0;
  std::map<CoolingType, std::pair<int,int>>::iterator res;
  res = coolingTypeLimits.find(coolingType);
  if(res != coolingTypeLimits.end())
  {
    lowerLimit = res->second.first;
    upperLimit = res->second.second;
  }
  return inferBreach(temperatureInC, lowerLimit, upperLimit);
}

typedef void (*targetFunc)(BreachType);
std::map<AlertTarget, targetFunc> alertTargets = {
{AlertTarget::TO_CONTROLLER, &sendToController},
{AlertTarget::TO_EMAIL, &sendToEmail}
};
void checkAndAlert(
    AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

  BreachType breachType = classifyTemperatureBreach(
    batteryChar.coolingType, temperatureInC
  );

  std::map<AlertTarget, targetFunc>::iterator res;
  res = alertTargets.find(alertTarget);
  if(res != alertTargets.end())
  {
    (res->second)(breachType);
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

std::map<BreachType, std::string> breachTypeStatus = {
{BreachType::TOO_LOW, "low"},
{BreachType::TOO_HIGH, "high"}
};

void sendToEmail(BreachType breachType) {
  if(breachType != BreachType::NORMAL)
  {
    const char* recepient = "a.b@c.com";
    std::map<BreachType, std::string>::iterator res;
    res = breachTypeStatus.find(breachType);
    if (res != breachTypeStatus.end())
    {
      printf("To: %s\n", recepient);
      printf("Hi, the temperature is too %s\n", res->second.c_str());
    }
  }
}
