#include "../include/Simulation.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
int main(int argc, char *argv[]) {
  auto &cfg = Configuration::instance();
  cfg.load("parameters.cfg");
  std::cout << "=================================================="
            << std::endl;
  std::cout << "SEDPNR Agent-Based Misinformation Simulation" << std::endl;
  std::cout << "=================================================="
            << std::endl;  
  if (argc >= 2) {
    cfg.population = std::stoi(argv[1]);
  }
  if (argc >= 3) {
    cfg.timesteps = std::stoi(argv[2]);
  }
  std::cout << "\nConfiguration:" << std::endl;
  std::cout << "  Towns:       " << cfg.num_towns << std::endl;
  std::cout << "  Population:  " << cfg.population << std::endl;
  std::cout << "  Time steps:  " << cfg.timesteps << std::endl;
  std::cout << "  Random seed: " << cfg.seed << std::endl;
  Simulation sim(cfg.seed);
  std::cout << "\nInitializing city and population..." << std::endl;
  sim.initialize(cfg.population);
  std::cout << "City generated with " << sim.city.getPopulationSize()
            << " agents" << std::endl;
  std::cout << "\nAdding claims..." << std::endl;  
  Claim truth = Claim::createTruth(0, "Factual_Claim");
  sim.addClaim(truth, 455); 
  std::cout << "  Added: " << truth.name
            << " (Truth) with 10 initial propagators" << std::endl;  
  Claim misinfo1 = Claim::createMisinformation(1, "Misinfo_Claim_1");
  sim.addClaimPerDistrict(misinfo1, 45);
  std::cout << "  Added: " << misinfo1.name
            << " (Misinformation) with 5 propagators per district" << std::endl;  
  std::cout << "\nRunning controllable simulation..." << std::endl;
  std::cout
      << "Controls: [Enter] to step, [R] to run continuously, [P] to pause"
      << std::endl;
  bool continuous = true; 
  for (int t = 0; t < cfg.timesteps; ++t) {
    sim.step();   
    if (t % 20 == 0) {
      std::cout << "\n"
                << std::setw(6) << "Step" << " | " << std::setw(15) << "Claim"
                << " | " << std::setw(4) << "S" << " | " << std::setw(4) << "E"
                << " | " << std::setw(4) << "D" << " | " << std::setw(4) << "P"
                << " | " << std::setw(4) << "N" << " | " << std::setw(4) << "R"
                << std::endl;
      std::cout << std::string(65, '-') << std::endl;
    }    
    for (const auto &claim : sim.claims) {
      StateCounts sc = sim.getLatestStateCounts(claim.claimId);
      std::cout << std::setw(6) << t << " | " << std::setw(15)
                << claim.name.substr(0, 15) << " | " << std::setw(4)
                << sc.susceptible << " | " << std::setw(4) << sc.exposed
                << " | " << std::setw(4) << sc.doubtful << " | " << std::setw(4)
                << sc.propagating << " | " << std::setw(4) << sc.notSpreading
                << " | " << std::setw(4) << sc.recovered << std::endl;
    }
    if (!continuous) {
      std::string input;
      std::getline(std::cin, input);
      if (input == "r" || input == "R") {
        continuous = true;
      }
    } else {}
  }  
  std::cout << "\nWriting results..." << std::endl;
  sim.outputResults("output/simulation_results.csv");  
  sim.outputSummary();

  std::cout << "\n=================================================="
            << std::endl;
  std::cout << "Simulation complete!" << std::endl;
  std::cout << "=================================================="
            << std::endl;
  return 0;
}
