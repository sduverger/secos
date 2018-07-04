#!/usr/bin/make -f

define compile
echo "    CC    $<"
$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $<
endef

define depend
echo "    DP    $<"
$(CPP) -M -MG -MT '$<' $(INCLUDE) $(CFLAGS) $< | \
$(SED) 's,\($*\)\.[c|s][ :]*,\1.o $@ : ,' > $@
endef

define assemble
echo "    AS    $<"
$(CPP) $< $(CFLAGS) -o $<.s
$(CC) $(CFLAGS) -o $@ -c $<.s 
$(RM) $<.s
endef

define link
echo "    LD    $@"
$(LD) $(LDFLAGS) --gc-sections -T $(LDSCRIPT) $^ -o $@ $(CCLIB)
endef

define qemu
echo "hit <ctl+a> <h> <enter>, for Qemu help"
$(QEMU) $(QOPT)
endef

define debug
echo "hit <ctl+a> <h> <enter>, for Qemu help"
$(QEMU) $(QOPT) -s -S
endef

.PHONY: clean

%.d: %.c
	@$(depend)
%.d: %.s
	@$(depend)
%.o: %.c
	@$(compile)
%.o: %.s
	@$(assemble)

all:$(TARGET)

$(TARGET): $(objects)
	@$(link)

dependencies := $(objects:.o=.d)

clean:
	@$(RM) -f $(TARGET) $(objects) $(dependencies)

qemu: $(TARGET)
	@$(qemu)

debug: $(TARGET)
	@$(debug)
