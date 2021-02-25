## Usage

On the command line parameters can be set by including them in the file passed to the *paramsfile* argument. Strings must match the case and spelling exactly to be accepted. They can also only be set on an organ that includes the given parameter.

When using the library parameters can be read through pointers on a SimCtl object corresponding to the organ that includes a given parameter, as given below. Setting parameters is done through construction of the SimCtl object by passing an object of type `std::map<BodyState, MetabolicParams>`. See [SimCtl](SimCtl.md) for an explanation of how to use this object. Refer back to here for the meaning of each parameter you wish to use.



## Adipose Tissue

Readable through `SimCtl::adiposeTissue`. No parameters currently settable.

| Identifier | Value Type | Unit | Default Value | Readable | Settable |
| ---------- | ---------- | ---- | ------------- | -------- | -------- |
| fat        | double     | g    | n/a           | X        |          |

### Glossary

**fat:** Body fat.



## Blood

Readable through `SimCtl::blood`. Settable through `MetabolicParams::blood`.

| Identifier           | Value Type | Unit                | Default Value              | Readable | Settable |
| -------------------- | ---------- | ------------------- | -------------------------- | -------- | -------- |
| alanine              | double     |                     | 0                          | X        |          |
| baseGlucoseLevel     | double     | mg / dl             | 100                        |          | X        |
| baseInsulinLevel     | double     |                     | 0                          | X        | X        |
| branchedAminoAcids   | double     |                     | 0                          | X        |          |
| glEnterocytes        | double     |                     | 0                          | X        |          |
| glLumen              | double     |                     | 0                          | X        |          |
| glPortalVein         | double     |                     | 0                          | X        |          |
| glucose              | double     | mg                  | 100 * 50.0                 | X        |          |
| glutamine            | double     |                     | 0                          | X        |          |
| glycationProbConst   | double     |                     | 0                          |          | X        |
| glycationProbSlope   | double     | decimal %           | 0.085 / 10000.0            |          | X        |
| glycolysisMax        | double     | micromol / kg / min | 0.9 * 0.35 * 2 * 0.1801559 |          | X        |
| glycolysisMin        | double     | micromol / kg / min | 0.35 * 0.5 * 0.1801559     |          | X        |
| glycolysisPerTick    | double     |                     | 0                          | X        |          |
| glycolysisToLactate  | double     | decimal %           | 1.0                        |          | X        |
| gngSubstrates        | double     |                     | 0                          | X        |          |
| highGlucoseLevel     | double     | mg / dl             | 200                        |          | X        |
| highLactateLevel     | double     | mg                  | 4053.51                    |          | X        |
| insulinLevel         | double     |                     | 0                          | X        |          |
| killBin              | int        |                     | N/A                        | X        |          |
| killRate             | double[]   |                     | N/A                        | X        |          |
| lactate              | double     | mg                  | 450.39                     | X        |          |
| minGlucoseLevel      | double     | mg / dl             | 40                         | X        | X        |
| peakInsulinLevel     | double     |                     | 1.0                        |          | X        |
| rbcBirthRate         | double     | count               | 144 * 60 * 24              |          | X        |
| RBCsUpdated          | bool       |                     | false                      | X        |          |
| totalGlycolysisSoFar | double     |                     | 0.0                        | X        |          |
| unbranchedAminoAcids | double     |                     | 0                          | X        |          |

| Function         | Return type | Unit    |
| ---------------- | ----------- | ------- |
| baseBGL          | double      | mg / dl |
| getBGL           | double      | mg / dl |
| getGNGSubstrates | double      |         |
| highBGL          | double      | mg / dl |
| volume           | double      | dl      |

### Glossary

**alanine:**

**baseBGL:**

**baseGlucoseLevel:** Typical fasting BGL.

**baseInsulinLevel:** Typical fasting insulin level.

**branchedAminoAcids:**

**getBGL:**

**getGNGSubstrates:**

**glEnterocytes:**

**glLumen:**

**glPortalVein:**

**glucose:**

**glutamine:**

**glycationProbConst:**

**glycationProbSlope:**

**glycolysisMax:**

**glycolysisMin:**

**glycolysisPerTick:**

**glycolysisToLactate:**

**gngSubstrates:**

**highBGL:**

**highGlucoseLevel:** Typical peak BGL.

**highLactateLevel:**

**insulinLevel:** The impact of insulin on the various organs (not the actual concentration). It is possible that two different actual insulin concentrations for two individuals map to the same value for *insulinLevel* because they have the same impact on carbohydrate metabolism-related functions of the organs.

**killBin:**

**killRate:**

