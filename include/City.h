#pragma once

#include "Agent.h"
#include "Configuration.h"
#include "Location.h"
#include "Town.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <vector>
class City {
public:
  
  std::vector<Town> towns;
  std::vector<Location *> allLocations;
  std::vector<Agent> agents;
  std::mt19937 rng;
  
  City(unsigned int seed = 42) : rng(seed) {}

  void generateTowns() {
    auto &cfg = Configuration::instance();
    towns.clear();
    allLocations.clear();

    for (int i = 0; i < cfg.num_towns; ++i) {
      towns.emplace_back(i, cfg.schools_per_town, cfg.religious_per_town,
                         cfg.workplaces_per_town, cfg.school_capacity,
                         cfg.religious_capacity, cfg.workplace_capacity);

      
      for (auto &school : towns[i].schools) {
        allLocations.push_back(&school);
      }
      for (auto &religious : towns[i].religiousEstablishments) {
        allLocations.push_back(&religious);
      }
      for (auto &work : towns[i].workplaces) {
        allLocations.push_back(&work);
      }
    }
  }

  void generatePopulation(int populationSize) {
    auto &cfg = Configuration::instance();
    agents.clear();
    agents.reserve(populationSize);

    std::uniform_real_distribution<double> uniformDist(0.0, 1.0);
    std::uniform_int_distribution<int> townDist(0, cfg.num_towns - 1);

    for (int i = 0; i < populationSize; ++i) {
      
      int age = generateAge(uniformDist(rng));
      EthnicGroup ethnicity = generateEthnicity(uniformDist(rng));
      int education = generateEducation(age);
      ReligiousDenomination denomination =
          generateDenomination(uniformDist(rng));

      
      int townId = townDist(rng);

      
      int schoolId = -1;
      Location *school = towns[townId].getRandomSchool(rng);
      if (school && school->assignAgent(i)) {
        schoolId = school->id;
      }

      int religiousId = -1;
      if (denomination != ReligiousDenomination::NONE) {
        Location *religious =
            towns[townId].getRandomReligiousOfDenomination(rng, denomination);
        if (religious && religious->assignAgent(i)) {
          religiousId = religious->id;
        }
      }
      int workplaceId = -1;
      if (age >= 18 && education >= 0) {
        if (!towns[townId].workplaces.empty()) {
          int numWork = towns[townId].workplaces.size();
          int workplaceIdx = (education * 2 + (i % 2)) % numWork;
          Location *work = &towns[townId].workplaces[workplaceIdx];
          if (work->assignAgent(i)) {
            workplaceId = work->id;
          }
        }
      }

      agents.emplace_back(i, age, education, townId, schoolId, religiousId,
                          workplaceId, ethnicity, denomination);
    }
  }

  void generateNetwork() {
    auto &cfg = Configuration::instance();
    for (auto &agent : agents) {
      agent.connections.clear();
    }
    for (size_t i = 0; i < agents.size(); ++i) {
      Agent &agent = agents[i];
      std::uniform_real_distribution<double> probDist(0.0, 1.0);

      for (size_t j = i + 1; j < agents.size(); ++j) {
        Agent &other = agents[j];

        
        double prob = agent.getInteractionProbability(other);

        
        if (probDist(rng) < prob) {
          if (static_cast<int>(agent.connections.size()) <
                  cfg.max_connections &&
              static_cast<int>(other.connections.size()) <
                  cfg.max_connections) {
            agent.connections.push_back(other.id);
            other.connections.push_back(agent.id);
          }
        }
      }
    }
  }


  std::vector<int> getAgentsByTown(int townId) const {
    std::vector<int> result;
    for (const auto &agent : agents) {
      if (agent.homeTownId == townId) {
        result.push_back(agent.id);
      }
    }
    return result;
  }

  
  std::vector<int> getAgentsByAgeGroup(AgeGroup group) const {
    std::vector<int> result;
    for (const auto &agent : agents) {
      if (agent.getAgeGroup() == group) {
        result.push_back(agent.id);
      }
    }
    return result;
  }

  
  std::vector<int> getAgentsByEthnicity(EthnicGroup group) const {
    std::vector<int> result;
    for (const auto &agent : agents) {
      if (agent.ethnicity == group) {
        result.push_back(agent.id);
      }
    }
    return result;
  }

  
  Agent &getAgent(int id) { return agents[id]; }

  const Agent &getAgent(int id) const { return agents[id]; }

  
  size_t getPopulationSize() const { return agents.size(); }

  
  
