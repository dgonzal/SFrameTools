dummy := $(shell ./apply-sframe-patches.sh > /dev/null)

# Package information
LIBRARY = SFrameTools
OBJDIR  = obj
DEPDIR  = $(OBJDIR)/dep
SRCDIR  = src
INCDIR  = include

# configure FastJet
INCLUDES += -I$(FASTJETDIR)/../include

USERCXXFLAGS := -g

#INCLUDES += -I$(LHAPDFDIR)/include
INCLUDES += -I/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.8/x86_64-slc5-gcc46-opt/include

# Include the generic compilation rules
include $(SFRAME_DIR)/Makefile.common