**lactate:**

**minGlucoseLevel:** Typical hypoglycemic BGL.

**peakInsulinLevel:** Typical insulin level when BGL is at peak. A value of 1 means normal (or excessive in the case of the initial stages of type 2 diabetes). A value of 0 means the pancreas does not produce any insulin at all (as in people with type 1 diabetes).

**rbcBirthRate:**

**RBCsUpdated:**

**totalGlycolysisSoFar:**

**unbranchedAminoAcids:**

**volume:**



## Brain

Readable through `SimCtl::brain`. Settable through `MetabolicParams::brain`.

| Identifier       | Value Type   | Unit     | Default Value | Readable | Settable |
| ---------------- | ------------ | -------- | ------------- | -------- | -------- |
| bAAToGlutamine   | double       |          | 0             |          | X        |
| glucoseOxidized  | double       | mg / min | 83.333        |          | X        |
| glucoseRemoved   | GlucoseState | n/a      | n/a           | X        |          |
| glucoseToAlanine | double       |          | 0             |          | X        |
| oxidationPerTick | double       |          |               | X        |          |

### Glossary

**bAAToGlutamine:**

**glucoseOxidized:** The mean used to calculate *oxidationPerTick*.

**glucoseRemoved:**

**glucoseToAlanine:**

**oxidationPerTick:** A poisson-distribute random value which determines the amount of glucose oxidized during a single tick.



## Chyme Consumed

Diagnostic information accessible through `SimCtl::intestine::chymeConsumed`.

| Identifier  | Value Type | Unit |
| ----------- | ---------- | ---- |
| origRAG     | double     |      |
| origSAG     | double     |      |
| RAG         | double     |      |
| RAGConsumed | double     |      |
| SAG         | double     |      |
| SAGConsumed | double     |      |

### Glossary

**origRAG:**

**origSAG:**

**RAG:** Rapidly available glucose, which includes sugars and rapidly digestible starches (i.e. starch that gets digested in vitro within 20 minutes).

**RAGConsumed:**

**SAG:** Slowly available glucose, which includes slowly digestible starches (i.e. starch that gets digested in vitro between 20 and 120 minutes).

**SAGConsumed:**



## Fat State

Diagnostic information accessible through `SimCtl::stomach::fatInBolus`.

| Identifier | Value Type | Unit |
| ---------- | ---------- | ---- |
| amount     | double     | mg   |
| bodyWeight | double     | kg   |

### Glossary

**amount:**

**bodyWeight:**


## Gluconeogenesis State

Diagnostic information accessible through `SimCtl::liver::postGluconeogenesis`.

| Identifier   | Value Type | Unit |
| ------------ | ---------- | ---- |
| bloodGlucose | double     |      |
| bloodLactate | double     |      |
| glucose      | double     | mg   |
| glycogen     | double     | mg   |

### Glossary

**bloodGlucose:**

**bloodLactate:**

**glucose:**

**glycogen:**



## Glucose State

Diagnostic information accessible through the following member variables:

* `SimCtl::brain::glucoseRemoved`
* `SimCtl::heart::basalAbsorption`
* `SimCtl::intestine::excessGlucoseInEntercytes`
* `SimCtl::kidneys::postGluconeogenesis`
* `SimCtl::kidneys::postGlucoseExtraction`
* `SimCtl::kidneys::postGlycolysis`
* `SimCtl::muscles::absorptionState`
* `SimCtl::muscles::basalBase`
* `SimCtl::muscles::basalGLUT4`
* `SimCtl::portalVein::fromBlood`

| Identifier  | Value Type | Unit    |
| ----------- | ---------- | ------- |
| amount      | double     | mg      |
| bloodBGL    | double     | mg / dl |
| bloodMinBGL | double     | mg / dl |

### Glossary

**amount:**

**bloodBGL:**

**bloodMinBGL:**



## Glycogen Synthesis State

Diagnostic information accessible through `SimCtl::liver::postGlycogen`.

| Identifier | Value Type | Unit |
| ---------- | ---------- | ---- |
| glucose    | double     | mg   |
| glycogen   | double     | mg   |

### Glossary

**glucose:**

**glycogen:**


## Heart

Readable through `SimCtl::heart`. Settable through `MetabolicParams::heart`.

| Identifier           | Value Type   | Unit          | Default Value       | Readable | Settable |
| -------------------- | ------------ | ------------- | ------------------- | -------- | -------- |
| basalAbsorption      | GlucoseState | n/a           | n/a                 | X        |          |
| basalGlucoseAbsorbed | double       | mg / min      | 14                  |          | X        |
| Glut4Km              | double       | mg / dl       | 5 * 180.1559 / 10.0 |          | X        |
| Glut4VMAX            | double       | mg / kg / min | 0                   |          | X        |
| lactateOxidized      | double       |               | 0                   |          | X        |
| oxidationPerTick     | double       |               |                     | X        |          |

