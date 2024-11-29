DEPS_LINUX := deps/linux-6.11.8

all:
	@echo "Run Prebuild Script"

	@bash prebuild.sh
	
	@echo "Run Successful"

	$(MAKE) -C impl
