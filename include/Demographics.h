#pragma once

enum class EthnicGroup {
  WHITE = 0,
  HISPANIC,
  BLACK,
  ASIAN,
  NATIVE_AMERICAN,
  MULTIRACIAL,
  NUM_GROUPS
};

enum class AgeGroup {
  CHILD = 0,   
  TEEN,        
  YOUNG_ADULT, 
  ADULT,       
  SENIOR,      
  NUM_GROUPS
};

enum class ReligiousDenomination {
  NONE = 0,
  CATHOLIC,
  EVANGELICAL,
  MAINLINE,
  LDS,
  JEWISH,
  MUSLIM,
  BUDDHIST,
  HINDU,
  NUM_DENOMINATIONS
};

static const double PHOENIX_AGE_PROBS[] = {0.12, 0.14, 0.32, 0.30, 0.12};
static const double PHOENIX_ETHNICITY_PROBS[] = {0.41, 0.42, 0.07,
                                                 0.04, 0.02, 0.04};
static const double PHOENIX_RELIGION_PROBS[] = {0.26, 0.21, 0.18, 0.09, 0.05,
                                                0.02, 0.01, 0.03, 0.02};