### Glossary

**basalAbsorption:**

**basalGlucoseAbsorbed:**

**Glut4Km:**

**Glut4VMAX:**

**lactateOxidized:**

**oxidationPerTick:** A poisson-distributed random value which determines the amount of glucose consumed from the blood to be oxidized.



## Human Body

Readable through `SimCtl::body`. Settable through `MetabolicParams::body`.

| Identifier                                   | Value Type    | Unit            | Default Value | Readable | Settable |
| -------------------------------------------- | ------------- | --------------- | ------------- | -------- | -------- |
| adiposeTissue                                | AdiposeTissue | n/a             |               |          |          |
| age                                          | int           | years           | n/a           | X        | X        |
| baseBGL                                      | double        | mg / dL         |               | X        |          |
| blood                                        | Blood         | n/a             | n/a           | X        |          |
| bodyWeight                                   | double        | kg              | n/a           | X        | X        |
| brain                                        | Brain         | n/a             | n/a           | X        |          |
| dayEndTotals                                 | TotalsState   | n/a             | n/a           | X        |          |
| excretionKidneysImpact                       | double        |                 |               | X        | X        |
| exerciseOverAt                               | unsigned      | ticks           | 0             | X        |          |
| fatFraction                                  | double        |                 | 0.2           | X        |          |
| fitnessLevel                                 | int           | %               | n/a           | X        | X        |
| gender                                       | int           | 0 or 1          | n/a           | X        | X        |
| glut4Impact                                  | double        |                 | 1.0           | X        | X        |
| glycolysisMaxImpact                          | double        |                 | 1.0           |          | X        |
| glycolysisMinImpact                          | double        |                 | 1.0           |          | X        |
| gngImpact                                    | double        |                 | 6.0           |          | X        |
| heart                                        | Heart         | n/a             | n/a           | X        |          |
| insulinImpactGlycogenBreakdownInLiver_Mean   | double        |                 | 0.1           | X        | X        |
| insulinImpactGlycogenBreakdownInLiver_StdDev | double        |                 | 0.02          |          | X        |
| insulinImpactGlycogenSynthesisInLiver_Mean   | double        |                 | 0.5           |          | X        |
| insulinImpactGlycogenSynthesisInLiver_StdDev | double        |                 | 0.2           |          | X        |
| insulinImpactOnGlycolysis_Mean               | double        |                 | 0.5           |          | X        |
| insulinImpactOnGlycolysis_StdDev             | double        |                 | 0.2           |          | X        |
| insulinImpactOnGNG_Mean                      | double        |                 | 0.5           | X        | X        |
| insulinImpactOnGNG_StdDev                    | double        |                 | 0.2           |          | X        |
| intensityPeakGlucoseProd                     | double        | decimal %       | 0.2           | X        | X        |
| intestine                                    | Intestine     | n/a             | n/a           | X        |          |
| kidneys                                      | Kidneys       | n/a             | n/a           | X        |          |
| lastHardExerciseAt                           | int           | ticks           | -61           | X        |          |
| liver                                        | Liver         | n/a             | n/a           | X        |          |
| liverGlycogenBreakdownImpact                 | double        |                 | 6.0           |          | X        |
| liverGlycogenSynthesisImpact                 | double        |                 | 1.0           | X        | X        |
| muscles                                      | Muscles       | n/a             | n/a           | X        |          |
| peakBGL                                      | double        | mg / dl         |               | X        |          |
| percentVO2Max                                | double        | decimal %       | n/a           | X        |          |
| portalVein                                   | PortalVein    | n/a             | n/a           | X        |          |
| stomach                                      | Stomach       | n/a             | n/a           | X        |          |
| tempExcretion                                | double        |                 | n/a           | X        |          |
| tempGlycogenBreakdown                        | double        | mg              | n/a           | X        |          |
| tempGlycogenStorage                          | double        | mg              | n/a           | X        |          |
| tempGlycolysis                               | double        | mg              | n/a           | X        |          |
| tempGNG                                      | double        | mg              | n/a           | X        |          |
| tempOxidation                                | double        | mg              | n/a           | X        |          |
| totalEndogeneousGlucoseReleasePerTick        | double        |                 |               | X        |          |
| totalEndogeneousGlucoseReleaseSoFar          | double        | mg              |               |          |          |
| totalGlucoseReleasePerTick                   | double        |                 |               | X        |          |
| totalGlucoseReleaseSoFar                     | double        | mg              |               |          |          |
| totalGlycogenBreakdownPerTick                | double        |                 |               | X        |          |
| totalGlycogenStoragePerTick                  | double        |                 |               |          |          |
| totalGlycolysisPerTick                       | double        | mg              |               | X        |          |
| totalGlycolysisSoFar                         | double        | mg              | 0             | X        |          |
| totalGNGPerTick                              | double        | mg              |               | X        |          |
| totalGNGSoFar                                | double        | mg              | 0             | X        |          |
| totalLiverGlycogenBreakdownSoFar             | double        | mg              | 0             | X        |          |
| totalLiverGlycogenStorageSoFar               | double        | mg              | 0             | X        |          |
| totalMusclesGlycogenBreakdownSoFar           | double        | mg              | 0             | X        |          |
| totalMusclesGlycogenStorageSoFar             | double        | mg              | 0             | X        |          |
| totalOxidationPerTick                        | double        | mg              |               | X        |          |
| totalOxidationSoFar                          | double        | mg              | 0             | X        |          |
| VO2Max                                       | double        | mLO2 / kg / min | n/a           | X        |          |

