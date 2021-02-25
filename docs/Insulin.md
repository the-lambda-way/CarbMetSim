# Insulin

The insulin level in blood generally depends on the Blood Glucose Level (BGL). If the BGL is high, the insulin level increases as well so as to signal the liver and the muscles to absorb glucose from the blood stream and also to signal both the liver and the kidneys to slow down or stop the endogeneous glucose production via glycogen breakdown and gluconeogenesis. Also, the insulin level in blood decreases in response to physical exercise so as to signal the liver and the kidneys to ramp up the endogeneous glucose production. In CarbMetSim, the current insulin level in the blood is represented by a variable called insulinLevel (inside the Blood object) that assumes values between 0 and 1. The value of insulinLevel depends on the current BGL, the current exercise intensity (in %VO2max) and a number of configurable parameters: minGlucoseLevel (typical hypoglycemic BGL), baseGlucoseLevel (typical fasting BGL), highGlucoseLevel (typical peak BGL) (minGlucoseLevel < baseGlucoseLevel < highGlucoseLevel), baseInsulinLevel (representing the typical fasting insulin level), peakInsulinLevel (representing typical insulin level when BGL is at peak) (where 0 <= baseInsulinLevel <= peakInsulinLevel <= 1), restIntensity (the oxygen consumption rate in %VO 2 max when the individual is not exercising, by default 2 METs converted to %VO2max) and intensityPeakGlucoseProd (the exercise intensity in %VO2max at which the liver and kidney produce glucose at the maximum rate, by default 20%). The following rules govern the value of insulinLevel:

- If the current BGL is less than or equal to the minGlucoseLevel, the insulinLevel stays at value zero.

- If the current BGL is between the minGlucoseLevel and the baseGlucoseLevel, the insulinLevel depends on whether the individual being simulated is currently engaged in exercise or not. If the individual is exercising and

  - if the exercise intensity is greater than or equal to intensityPeakGlucoseProd, the insulinLevel stays at zero.
  - otherwise, the insulinLevel depends on the exercise intensity. As the exercise intensity decreases from intensityPeakGlucoseProd to the the restIntensity, the insulinLevel increases linearly from zero to the baseInsulinLevel.

  If the individual is not exercising, as the BGL increases from minGlucoseLevel to baseGlucoseLevel, the insulinLevel increases linearly from zero to the baseInsulinLevel.

- As the BGL increases from the baseGlucoseLevel to the highGlucoseLevel, the insulinLevel increases linearly from the baseInsulinLevel to the peakInsulinLevel.

- If the BGL is greater than or equal to the highGlucoseLevel, the insulinLevel stays at the peakInsulinLevel value.

In CarbMetSim, the peakInsulinLevel represents the peak ability to produce insulin. A value 1 for peakInsulinLevel means normal (or excessive, as in the case of initial stages of Type 2 Diabetes) insulin production, whereas a value 0 means that the pancreas does not produce any insulin at all (as in people with Type 1 Diabetes). A value x (between 0 and 1) for peakInsulinLevel means that peak insulin production is just x times the normal peak.

As described in the later sections, the insulinLevel variable has a profound impact on the operation of different organ objects in CarbMetSim. So, its value should be interpreted in terms of the impact it has on various organ objects, rather than the actual insulin concentration it corresponds to for a particular person. So, it is entirely possible that two very different actual insulin concentrations for two individuals map to the same value for the insulinLevel because they have the same impact on carbohydrate metabolism related functions of the organs.



