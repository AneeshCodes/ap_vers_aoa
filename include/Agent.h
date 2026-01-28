#pragma once
#include "Demographics.h"
#include "SEDPNR.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <random>
#include <vector>

#include "Configuration.h"
#include "SEDPNR.h"









class Agent {
public:
  
  int id;

  
  int age;
  int educationLevel; 
  int homeTownId;
  int schoolLocationId; 
  int religiousLocationId;
  int workplaceLocationId; 
  EthnicGroup ethnicity;
  ReligiousDenomination denomination;

  
  double credibilityValue; 

  
  std::vector<int> connections; 

  
  std::map<int, SEDPNRState> claimStates;

  
  std::map<int, int> timeInState;

  
  
  std::map<int, int> connectionTenure;

  
  Agent(int agentId, int agentAge, int eduLevel, int town, int school,
        int religious, int work, EthnicGroup ethnic,
        ReligiousDenomination denom)
      : id(agentId), age(agentAge), educationLevel(eduLevel), homeTownId(town),
        schoolLocationId(school), religiousLocationId(religious),
        workplaceLocationId(work), ethnicity(ethnic), denomination(denom) {
    credibilityValue = calculateCredibility();
  }

  
  Agent()
      : id(-1), age(0), educationLevel(0), homeTownId(-1), schoolLocationId(-1),
        religiousLocationId(-1), workplaceLocationId(-1),
        ethnicity(EthnicGroup::WHITE),
        denomination(ReligiousDenomination::NONE), credibilityValue(0) {}

  
  
  
  
  
  double calculateCredibility() const {
    
    double eduNormalized = static_cast<double>(educationLevel) / 5.0;

    
    auto &cfg = Configuration::instance();
    double ageFactor = 0.0;
    if (cfg.age_spread > 0) {
      ageFactor = std::exp(-std::pow(age - cfg.age_optimal, 2) /
                           (2 * cfg.age_spread * cfg.age_spread));
    }

    
    double credibility =
        cfg.age_weight * ageFactor + cfg.edu_weight * eduNormalized;

    
    return std::max(0.0, std::min(1.0, credibility));
  }

  
  
  double calculateSimilarity(const Agent &other) const {
    double score = 1.0; 

    
    if (ethnicity == other.ethnicity)
      score += 0.2; 

    
    if (denomination == other.denomination)
      score += 0.2; 

    
    if (std::abs(age - other.age) <= 10)
      score += 0.1; 

    
    if (std::abs(educationLevel - other.educationLevel) <= 1)
      score += 0.1; 

    return score;
  }

  
  
  
  AgeGroup getAgeGroup() const {
    if (age <= 12)
      return AgeGroup::CHILD;
    if (age <= 19)
      return AgeGroup::TEEN;
    if (age <= 35)
      return AgeGroup::YOUNG_ADULT;
    if (age <= 55)
      return AgeGroup::ADULT;
    return AgeGroup::SENIOR;
  }

  
  
  
  
  
  
  
  double getInteractionProbability(const Agent &other) const {
    auto &cfg = Configuration::instance();
    double prob = cfg.base_interaction_prob;

    
    
    if (schoolLocationId != -1 && schoolLocationId == other.schoolLocationId) {
      prob += cfg.same_school_weight;
    }

    
    if (religiousLocationId == other.religiousLocationId) {
      prob += cfg.same_religious_weight;
    }

    
    if (workplaceLocationId != -1 &&
        workplaceLocationId == other.workplaceLocationId) {
      prob += cfg.same_workplace_weight;
    }

    
    if (homeTownId == other.homeTownId) {
      prob += cfg.same_town_weight;
    }

    
    if (getAgeGroup() == other.getAgeGroup()) {
      prob += cfg.age_group_weight;
    }

    
    if (ethnicity == other.ethnicity) {
      prob += cfg.ethnicity_weight;
    }

    
    return std::max(0.0, std::min(1.0, prob));
  }

  
  
  
  
  
  double getClaimPassingFrequency() const {
    
    
    double baseFrq = 1.0; 

    switch (getAgeGroup()) {
    case AgeGroup::CHILD:
      baseFrq *= 0.5; 
      break;
    case AgeGroup::TEEN:
      baseFrq *= 1.5; 
      break;
    case AgeGroup::YOUNG_ADULT:
      baseFrq *= 1.2; 
      break;
    case AgeGroup::ADULT:
      baseFrq *= 1.0; 
      break;
    case AgeGroup::SENIOR:
      baseFrq *= 0.8; 
      break;
    default:
      break;
    }

    return baseFrq;
  }

  
  
  

  
  bool isInvolved() const {
    for (auto const &entry : claimStates) {
      SEDPNRState state = entry.second;
      if (state != SEDPNRState::SUSCEPTIBLE) {
        return true;
      }
    }
    return false;
  }

  
  SEDPNRState getState(int claimId) const {
    auto it = claimStates.find(claimId);
    if (it != claimStates.end()) {
      return it->second;
    }
    return SEDPNRState::SUSCEPTIBLE; 
  }

  
  void setState(int claimId, SEDPNRState state) {
    if (claimStates.find(claimId) == claimStates.end() ||
        claimStates[claimId] != state) {
      timeInState[claimId] = 0; 
    }
    claimStates[claimId] = state;
  }

  
  void incrementTimeInState(int claimId) { timeInState[claimId]++; }

  
  int getTimeInState(int claimId) const {
    auto it = timeInState.find(claimId);
    if (it != timeInState.end()) {
      return it->second;
    }
    return 0;
  }

  
  
  

  void removeConnection(int connId) {
    connections.erase(
        std::remove(connections.begin(), connections.end(), connId),
        connections.end());
    connectionTenure.erase(connId);
  }

  void addConnection(int connId) {
    
    if (std::find(connections.begin(), connections.end(), connId) ==
        connections.end()) {
      connections.push_back(connId);
    }
  }

  void resetConnectionTenure() { connectionTenure.clear(); }

  void incrementConnectionTenure(int connId) { connectionTenure[connId]++; }

  int getConnectionTenure(int connId) const {
    auto it = connectionTenure.find(connId);
    if (it != connectionTenure.end()) {
      return it->second;
    }
    return 0;
  }
};
