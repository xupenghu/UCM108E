.PHONY: clean All

All:
	@echo "----------Building project:[ rtthread_nav - Debug ]----------"
	@"$(MAKE)" -f  "rtthread_nav.mk" && "$(MAKE)" -f  "rtthread_nav.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ rtthread_nav - Debug ]----------"
	@"$(MAKE)" -f  "rtthread_nav.mk" clean
