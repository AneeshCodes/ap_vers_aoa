#pragma once
#include "Demographics.h"
#include "Location.h"
#include <random>
#include <string>
#include <vector>

struct Town {
  int id;
  std::string name;
  std::vector<Location> schools;
  std::vector<Location> religiousEstablishments;
  std::vector<Location> workplaces;

  Town() : id(-1) {}

  Town(int townId, int numSchools, int numReligious, int numWorkplaces,
       int schoolCap, int religiousCap, int workplaceCap)
      : id(townId) {
    name = "Town_" + std::to_string(townId);

    
    for (int i = 0; i < numSchools; ++i) {
      int locId = townId * 1000 + i; 
      std::string locName = name + "_School_" + std::to_string(i);
      schools.emplace_back(locId, LocationType::SCHOOL, townId, locName,
                           schoolCap);
    }

    
    
    int numReligiousDenoms =
        static_cast<int>(ReligiousDenomination::NUM_DENOMINATIONS) - 1;
    for (int i = 0; i < numReligious; ++i) {
      int locId = townId * 1000 + 100 + i;

      ReligiousDenomination denom;
      if (i < numReligiousDenoms) {
        
        denom = static_cast<ReligiousDenomination>(i + 1);
      } else {
        
        denom =
            static_cast<ReligiousDenomination>((i % numReligiousDenoms) + 1);
      }

      std::string denomName;
      switch (denom) {
      case ReligiousDenomination::CATHOLIC:
        denomName = "Catholic";
        break;
      case ReligiousDenomination::EVANGELICAL:
        denomName = "Evangelical";
        break;
      case ReligiousDenomination::MAINLINE:
        denomName = "Mainline";
        break;
      case ReligiousDenomination::LDS:
        denomName = "LDS";
        break;
      case ReligiousDenomination::JEWISH:
        denomName = "Jewish";
        break;
      case ReligiousDenomination::MUSLIM:
        denomName = "Muslim";
        break;
      case ReligiousDenomination::BUDDHIST:
        denomName = "Buddhist";
        break;
      case ReligiousDenomination::HINDU:
        denomName = "Hindu";
        break;
      default:
        denomName = "Other";
        break;
      }

      std::string locName = name + "_" + denomName + "_" + std::to_string(i);
      religiousEstablishments.emplace_back(
          locId, LocationType::RELIGIOUS_ESTABLISHMENT, townId, locName,
          religiousCap, denom);
    }

    
    for (int i = 0; i < numWorkplaces; ++i) {
      int locId = townId * 1000 + 500 + i;
      std::string locName = name + "_Work_" + std::to_string(i);
      workplaces.emplace_back(locId, LocationType::WORKPLACE, townId, locName,
                              workplaceCap);
    }
  }

  
  Location *getRandomSchool(std::mt19937 &rng) {
    if (schools.empty())
      return nullptr;
    std::uniform_int_distribution<size_t> dist(0, schools.size() - 1);
    return &schools[dist(rng)];
  }

  
  Location *getRandomReligious(std::mt19937 &rng) {
    if (religiousEstablishments.empty())
      return nullptr;
    std::uniform_int_distribution<size_t> dist(
        0, religiousEstablishments.size() - 1);
    return &religiousEstablishments[dist(rng)];
  }

  
  Location *getRandomReligiousOfDenomination(std::mt19937 &rng,
                                             ReligiousDenomination denom) {
    std::vector<Location *> matches;
    for (auto &loc : religiousEstablishments) {
      if (loc.denomination == denom) {
        matches.push_back(&loc);
      }
    }
    if (matches.empty())
      return nullptr;
    std::uniform_int_distribution<size_t> dist(0, matches.size() - 1);
    return matches[dist(rng)];
  }

  
  Location *getRandomWorkplace(std::mt19937 &rng) {
    if (workplaces.empty())
      return nullptr;
    std::uniform_int_distribution<size_t> dist(0, workplaces.size() - 1);
    return &workplaces[dist(rng)];
  }
};
