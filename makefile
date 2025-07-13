# Compilers
CPP := wcl.exe
LINKER := wlink.exe
NVCC := nvcc

# Folders
OBJDIR := obj
SRCDIR := src
BINDIR := bin

empty :=
space := $(empty) $(empty)
comma := ,
file := file

# Paths
define recursive_wildcard
$(wildcard $(1)/*.c) $(foreach dir,$(wildcard $(1)/*),$(call recursive_wildcard, $(dir)))
endef

# List of source files
SRC_FILES := $(call recursive_wildcard,$(SRCDIR))

$(foreach file,$(SRC_FILES),$(info $(file)))
$(foreach file,$(SRC_FILES_CU),$(info $(file)))

# Convert to object file paths
OBJ_FILES := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.obj,$(SRC_FILES))

CPPFLAGS := -zastd=c99 -c -i=$(SRCDIR) -ms -lr -4 -l=COM

TARGETFILE := VESAINFO.COM
TARGET := $(BINDIR)/$(TARGETFILE)

MKDIR_P = if not exist "$(dir $@)" mkdir "$(dir $@)"

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(MKDIR_P)
	$(LINKER) system dos name $@ file $(subst $(space),$(space)$(file)$(space),$^)
	del /q *.err

$(OBJDIR)/%.obj: $(SRCDIR)/%.c
	$(MKDIR_P)
	$(CPP) $(CPPFLAGS) $< /Fo$@

-include $(OBJ_FILES:.obj=.d)

clean:
	rm -rf $(OBJDIR) $(TARGET)