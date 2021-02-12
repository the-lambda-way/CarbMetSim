# File locations relative to the repo's root folder


MEAL_EVENT_NORM_FLDR = tests/system/meal-event-normal


.PHONY: meal-event-normal
meal-event-normal:
	@$(MAKE) --no-print-directory $(MAIN)
	@cp $(MAIN) $(MEAL_EVENT_NORM_FLDR); \
	cd $(MEAL_EVENT_NORM_FLDR); \
	echo "Testing meal-event-normal..."; \
	$(RM) -f outNormal.alpha; \
	./$(MAIN) Food.txt Exercise.txt ParamsNormal.txt Events.txt alpha outNormal.alpha; \
	RESULT=$$?; \
	$(RM) -f $(MAIN); \
	if [ $$RESULT -eq 0 ]; then \
		diff -u --color outNormal.alpha outNormal.expected.alpha | head -n 100; \
	fi


.PHONY: clean-meal-event-normal
clean-meal-event-normal:
	@echo "Cleaning meal-event-normal test"
	@$(RM) -f $(MEAL_EVENT_NORM_FLDR)/outNormal.alpha
