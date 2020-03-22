CXX := g++
CXXFLAGS := -g -c -O -Wall -Wextra -pedantic -std=gnu++11
LINKFLAGS := -O -g
LIB_DIRS := -Lext/lib

SFML_VERSION :=
CXX_VER = "-DSFML_VERSION=\"$(SFML_VERSION)\""

FILES_CPP := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp) $(wildcard src/*/*/*/*.cpp)
FILES_HPP := $(wildcard src/*.hpp) $(wildcard src/*/*.hpp) $(wildcard src/*/*/*.hpp) $(wildcard src/*/*/*/*.hpp)
FILES_DEP := $(patsubst src/%,build/%.dep,$(FILES_CPP))

GUI_SRC := $(filter-out src/cli/%,$(FILES_CPP))
GUI_BIN := bin/atcradar
GUI_LIBS := -lsfml-system -lsfml-window -lsfml-graphics -lsfml-network -lglu32 -lsqlite3pp -lsqlite -ltinyxml

# Hack for OS differences.
# On Windows, echo '1' produces literally '1' instead of 1.
ifeq "$(shell echo '1')" "'1'"
  WIN32 := 1
  mkdir = $(shell mkdir $(subst /,\,$(1)/dummy.mkdir) && rmdir $(subst /,\,$(1)/dummy.mkdir))
  rm_rf = $(foreach F,$(subst /,\,$(1)),$(shell rmdir /Q /S $(F) 2>NUL >NUL || del /Q /S $(F) 2>NUL >NUL)) $(1)

  # Get the version number from git; we must first check manually that git.exe exists
  ifeq "$(findstring git,$(shell for %%i in (git.exe) do @echo.%%~$$PATH:i))" "git"
    SFML_VERSION := $(strip $(shell git describe 2>NUL))
  endif
else
  mkdir = $(shell mkdir -p $(1))
  rm_rf = $(shell rm -rf $(1)) $(1)

  # Get the version number from git
  SFML_VERSION := $(strip $(shell git describe 2>/dev/null))

  # Something needs pthread.
  CLI_LIBS += -lpthread
  GUI_LIBS += -lpthread

  # Mac OS X needs different libraries
  ifeq "$(shell uname -s)" "Darwin"
    GUI_LIBS := $(patsubst -l%,-framework %,$(GUI_LIBS))
  endif

  ifeq "$(findstring mingw,$(CXX))" "mingw"
    WIN32 := 1
  endif
endif

ifdef WIN32
  GUI_BIN := $(GUI_BIN).exe
  GUI_LIBS := $(GUI_LIBS) -lws2_32
endif

# Abstract build rules.
all: gui
gui: $(GUI_BIN)

clean:
	@echo [RM] $(call rm_rf, build html)
clean_deps:
	@echo [RM] $(call rm_rf,$(FILES_DEP))
clean_html:
	@echo [RM] $(call rm_rf,html)

# Documentation build with Doxygen
html: Doxyfile $(FILES_CPP) $(FILES_HPP)
	@echo [DOXYGEN]
	@$(call mkdir,html)
	@doxygen > html/doxygen.log

# Build rules for binaries.
$(GUI_BIN): $(patsubst src/%,build/%.o,$(GUI_SRC))

$(GUI_BIN):
	@echo [LINK] $@
	@$(call mkdir,$(dir $@))
	@$(CXX) $(LINKFLAGS) $(LIB_DIRS) -o $@ $(filter %.o,$^) $(GUI_LIBS)


# Include dependencies; generation rules are below.
-include $(FILES_DEP)

# Dependency generation.
build/%.dep: src/%
	@echo [DEPEND] $<
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) $(CXX_VER) -MM $< -MT $@ -MP > $@

# Compilation
build/%.o: src/% build/%.dep
	@echo [CXX] $<
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) $(CXX_VER) $< -c -o $@

# Always regenerate program version string
build/ProgramInfo.cpp.o: $(FILES_CPP) $(FILES_HPP)