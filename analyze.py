import pandas as pd
import sys


file_path = "output/spatial_data.csv"




print("Analyzing demographics of infected agents...")

try:
    
    usecols = ["Time", "ClaimId", "State", "Ethnicity", "Denomination"]
    
    
    
    
    
    
    stats = {}
    
    
    chunksize = 100000
    for chunk in pd.read_csv(file_path, chunksize=chunksize, usecols=usecols):
        
        infected = chunk[chunk["State"] > 0]
        
        if infected.empty:
            continue

        
        
        
        
        
        pass
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    counts = {} 
    
    for chunk in pd.read_csv(file_path, chunksize=chunksize):
        
        mask = chunk["State"] > 0 
        relevant = chunk[mask]
        
        for _, row in relevant.iterrows():
            cid = row["ClaimId"]
            aid = row["AgentId"]
            eth = row["Ethnicity"]
            den = row["Denomination"]
            
            key = (cid, eth, den)
            if key not in counts:
                counts[key] = set()
            counts[key].add(aid)

    print("\n--- Demographics of Infected/Recovered Agents ---")
    
    
    eth_names = {0: "White", 1: "Hispanic", 2: "Black", 3: "Asian", 4: "Native", 5: "Multi"}
    
    for key, agents in counts.items():
        cid, eth, den = key
        count = len(agents)
        if count > 10: 
            print(f"Claim {cid}, Eth {eth_names.get(eth, eth)}, Denom {den}: {count} unique agents")

except Exception as e:
    print(f"Error: {e}")
