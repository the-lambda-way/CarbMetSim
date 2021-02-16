# File locations relative to the repo's root folder


EXERCISE_EVENT_AHLARM30_FLDR = tests/system/exercise-event-AhlborgArm30


.PHONY: exercise-event-AhlborgArm30
exercise-event-AhlborgArm30:
	@$(MAKE) --no-print-directory $(MAIN)
	@cd $(EXERCISE_EVENT_AHLARM30_FLDR); \
	python3 runSims.py


.PHONY: clean-exercise-event-AhlborgArm30
clean-exercise-event-AhlborgArm30:
	@echo "Cleaning exercise-event-AhlborgArm30 test"
	@$(RM) -rf $(EXERCISE_EVENT_AHLARM30_FLDR)/output/
	@$(RM) -f $(EXERCISE_EVENT_AHLARM30_FLDR)/outAhlborgArm30.diff
