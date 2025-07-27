# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Target executable
TARGET = $(BINDIR)/library_manager

# Default target
all: $(TARGET)

# Create directories if they don't exist
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

# Build target
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(OBJECTS) -o $@
	@echo "Build complete! Executable: $(TARGET)"

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	@rm -rf $(OBJDIR) $(BINDIR)
	@echo "Clean complete!"

# Run the program
run: $(TARGET)
	@echo "Running Library Management System..."
	@./$(TARGET)

# Install dependencies (placeholder for future database integration)
install-deps:
	@echo "Installing dependencies..."
	@echo "For PostgreSQL integration, install libpq-dev:"
	@echo "sudo apt-get install libpq-dev  # On Ubuntu/Debian"
	@echo "brew install postgresql        # On macOS"

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build
release: CXXFLAGS += -DNDEBUG
release: clean $(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all         - Build the project (default)"
	@echo "  clean       - Remove build files"
	@echo "  run         - Build and run the program"
	@echo "  debug       - Build with debug information"
	@echo "  release     - Build optimized release version"
	@echo "  install-deps- Show dependency installation instructions"
	@echo "  help        - Show this help message"

.PHONY: all clean run install-deps debug release help
