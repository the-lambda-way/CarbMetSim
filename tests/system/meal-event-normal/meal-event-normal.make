# File locations relative to the repo's root folder


MEAL_EVENT_NORM_FLDR = tests/system/meal-event-normal


.PHONY: meal-event-normal
meal-event-normal:
	@$(MAKE) --no-print-directory $(MAIN)
	@cd $(MEAL_EVENT_NORM_FLDR); \
	python3 runSims.py


.PHONY: clean-meal-event-normal
clean-meal-event-normal:
	@echo "Cleaning meal-event-normal test"
	@$(RM) -rf $(MEAL_EVENT_NORM_FLDR)/output/
	@$(RM) -f $(MEAL_EVENT_NORM_FLDR)/outNormal.diff
