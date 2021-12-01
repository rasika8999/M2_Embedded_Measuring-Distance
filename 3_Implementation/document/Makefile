DOXY_FILE = ultra
BUILD_FILE = html

FixPath = $(subst /,\,$1)

doc:
	doxygen ./$(DOXY_FILE)

clean:
	rm -rf $(call FixPath,$(BUILD_FILE)/*)