| Function                                | Return type | Unit      |
| --------------------------------------- | ----------- | --------- |
| currentEnergyExpenditure                | double      |           |
| getGlucoseEnergyExpenditure             | double      |           |
| insulinImpactOnGlycogenBreakdownInLiver | double      | decimal % |
| insulinImpactOnGlycogenSynthesisInLiver | double      | decimal % |
| insulinImpactOnGlycolysis               | double      |           |
| insulinImpactOnGNG                      | double      |           |

### Glossary

**adiposeTissue:**  Models the effects of adipose tissue on carbohydrate metabolism.

**age:** The age of the simulated subject in years.

**baseBGL:**

**blood:** Models the effects of blood on carbohydrate metabolism.

**bodyWeight:**

**brain:** Models the effects of the brain on carbohydrate metabolism.

**currentEnergyExpenditure:**

**dayEndTotals:**

**excretionKidneysImpact:** A multiplicative factor used to modify the amount of glucose excreted per tick.

**exerciseOverAt:**

**fatFraction:**

**fitnessLevel:** The self-assessed fitness level of the simulated subject as an integer percentage.

**gender:** The gender of the simulated subject, either male (0) or female (1).

**getGlucoseEnergyExpenditure:**

**glut4Impact:**

**glycolysisMaxImpact:**

**glycolysisMinImpact:**

**gngImpact:**

**heart:** Models the effects of the heart on carbohydrate metabolism.

**insulinImpactOnGlycogenBreakdownInLiver:** The impact of insulin on glycogen breakdown in the liver.

**insulinImpactGlycogenBreakdownInLiver_Mean:** The normal distribution mean used to calculate the impact of insulin on glycogen breakdown in the liver.

**insulinImpactGlycogenBreakdownInLiver_StdDev:** The normal distribution standard deviation used to calculate the impact of insulin on glycogen breakdown in the liver.

**insulinImpactOnGlycogenSynthesisInLiver:** The impact of insulin on glycogen synthesis in the liver.

**insulinImpactGlycogenSynthesisInLiver_Mean:** The normal distribution mean used to calculate the impact of insulin on glycogen synthesis in the liver.

**insulinImpactGlycogenSynthesisInLiver_StdDev:** The normal distribution standard deviation used to calculate the impact of insulin on glycogen synthesis in the liver.

**insulinImpactOnGlycolysis:** The impact of insulin on glycolysis.

**insulinImpactOnGlycolysis_Mean:** The normal distribution mean used to calculate the impact of insulin on glycolysis.

**insulinImpactOnGlycolysis_StdDev:** The normal distribution standard deviation used to calculate the impact of insulin on glycolysis.

**insulinImpactOnGNG:** The impact of insulin on gluconeogenesis.

**insulinImpactOnGNG_Mean:** The normal distribution mean used to calculate the impact of insulin on gluconeogenesis.

**insulinImpactOnGNG_StdDev:** The normal distribution standard deviation used to calculate the impact of insulin on gluconeogenesis.

**intensityPeakGlucoseProd:** The exercise intensity in % VO2max at which the liver and kidney produce glucose at the maximum rate.

**intestine:** Models the effects of the intestine on carbohydrate metabolism.

**kidneys:** Models the effects of the kidneys on carbohydrate metabolism.

**lastHardExerciseAt:**

**liver:** Models the effects of the liver on carbohydrate metabolism.

