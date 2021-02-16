# Glucose Transport

Glucose crosses cell membranes using either active transporters or passive ones. The active transporters, such as Sodium GLucose coTransporters (SGLTs) are able to move glucose from a low concentration to a high concentration. The passive transporters, such as Glucose Transporters (GLUTs) move glucose from a high concentration to a low concentration. CarbMetSim
models the operation of active transporters in an organ by specifying the average amount of glucose transferred per minute via active transport. The actual amount transferred is a poisson distributed random variable. The simulator uses Michaelis Menten kinetics to determine the amount of glucose transferred in a minute via passive transport. As per the Michaelis Menten kinetics, the rate of transport (V) across a membrane depends on the difference in the substrate concentration (Y) across the membrane in the following manner: V = Vmax * Y / (Y + Km), where Vmax is the maximum rate of transport and Km is the substrate concentration difference at which the transport rate is half the maximum. The Vmax value associated with a GLUT transporter in an organ indicates the number of transporters involved. Hence, the simulator treats Vmax associated with a particular GLUT in a particular organ as a poisson distributed random variable with a configurable mean.



## Modeling GLUT4 operation in muscles.

Among the GLUTs, the GLUT4 transporters are of particular importance because they allow the muscles to absorb glucose from the bloodstream. When the human body is engaged in exercise, the physical activity itself activates sufficient number of GLUT4 transporters and the muscles are able to absorb the desired amount of glucose from the bloodstream. CarbMetSim replicates this behavior. However, in the resting state, the number of active GLUT4 transporters depends on the insulin level in the bloodstream. When the insulin level is low (because of low BGL), GLUT4 transporters are inactive and the muscles do not absorb much glucose from the bloodstream. As the insulin level rises in the blood (in response to increase in BGL), GLUT4 transporters
become active proportionately and allow the muscles to quickly absorb excess glucose from the blood.

In CarbMetSim, GLUT4 activation during the resting states is modeled by manipulating the Vmax value associated with GLUT4 transporters in the following manner:

- Since a large fraction of the absorbed glucose is converted to glycogen inside muscles and there is a limit on how much glycogen can be stored inside muscles, the current amount of muscle glycogen impacts the Vmax value. Specifically, as the muscle glycogen storage increases from zero to a configurable maximum value, the Vmax value reduces linearly from a configurable maximum (7 mg/kg/min by default) to a configurable minimum (3.5 mg/kg/min by default).

- The impact of insulin level is captured by multiplying the Vmax value with a factor (between 0 and 1) that increases in value with increase in the insulinLevel. Currently, the insulinLevel itself is used as the value of this factor. Since vigorous physical exercise causes temporary increase in glucose absorption by muscles to make up for the glycogen lost during exercise), the insulinLevel does not impact the Vmax value in the first hour after an intense physical exercise activity (unless the current BGL drops below the baseGlucoseLevel).

- The impact of insulin resistance in reducing the activation of GLUT4 transporters is modeled by multiplying the the Vmax value with a configurable parameter (glut4Impact) that assumes values between 0 and 1 (by default 1.0).

