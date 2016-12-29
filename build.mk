
sketch-dir := $(shell pwd)
build-dir = $(sketch-dir)/build
libraries-dir = $(sketch-dir)/libraries
#local-libraries-dir = $(sketch-dir)/../local-libraries

ARDUINO := arduino

# Find out where the proper build tool is
ide-dir := $(shell arduino --get-pref runtime.ide.path)
#ide-tools := $(shell arduino --get-pref | grep '^runtime\.tools\.')
builder = $(ide-dir)/arduino-builder

# Set up the build flags in the same way the IDE does
builder-flags = -built-in-libraries $(ide-dir)/libraries
builder-flags += -hardware $(ide-dir)/hardware
builder-flags += -tools $(ide-dir)/tools-builder
builder-flags += -tools $(ide-dir)/hardware/tools/avr
builder-flags += -build-path $(build-dir)
#builder-flags += $(patsubst %,-prefs %,$(ide-tools))
builder-flags += -fqbn "$(BOARD)"
builder-flags += -libraries $(libraries-dir)
builder-flags += -warnings more
#builder-flags += -verbose

all: $(build-dir)
	$(builder) $(builder-flags) -compile $(SKETCH)

install-libs:
	for lib in $(LIBRARIES); do $(ARDUINO) --pref sketchbook.path=$(sketch-dir) --install-library "$$lib"; done

$(build-dir):
	mkdir -p $(build-dir)

.PHONY: all install-libraries
