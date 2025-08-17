# Compiler and flags
CXX = g++
STD = -std=c++17
WARNINGS = -Wall
CXXFLAGS = $(STD) $(WARNINGS)

# Output and directories
OUTPUT = yopl.exe
SRC_DIRS = . env lexer parser visitor api preprocessor
BUILD_DIR = bin

# Gather source files
SOURCES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

# Map source files to object files in bin/
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Default rule
all: $(OUTPUT)

# Link object files into the final executable
$(OUTPUT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(OUTPUT)

# Compile rule: build object files in bin/
$(BUILD_DIR)/%.o: %.cpp
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule for cmd.exe
clean:
	@echo Cleaning build artifacts...
	@if exist $(OUTPUT) del /Q $(OUTPUT)
	@for %%f in ($(subst /,\,$(OBJECTS))) do if exist %%f del /Q %%f

# Static release build
STATICFLAGS = -static -static-libgcc -static-libstdc++
release: CXXFLAGS += $(STATICFLAGS)
release: clean $(OUTPUT)
