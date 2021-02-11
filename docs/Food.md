## Concept

Food is described in terms of its serving size and the amount of *rapidly available glucose* (RAG), *slowly available glucose* (SAG), protein, and fat per serving. The RAG contents include sugars and the *rapidly digestible* starch (i.e. starch that gets digested in vitro within 20 minutes[^39][^40}]). The SAG contents include the *slowly digestible* starch (i.e. starch that get digested between 20 and 120 minutes [^39][^40]).

In general, the starch with high amylopectin to amylose ratio is classified as rapidly digestible starch whereas the one with high amylose to amylopectin ratio is classified as slowly digestible starch. The *non-starch polysaccharide* (also known as *dietary fiber*) part of the carbohydrates is currently ignored (even though the fiber contents of the food are known to have an impact on the *gastric emptying*).

CarbMetSim currently does not have a detailed implementation of the protein and lipid metabolism. However, it does model the impact of protein and fat contents of food on gastric emptying. Hence, the food description should include the total amount of protein and total amount of fat per serving. CarbMetSim currently does not characterize protein in terms of its amino acid contents. Since only 3 of the 20 amino acids have *branched chains*, a general assumption is made that 85% of amino acids resulting from protein digestion have *unbranched chains* and the remaining have *branched chains*[^41].



## Design

### Food event

A food event is represented by four fields:

| type      | name      | description                                    |
| --------- | --------- | ---------------------------------------------- |
| unsigned  | fireTime  | the fire time of the event in simulation ticks |
| EventType | eventType | always `EventType::FOOD`                       |
| unsigned  | quantity  | the amount of food in grams                    |
| unsigned  | foodID    | uniquely identifies the food item              |

A new food event can be constructed with `FoodEvent::FoodEvent(unsigned fireTime, unsigned quantity, unsigned foodID)` or by adding one directly to the simulator. For information on adding events, see [Events](Events.md). Clock time can be converted to ticks with the function `unsigned SimCtl::timeToTicks(unsigned days, unsigned hours, unsigned minutes)`.



### Food type











## Validation





## References

[^39]: Englyst HN, Kingman S, Cummings J. Classification and Measurement of Nutritionally Important Starch Fractions. European Journal of Clinical Nutrition. 1992; 46:S33–50. PMID: 1330528

[^40]: Frayn K. Metabolic Regulation: A Human Perspective, 3rd Edition. Wiley-Blackwell; 2010.

[^41]: Gleeson M. Interrelationship between Physical Activity and Branched-Chain Amino Acids. J Nutrition. 2005; 135(6):1591S–1595S. https://doi.org/10.1093/jn/135.6.1591S