**liverGlycogenBreakdownImpact:** Modifies the average rate of glycogen breakdown in the liver (i.e. *glycogenToGlucoseInLiver*). Can be used to model the increased liver glycogen breakdown when BGL is low.

**liverGlycogenSynthesisImpact:** Modifies the average rate of glycogen synthesis in the liver (i.e. *glucoseToGlycogenInLiver*). Can be used to model the impact of diabetes on glycogen synthesis.

**muscles:** Models the effects of the muscles on carbohydrate metabolism.

**peakBGL:**

**percentVO2Max:** The intensity of an exercise activity in terms of oxygen consumption rate.

**portalVein:** Models the effects of the portal vein on carbohydrate metabolism.

**stomach:** Models the effects of the stomach on carbohydrate metabolism.

**tempExcretion:**

**tempGlycogenBreakdown:**

**tempGlycogenStorage:**

**tempGlycolysis:**

**tempGNG:**

**tempOxidation:**

**totalEndogeneousGlucoseReleasePerTick:**

**totalEndogeneousGlucoseReleaseSoFar:**

**totalGlucoseReleasePerTick:**

**totalGlucoseReleaseSoFar:**

**totalGlycogenBreakdownPerTick:**

**totalGlycogenStoragePerTick:**

**totalGlycolysisPerTick:**

**totalGlycolysisSoFar:**

**totalGNGPerTick:**

**totalGNGSoFar:**

**totalLiverGlycogenBreakdownSoFar:**

**totalLiverGlycogenStorageSoFar:**

**totalMusclesGlycogenBreakdownSoFar:**

**totalMusclesGlycogenStorageSoFar:**

**totalOxidationPerTick:**

**totalOxidationSoFar:**

**VO2Max:** The maximal rate at which an individual can consume oxygen. In CarbMetSim, this rate is estimated from age, gender, and self-reported fitness level.



## Intestine

Readable through `SimCtl::intestine`. Settable through `MetabolicParams::intestine`.

| Identifier                 | Value Type            | Unit     | Default Value                | Readable | Settable |
| -------------------------- | --------------------- | -------- | ---------------------------- | -------- | -------- |
| activeAbsorption           | double                |          | 0                            | X        |          |
| aminoAcidAbsorptionRate    | double                | mg / min | 1                            |          | X        |
| chymeConsumed              | vector[ChymeConsumed] | n/a      | n/a                          | X        |          |
| excessGlucoseInEnterocytes | GlucoseState          | n/a      | n/a                          | X        |          |
| fluidVolumeInEnterocytes   | double                | dL       | 3                            |          | X        |
| fluidVolumeInLumen         | double                | dL       | 4                            |          | X        |
| glEnterocytes              | double                |          | 0                            | X        |          |
| glLumen                    | double                |          | 0                            | X        |          |
| glPortalVein               | double                |          | 0                            | X        |          |
| glPortalVeinConcentration  | double                |          | 0                            | X        |          |
| Glut2Km_In                 | double                | mg / dL  | 20 * 180.1559 / 10.0         |          | X        |
| Glut2Km_Out                | double                | mg / dL  | 20 * 180.1559 / 10.0         |          | X        |
| Glut2VMAX_In               | double                | mg       | 800                          |          | X        |
| Glut2VMAX_Out              | double                | mg       | 800                          |          | X        |
| glutamineOxidationRate     | double                | mg / min | 1                            |          | X        |
| glutamineToAlanineFraction | double                |          | 0.5                          |          | X        |
| glycolysisMax              | double                |          | 0.9 * 0.35 * 2.0 * 0.1801559 |          | X        |
| glycolysisMin              | double                |          | 0.35 * 0.5 * 0.1801559       |          | X        |
| glycolysisPerTick          | double                |          |                              | X        |          |
| passiveAbsorption          | double                |          | 0                            | X        |          |
| RAG_Mean                   | double                |          | 2                            |          | X        |
| RAG_StdDev                 | double                |          | 0.5                          |          | X        |
| SAG_Mean                   | double                |          | 30                           |          | X        |
| SAG_StdDev                 | double                |          | 10                           |          | X        |
| sglt1Rate                  | double                | mg / min | 30                           |          | X        |
| toPortalVeinPerTick        | double                |          |                              | X        |          |
| totalRAGConsumed           | double                |          | 0                            | X        |          |
| totalSAGConsumed           | double                |          | 0                            | X        |          |

### Glossary

**activeAbsorption:**

**aminoAcidAbsorptionRate:**

**chymeConsumed:**

**excessGlucoseInEnterocytes:**

