# File locations relative to the repo's root folder


MEAL_EVENT_DIAB_FLDR = tests/system/meal-event-diabetic


.PHONY: meal-event-diabetic
meal-event-diabetic:
	@$(MAKE) --no-print-directory $(MAIN)
	@cd $(MEAL_EVENT_DIAB_FLDR); \
	python3 runSims.py


.PHONY: clean-meal-event-diabetic
clean-meal-event-diabetic:
	@echo "Cleaning meal-event-diabetic test"
	@$(RM) -rf $(MEAL_EVENT_DIAB_FLDR)/output/
	@$(RM) -f $(MEAL_EVENT_DIAB_FLDR)/outDiab.diff
