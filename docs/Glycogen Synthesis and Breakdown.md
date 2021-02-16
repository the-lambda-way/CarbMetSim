# Glycogen Synthesis and Breakdown

In human body, the liver helps maintain glucose homeostasis by storing excess glucose in blood during the post-prandial state (when the insulin levels are high) as glycogen and releasing glucose to the blood during the post-absorptive and exercising states (when insulin level is low) by breaking down the stored glycogen. Diabetes may effect both glycogen synthesis and breakdown in the liver.

Subject to the availability of glucose, the amount of glycogen synthesized by the Liver object in CarbMetSim simulator during each tick is a poisson distributed random variable with a configurable average (glucoseToGlycogenInLiver\_, 4.5 mg/kg/min by default) that is modified multiplicatively by two factors. The first factor models the impact of insulin on glycogen synthesis. This factor (with values between 0 and 1) increases in value with increase in the insulinLevel and is calculated using a sigmoid function, which is currently the CDF of a normal distribution with a configurable mean and standard deviation. The second factor called liverGlycogenSynthesisImpact (by default 1.0) simply modifies the configured average multiplicatively and can be used to model the impact of diabetes on glycogen synthesis in the liver. The Liver object has a finite capacity to store glycogen and hence any excess glycogen is converted to fat and stored in the AdiposeTissue object.

Glycogen breakdown in the liver serves as the key source of glucose when no new glucose is entering the body via food or when the glucose needs of the body increase due to intense physical exercise. Accordingly, in CarbMetSim, the amount of glycogen stored in the Liver that is broken down to glucose during a tick closely depends on the insulinLevel (and hence on the current BGL). When the insulinLevel is above the baseInsulinLevel (i.e. the BGL is more than the baseGlucoseLevel), the average glycogen breakdown flux in the Liver (glycogenToGlucoseInLiver, 0.9 mg/kg/min by default) is multiplied by a factor (between 0 and 1) that decreases in value with increase in the insulinLevel as per an inverse sigmoid function (currently, the complementary CDF of a normal distribution with a configurable mean and standard deviation). This allows us to model the decrease in liver glycogen breakdown with increase in the insulin level. On the other hand, if the insulinLevel is below the baseInsulinLevel (i.e. the BGL
is below the baseGlucoseLevel), the average Liver glycogen breakdown flux is multiplied by a factor that decreases in value from a configurable maximum (liverGlycogenBreakdownImpact >= 1, by default 6.0) to the minimum value 1 as the insulinLevel increases from zero to the baseInsulinLevel. This allows us to model the increased liver glycogen breakdown when BGL is
low.