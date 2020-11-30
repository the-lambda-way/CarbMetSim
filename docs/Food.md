# Food

Food is described in terms of its serving size and the amount of rapidly available glucose (RAG), slowly available glucose (SAG), protein, and fat per serving. RAG contents include sugars and the starches that gets digested within 20 minutes. SAG contents include starches that gets digested between 20 and 120 minutes.

In general, the starch with high amylopectin to amylose ratio is classified as rapidly digestible starch whereas the one with high amylose to amylopectin ratio is classified as slowly digestible starch. The non-starch polysaccharide (also known as dietary fiber) part of the carbohydrates is currently ignored (even though the fiber contents of the food are known to have an impact on the gastric emptying).

CarbMetSim currently does not have a detailed implementation of the protein and lipid metabolism. However, it does model the impact of protein and fat contents of food on gastric emptying. Hence, the food description should include the total amount of protein and total amount of fat per serving. CarbMetSim currently does not characterize protein in terms of its amino acid contents. Since only 3 of the 20 amino acids have branched chains, a general assumption is made that 85% of amino acids resulting from protein digestion have unbranched chains and the remaining have branched chains.

