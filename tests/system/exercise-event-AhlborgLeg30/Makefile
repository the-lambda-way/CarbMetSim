# File locations relative to the repo's root folder


EXERCISE_EVENT_AHLLEG30_FLDR = tests/system/exercise-event-AhlborgLeg30


.PHONY: exercise-event-AhlborgLeg30
exercise-event-AhlborgLeg30:
	@$(MAKE) --no-print-directory $(MAIN)
	@cd $(EXERCISE_EVENT_AHLLEG30_FLDR); \
	python3 runSims.py


.PHONY: clean-exercise-event-AhlborgLeg30
clean-exercise-event-AhlborgLeg30:
	@echo "Cleaning exercise-event-AhlborgLeg30 test"
	@$(RM) -rf $(EXERCISE_EVENT_AHLLEG30_FLDR)/output/
	@$(RM) -f $(EXERCISE_EVENT_AHLLEG30_FLDR)/outAhlborgLeg30.diff