**fluidVolumeInEnterocytes:**

**fluidVolumeInLumen:**

**glEnterocytes:**

**glLumen:**

**glPortalVein:**

**glPortalVeinConcentration:**

**Glut2Km_In:**  The Michaelis-Menten Km associated with GLUT2 transporters in moving glucose from the lumen to the enterocytes.

**Glut2Km_Out:** The Michaelis-Menten Km associated with GLUT2 transporters in moving glucose from the enterocytes to the portal vein.

**Glut2VMAX_In:** The Michaelis-Menten Vmax associated with GLUT2 transporters in moving glucose from the lumen to the enterocytes.

**Glut2VMAX_Out:** The Michaelis-Menten Vmax associated with GLUT2 transporters in moving glucose from the enterocytes to the portal vein.

**glutamineOxidationRate:**

**glutamineToAlanineFraction:**

**glycolysisMax:**

**glycolysisMin:**

**glycolysisPerTick:**

**passiveAbsorption:**

**RAG_Mean:**

**RAG_StdDev:**

**SAG_Mean:**

**SAG_StdDev:**

**sglt1Rate:**

**toPortalVeinPerTick:**

**totalRAGConsumed:**

**totalSAGConsumed:**



## Kidneys

Readable through `SimCtl::kidneys`. Settable through `MetabolicParams::kidneys`.

| Identifier            | Value Type   | Unit          | Default Value                | Readable | Settable |
| --------------------- | ------------ | ------------- | ---------------------------- | -------- | -------- |
| excretionPerTick      | double       |               |                              | X        |          |
| glycolysisMax         | double       | mg / kg / min | 0.9 * 0.35 * 2.0 * 0.1801559 |          | X        |
| glycolysisMin         | double       | mg / kg / min | 0.35 * 0.5 * 0.1801559       |          | X        |
| glycolysisPerTick     | double       | mg            |                              | X        |          |
| gngKidneys            | double       | mg / kg / min | 0.16                         |          | X        |
| gngPerTick            | double       |               |                              | X        |          |
| postGluconeogenesis   | GlucoseState | n/a           | n/a                          | X        |          |
| postGlucoseExtraction | GlucoseState | n/a           | n/a                          | X        |          |
| postGlycolysis        | GlucoseState | n/a           | n/a                          | X        |          |
| toGlycolysis          | double       |               | 0                            | X        |          |
| totalExcretion        | double       | mg            | 0                            | X        |          |

### Glossary

**excretionPerTick:**

**glycolysisMax:**

**glycolysisMin:**

**glycolysisPerTick:**

**gngKidneys:** The average rate of gluconeogenesis in the kidneys.

**gngPerTick:**

**postGluconeogenesis:**

**postGlucoseExtraction:**

**postGlycolysis:**

**toGlycolysis:**

**totalExcretion:**



## Liver

Readable through `SimCtl::liver`. Settable through `MetabolicParams::liver`.

| Identifier                  | Value Type            | Unit          | Default Value               | Readable | Settable |
| --------------------------- | --------------------- | ------------- | --------------------------- | -------- | -------- |
| absorptionPerTick           | double                |               |                             | X        |          |
| excessGlucoseAbsorption     | double                |               | 0                           | X        |          |
| fluidVolume                 | double                | dL            | 12                          | X        | X        |
| fromGlycogenPerTick         | double                |               |                             | X        |          |
| glucoseNeeded               | double                |               | 0                           | X        |          |
| glucoseToGlycogenInLiver    | double                | mg / kg / min | 4.5                         |          | X        |
| glucoseToNEFA               | double                |               | 0                           |          | X        |
| Glut2Km                     | double                | mg / dL       | 20 * 180.1559 / 10.0        |          | X        |
| Glut2VMAX                   | double                | mg / kg / min | 50                          |          | X        |
| glucose                     | double                | mg            | 100 * 12                    | X        |          |
| glycogen                    | double                |               | 100.0 * 1000.0              | X        | X        |
| glycogenMax                 | double                |               | 120.0 * 1000.0              |          | X        |
| glycogenToGlucoseInLiver    | double                | mg / kg / min | 0.9                         |          | X        |
| glycolysisMax               | double                | mg / kg / min | 0.9 * 0.35 * 10 * 0.1801559 |          | X        |
| glycolysisMin               | double                | mg / kg / min | 0.35 * 0.1801559            |          | X        |
| glycolysisPerTick           | double                |               |                             | X        |          |
| glycolysisToLactateFraction | double                | decimal %     | 1                           |          | X        |
| gngLiver                    | double                | mg / kg / min | 0.16                        |          | X        |
| gngPerTick                  | double                |               |                             | X        |          |
| lipogenesisOccurred         | bool                  | n/a           | false                       | X        |          |
| maxGNGDuringExercise        | double                |               | 0                           | X        |          |
| maxLipogenesis              | double                | mg / min      | 400.0                       |          | X        |
| postGluconeogenesis         | GluconeogenesisState  | n/a           | n/a                         | X        |          |
| postGlycogen                | GlycogenSythesisState | n/a           | n/a                         | X        |          |
| releasePerTick              | double                |               |                             | X        |          |
| toGlycogenPerTick           | double                |               |                             | X        |          |

