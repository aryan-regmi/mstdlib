BUILD_DIR = ./build

TEST_EXE = mstdlibUnitTests

make: $(BUILD_DIR)
	@cd $(BUILD_DIR); \
	cmake --build .; \
	cd ../; \
	echo "=================================================="; \
	echo ""; \
	$(BUILD_DIR)/$(TEST_EXE)
	
