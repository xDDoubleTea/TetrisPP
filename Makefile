# TetrisPP build. Requires Allegro 5 via pkg-config; see README.md.

OUT    := game
BUILD  ?= release
OBJDIR := build/$(BUILD)

ALLEGRO_MODULES := allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 \
                   allegro_primitives-5 allegro_audio-5 allegro_acodec-5 \
                   allegro_color-5

ifneq ($(shell pkg-config --exists $(ALLEGRO_MODULES) 2>/dev/null && echo ok),ok)
$(error Allegro 5 not found by pkg-config. See README.md for install instructions)
endif

ifeq ($(OS),Windows_NT)
	BIN := $(OUT).exe
else
	BIN := $(OUT)
endif

SRC := $(shell find . -name '*.cpp' -not -path './build/*')
OBJ := $(patsubst ./%.cpp,$(OBJDIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

CXXFLAGS += -std=c++17 -Wall -Wextra
ifeq ($(BUILD),debug)
	CXXFLAGS += -g -O0 -DDEBUG
else
	CXXFLAGS += -O2
endif
CXXFLAGS += $(shell pkg-config --cflags $(ALLEGRO_MODULES))
LDLIBS   += $(shell pkg-config --libs $(ALLEGRO_MODULES))

.DEFAULT_GOAL := release
.PHONY: release debug build-config clean compile_commands run

release: build-config

debug:
	@$(MAKE) --no-print-directory BUILD=debug build-config

# Phony, so the copy always runs and ./$(BIN) matches the requested config.
build-config: $(OBJDIR)/$(BIN)
	@cp -f $< $(BIN)
	@echo "$(BIN): $(BUILD) build ready"

$(OBJDIR)/$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: release
	./$(BIN)

compile_commands:
	$(MAKE) clean
	bear -- $(MAKE)

clean:
	rm -rf build $(OUT) $(OUT).exe

-include $(DEP)