### Glossary

**absorptionPerTick:**

**excessGlucoseAbsorption:**

**fluidVolume:**

**fromGlycogenPerTick:**

**eglucoseNeeded:**

**glucoseToGlycogenInLiver:** The average rate of glycogen synthesis.

**glucoseToNEFA:**

**Glut2Km:** The Michaelis-Menten Km associated with GLUT2 transporters.

**Glut2VMAX:** The Michaelis-Menten Vmax associated with GLUT2 transporters

**glucose:**

**glycogen:**

**glycogenMax:**

**glycogenToGlucoseInLiver:** The average glycogen breakdown flux.

**glycolysisMax:**

**glycolysisMin:**

**glycolysisPerTick:**

**glycolysisToLactateFraction:**

**gngLiver:** The average rate of gluconeogenesis in the liver.

**gngPerTick:**

**lipogenesisOccurred:**

**maxGNGDuringExercise:**

**maxLipogenesis:**

**postGluconeogenesis:**

**postGlycogen:**

**releasePerTick:**

**toGlycogenPerTick:**




## Muscles

Readable through `SimCtl::muscles`. Settable through `MetabolicParams::muscles`.

| Identifier                         | Value Type   | Unit                | Default Value                 | Readable | Settable |
| ---------------------------------- | ------------ | ------------------- | ----------------------------- | -------- | -------- |
| absorbedFromBlood                  | double       | mg                  | 0                             | X        |          |
| absorptionState                    | GlucoseState | n/a                 | n/a                           | X        |          |
| baaToGlutamine                     |              |                     |                               |          | X        |
| basalBase                          | GlucoseState | n/a                 | n/a                           | X        |          |
| basalGlucoseAbsorbed               | double       | mg / kg / min       | 0                             |          | X        |
| basalGLUT4                         | GlucoseState | n/a                 | n/a                           | X        |          |
| basalGLUT4Occurred                 | bool         | n/a                 | false                         | X        |          |
| glucoseToGlycogen                  | double       | micromol / kg / min | 15.0 * 0.1801559              |          | X        |
| Glut4Km                            | double       | mg / dL             | 5 * 180.1559 / 10.0           |          | X        |
| Glut4VMAX                          | double       | mg / kg / min       | 3.5                           |          | X        |
| glucoseAbsorbedPerTick             | double       |                     |                               | X        |          |
| glycogen                           | double       | mg                  | 500 * 1000.0                  | X        | X        |
| glycogenBreakdownPerTick           | double       |                     |                               | X        |          |
| glycogenMax                        | double       | mg                  | 500 * 1000.0                  |          | X        |
| glycogenOxidizedPerTick            | double       |                     |                               | X        |          |
| glycogenShare                      | double       |                     |                               | X        |          |
| glycogenShareDuringExerciseMean    | double       |                     | 0.53                          |          | X        |
| glycogenSynthesizedPerTick         | double       |                     |                               | X        |          |
| glycolysisMax                      | double       | mg / kg / min       | 0.9 * 0.35 * 15.0 * 0.1801559 |          | X        |
| glycolysisMin                      | double       | mg / kg / min       | 0.35 * 1.0 * 0.1801559        |          | X        |
| glycolysisPerTick                  | double       |                     |                               | X        |          |
| hadExercise                        | bool         | n/a                 | false                         | X        |          |
| maxGlucoseAbsorptionDuringExercise | double       | micromol / kg / min | 30.0 * 0.1801559              |          | X        |
| oxidationPerTick                   | double       |                     |                               | X        |          |
| peakGlut4VMAX                      | double       | mg / kg / min       | 2 * 3.5                       |          | X        |

### Glossary

**absorbedFromBlood:**

**absorptionState:**

**baaToGlutamine:**

**basalBase:**

**basalGlucoseAbsorbed:**

**basalGLUT4:**

**basalGLUT4Occurred:**

**glucoseToGlycogen:**

**Glut4Km:** The Michaelis-Menten Km associated with GLUT4 transporters.