  int findRandomNewConnection(int agentId, int excludeId) {
    auto &cfg = Configuration::instance();
    Agent &agent = agents[agentId];

    
    if (static_cast<int>(agent.connections.size()) >= cfg.max_connections) {
      return -1;
    }

    
    std::vector<int> candidates;
    for (size_t i = 0; i < agents.size(); ++i) {
      int candidateId = static_cast<int>(i);
      if (candidateId == agentId || candidateId == excludeId)
        continue;

      
      bool alreadyConnected = false;
      for (int connId : agent.connections) {
        if (connId == candidateId) {
          alreadyConnected = true;
          break;
        }
      }
      if (alreadyConnected)
        continue;

      
      if (static_cast<int>(agents[candidateId].connections.size()) >=
          cfg.max_connections)
        continue;

      candidates.push_back(candidateId);
    }

    if (candidates.empty())
      return -1;

    
    std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    return candidates[dist(rng)];
  }

private:
  
  
  

  
  int generateAge(double randomValue) {
    double cumulative = 0.0;

    
    cumulative += PHOENIX_AGE_PROBS[0]; 
    if (randomValue < cumulative) {
      std::uniform_int_distribution<int> dist(0, 12);
      return dist(rng);
    }

    cumulative += PHOENIX_AGE_PROBS[1]; 
    if (randomValue < cumulative) {
      std::uniform_int_distribution<int> dist(13, 19);
      return dist(rng);
    }

    cumulative += PHOENIX_AGE_PROBS[2]; 
    if (randomValue < cumulative) {
      std::uniform_int_distribution<int> dist(20, 35);
      return dist(rng);
    }

    cumulative += PHOENIX_AGE_PROBS[3]; 
    if (randomValue < cumulative) {
      std::uniform_int_distribution<int> dist(36, 55);
      return dist(rng);
    }

    
    std::uniform_int_distribution<int> dist(56, 90);
    return dist(rng);
  }

  
  EthnicGroup generateEthnicity(double randomValue) {
    double cumulative = 0.0;

    cumulative += PHOENIX_ETHNICITY_PROBS[0];
    if (randomValue < cumulative)
      return EthnicGroup::WHITE;

    cumulative += PHOENIX_ETHNICITY_PROBS[1];
    if (randomValue < cumulative)
      return EthnicGroup::HISPANIC;

    cumulative += PHOENIX_ETHNICITY_PROBS[2];
    if (randomValue < cumulative)
      return EthnicGroup::BLACK;

    cumulative += PHOENIX_ETHNICITY_PROBS[3];
    if (randomValue < cumulative)
      return EthnicGroup::ASIAN;

    cumulative += PHOENIX_ETHNICITY_PROBS[4];
    if (randomValue < cumulative)
      return EthnicGroup::NATIVE_AMERICAN;

    return EthnicGroup::MULTIRACIAL;
  }

  
  ReligiousDenomination generateDenomination(double randomValue) {
    double cumulative = PHOENIX_RELIGION_PROBS[0];

    
    
    

    if (randomValue < cumulative)
      return ReligiousDenomination::NONE;

    cumulative += PHOENIX_RELIGION_PROBS[1];
    if (randomValue < cumulative)
      return ReligiousDenomination::CATHOLIC;

    cumulative += PHOENIX_RELIGION_PROBS[2];
    if (randomValue < cumulative)
      return ReligiousDenomination::EVANGELICAL;

    cumulative += PHOENIX_RELIGION_PROBS[3];
    if (randomValue < cumulative)
      return ReligiousDenomination::MAINLINE;

    cumulative += PHOENIX_RELIGION_PROBS[4];
    if (randomValue < cumulative)
      return ReligiousDenomination::LDS;

    
    
    double rem = randomValue - cumulative;
    if (rem < 0.005)
      return ReligiousDenomination::JEWISH;
    if (rem < 0.009)
      return ReligiousDenomination::MUSLIM;
    if (rem < 0.013)
      return ReligiousDenomination::BUDDHIST;
    if (rem < 0.017)
      return ReligiousDenomination::HINDU;

    return ReligiousDenomination::NONE; 
  }

  
  int generateEducation(int age) {
    
    

    double mean =
        2.8; 
    double stddev = 1.0;

    if (age < 18) {
      mean = std::min(static_cast<double>(age) / 3.0, mean);
    } else if (age < 22) {
      mean = 2.0; 
    }

    std::normal_distribution<double> dist(mean, stddev);
    int edu = static_cast<int>(std::round(dist(rng)));
    return std::max(0, std::min(5, edu));
  }
};
