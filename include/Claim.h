#pragma once

#include <string>

#include "Configuration.h"

class Claim {
public:
  
  int claimId;

  
  bool isMisinformation;

  
  double spreadRate;        
  double adoptionThreshold; 

  
  std::string name;

  
  int originAgentId; 
  int originTime;    

  static Claim createMisinformation(int id, const std::string &name = "") {
    auto &cfg = Configuration::instance();
    Claim c;
    c.claimId = id;
    c.isMisinformation = true;
    c.spreadRate = 1.0; 
    c.adoptionThreshold = cfg.misinfo_threshold;
    c.name = name.empty() ? "Misinformation_" + std::to_string(id) : name;
    c.originAgentId = -1;
    c.originTime = 0;
    return c;
  }

  static Claim createTruth(int id, const std::string &name = "") {
    auto &cfg = Configuration::instance();
    Claim c;
    c.claimId = id;
    c.isMisinformation = false;
    c.spreadRate = 1.0; 
    c.adoptionThreshold = cfg.truth_threshold;
    c.name = name.empty() ? "Truth_" + std::to_string(id) : name;
    c.originAgentId = -1;
    c.originTime = 0;
    return c;
  }

  
  Claim()
      : claimId(-1), isMisinformation(false), spreadRate(1.0),
        adoptionThreshold(0.5), originAgentId(-1), originTime(0) {}

  
  std::string getTypeString() const {
    return isMisinformation ? "Misinformation" : "Truth";
  }
};