**Glut4VMAX:** The Michaelis-Menten Vmax associated with GLUT4 transporters.

**glucoseAbsorbedPerTick:**

**glycogen:**

**glycogenBreakdownPerTick:** The total amount of glycogen broken down into glucose and lactate during a single tick.

**glycogenMax:** The maximum amount of glycogen that can be stored in the muscles.

**glycogenOxidizedPerTick:** The amount of glucose oxidized from glycogen during a single tick.

**glycogenShare:** A normally-distributed random value used to determine the fraction of energy needs to be met by oxidizing glucose from glycogen during a single tick.

**glycogenShareDuringExerciseMean:** The mean used to calculate *glycogenShare*. Calculated such that 50% VO2Max and 100% VO2Max yields values close to 0.4 and 0.9 respectively.

**glycogenSynthesizedPerTick:**

**glycolysisMax:** The maximum amount of glucose that can be consumed by glycolysis in a single tick.

**glycolysisMin:** The mean used to calculate *glycolysisPerTick*.

**glycolysisPerTick:** A poisson-distributed random value used to determine the amount of glucose consumed by glycolysis in a single tick.

**hadExercise:** Whether the muscles reacted to an exercise event during the current tick.

**maxGlucoseAbsorptionDuringExercise:** The maximum rate at which the muscles can absorb glucose from the blood during exercise.

**oxidationPerTick:**

**peakGlut4VMAX:**



## Portal Vein

Readable through `SimCtl::portalVein`. Settable through `MetabolicParams::portalVein`.

| Identifier  | Value Type   | Unit | Default Value | Readable | Settable |
| ----------- | ------------ | ---- | ------------- | -------- | -------- |
| fluidVolume | double       | dL   | 5             |          | X        |
| fromBlood   | GlucoseState | n/a  | n/a           | X        |          |

### Glossary

**fluidVolume:**

**fromBlood:**



## Stomach

Readable through `SimCtl::stomach`. Settable through `MetabolicParams::stomach`.

| Identifier         | Value Type | Unit | Default Value | Readable | Settable |
| ------------------ | ---------- | ---- | ------------- | -------- | -------- |
| calorificDensity   | double     |      | 0             | X        |          |
| fat                | double     | mg   | 0             | X        |          |
| fatInBolus         | FatState   | n/a  | n/a           | X        |          |
| geConstant         | double     | mg   | 500.0         |          | X        |
| geSlope            | double     |      | 0             | X        |          |
| geSlopeMin         | double     |      | 0.03          |          | X        |
| protein            | double     | mg   | 0             | X        |          |
| proteinInBolus     | double     |      | 0             | X        |          |
| RAG                | double     | mg   | 0             | X        |          |
| ragInBolus         | double     |      | 0             | X        |          |
| SAG                | double     | mg   | 0             | X        |          |
| sagInBolus         | double     |      | 0             | X        |          |
| stomachBecameEmpty | double     | n/a  | false         | X        |          |
| stomachEmpty       | bool       | n/a  | true          | X        |          |
| totalFood          | double     |      | 0             | X        |          |

### Glossary

**calorificDensity:**

**fat:**

**fatInBolus:**

**geConstant:**

**geSlope:**

**geSlopeMin:**

**protein:**

**proteinInBolus:**

**RAG:** Rapidly available glucose, which includes sugars and rapidly digestible starches (i.e. starch that gets digested in vitro within 20 minutes).

**ragInBolus:**

**SAG:** Slowly available glucose, which includes slowly digestible starches (i.e. starch that gets digested in vitro between 20 and 120 minutes).

**sagInBolus:**

**stomachBecameEmpty:**

**stomachEmpty:**

**totalFood:**



## Totals State

Diagnostic information accessible through `SimCtl::body::dayEndTotals` and `SimCtl::body::getTotals()`.

| Identifier               | Value Type | Unit |
| ------------------------ | ---------- | ---- |
| excretion                | double     |      |
| glucoseFromIntestine     | double     |      |
| glycolysis               | double     |      |
| GNG                      | double     |      |
| liverGlycogenBreakdown   | double     | mg   |
| liverGlycogenStorage     | double     | mg   |
| musclesGlycogenBreakdown | double     | mg   |
| musclesGlycogenStorage   | double     | mg   |
| oxidation                | double     |      |

### Glossary

**excretion:**

**glucoseFromIntestine:**

**glycolysis:**

**GNG:**

**liverGlycogenBreakdown:**

**liverGlycogenStorage:**

**musclesGlycogenBreakdown:**

**musclesGlycogenStorage:**

**oxidation:**

