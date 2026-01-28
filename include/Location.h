#pragma once
#include "Demographics.h"
#include <string>
#include <vector>

enum class LocationType {
  SCHOOL,
  RELIGIOUS_ESTABLISHMENT,
  WORKPLACE,
  HOME 
};

struct Location {
  int id;
  LocationType type;
  int townId;
  std::string name;
  int capacity;                       
  ReligiousDenomination denomination; 
  std::vector<int> assignedAgents;    

  Location()
      : id(-1), type(LocationType::HOME), townId(-1), capacity(0),
        denomination(ReligiousDenomination::NONE) {}

  Location(int locId, LocationType locType, int town,
           const std::string &locName, int cap,
           ReligiousDenomination denom = ReligiousDenomination::NONE)
      : id(locId), type(locType), townId(town), name(locName), capacity(cap),
        denomination(denom) {}

  
  bool assignAgent(int agentId) {
    if (static_cast<int>(assignedAgents.size()) >= capacity) {
      return false; 
    }
    assignedAgents.push_back(agentId);
    return true;
  }  
  std::string getTypeString() const {
    switch (type) {
    case LocationType::SCHOOL:
      return "School";
    case LocationType::RELIGIOUS_ESTABLISHMENT:
      return "Religious";
    case LocationType::WORKPLACE:
      return "Workplace";
    case LocationType::HOME:
      return "Home";
    default:
      return "Unknown";
    }
  }
};
