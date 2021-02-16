# Exercise

CarbMetSim can currently simulate only aerobic exercises. An exercise activity is described in terms of its intensity in units of Metabolic Equivalent of Task (MET), where 1 MET is 1 kcal of energy expenditure per kg of body weight per hour. By convention, 1 MET is considered equivalent to 3.5ml of oxygen consumption per kg of body weight per minute. Each individual has a certain maximal rate at which he/she can consume oxygen. This individual-specific maximal rate, called VO2max, depends on the gender, age and fitness level of the individual. The intensity of an exercise activity in terms of the associated oxygen consumption rate (described as the %age of the individual’s VO2max, henceforth referred to as %VO2max) determines to a large extent the relative fraction of the glucose and fatty acids oxidized to meet the energy needs of the exercising muscles. Thus, CarbMetSim needs to know the gender, age and (self-assessed) fitness level within the age group of the human subject being simulated. This information is used to estimate the VO2max for the human subject using the tables in Kaminsky et al.