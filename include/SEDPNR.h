#pragma once
enum class SEDPNRState {
  SUSCEPTIBLE = 0, 
  EXPOSED,         
  DOUBTFUL,        
  PROPAGATING,     
  NOT_SPREADING,   
  RECOVERED,       
  NUM_STATES
};
inline const char *stateToString(SEDPNRState state) {
  switch (state) {
  case SEDPNRState::SUSCEPTIBLE:
    return "Susceptible";
  case SEDPNRState::EXPOSED:
    return "Exposed";
  case SEDPNRState::DOUBTFUL:
    return "Doubtful";
  case SEDPNRState::PROPAGATING:
    return "Propagating";
  case SEDPNRState::NOT_SPREADING:
    return "Not-Spreading";
  case SEDPNRState::RECOVERED:
    return "Recovered";
  default:
    return "Unknown";
  }
}

inline char stateToChar(SEDPNRState state) {
  switch (state) {
  case SEDPNRState::SUSCEPTIBLE:
    return 'S';
  case SEDPNRState::EXPOSED:
    return 'E';
  case SEDPNRState::DOUBTFUL:
    return 'D';
  case SEDPNRState::PROPAGATING:
    return 'P';
  case SEDPNRState::NOT_SPREADING:
    return 'N';
  case SEDPNRState::RECOVERED:
    return 'R';
  default:
    return '?';
  }
}
