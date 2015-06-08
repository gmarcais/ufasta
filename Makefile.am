ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Wall -I$(top_srcdir)
AM_CXXFLAGS = $(ALL_CXXFLAGS) -g -O3
AM_LDFLAGS =

bin_PROGRAMS = ufasta
noinst_HEADERS = $(YAGGO_SRCS) common.hpp
BUILT_SOURCES = $(YAGGO_SRCS)
CLEANFILES =
DISTCLEANFILES = $(BUILT_SOURCES)

# Yaggo automatic rules with silencing
V_YAGGO = $(V_YAGGO_$(V))
V_YAGGO_ = $(V_YAGGO_$(AM_DEFAULT_VERBOSITY))
V_YAGGO_0 = @echo "  YAGGO   " $@;
.yaggo.hpp:
#	$(V_YAGGO)$(YAGGO) --license $(srcdir)/header-license -o $@ $<
	$(V_YAGGO)$(YAGGO) -o $@ $<

YAGGO_SRCS = # Append all file to be built by yaggo

##################
# Build binaries #
##################
ufasta_SOURCES = ufasta.cc one.cc sizes.cc head.cc tail.cc	\
                 rc.cc n50.cc extract.cc format.cc sort.cc dsort.cc
YAGGO_SRCS += hgrep_cmdline.hpp one_cmdline.hpp sizes_cmdline.hpp	\
              head_cmdline.hpp tail_cmdline.hpp rc_cmdline.hpp		\
              n50_cmdline.hpp extract_cmdline.hpp format_cmdline.hpp	\
              sort_cmdline.hpp dsort_cmdline.hpp dgrep_cmdline.hpp

if BOOST_REGEX
AM_LDFLAGS += -lboost_regex
ufasta_SOURCES += hgrep.cc dgrep.cc
endif
