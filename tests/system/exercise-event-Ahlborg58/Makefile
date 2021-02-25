# File locations relative to the repo's root folder


EXERCISE_EVENT_AHL58_FLDR = tests/system/exercise-event-Ahlborg58


.PHONY: exercise-event-Ahlborg58
exercise-event-Ahlborg58:
	@$(MAKE) --no-print-directory $(MAIN)
	@cd $(EXERCISE_EVENT_AHL58_FLDR); \
	python3 runSims.py


.PHONY: clean-exercise-event-Ahlborg58
clean-exercise-event-Ahlborg58:
	@echo "Cleaning exercise-event-Ahlborg58 test..."
	@$(RM) -rf $(EXERCISE_EVENT_AHL58_FLDR)/output/
	@$(RM) -f $(EXERCISE_EVENT_AHL58_FLDR)/outAhl58.diff
