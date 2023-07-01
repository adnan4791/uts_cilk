# Generated automatically from Makefile.in by configure.
# $Id: Makefile.in,v 1.1.1.1 2004-02-06 18:15:04 msato Exp $
# $RWC_Release$
# $RWC_Copyright$

TOPDIR		=	../..
prefix		=	$(DESTDIR)/usr/local
exec_prefix	=	${prefix}

bindir		=	$(exec_prefix)/bin
libdir		=	$(prefix)/lib

ompcdir		=	$(libdir)/openmp
ompcincdir	=	$(ompcdir)/include
ompclibexecdir	=	$(ompcdir)/libexec
ompclibdir	=	$(ompcdir)/lib
ompcclassdir	=	$(ompcdir)/java
ompctempldir	=	$(ompcdir)/template

INSTALLDIR	=	$(ompclibdir)
INSTALLHDRDIR	=	$(ompcincdir)
INSTALL		= 	$(TOPDIR)/utils/install-sh -c
MKDIR		=	$(TOPDIR)/utils/mkdirhier

GCCINC		=	-I`case "$(CC)" in *gcc) echo \`$(CC) -v 2>&1 | grep specs | sed 's/specs$$/include/' | awk '{ print $$NF }'\`;; esac`


CC_OPT_SWITCH	=	-msse3 -mfpmath=sse -mtune=core2 -ffast_math
CPPFLAGS	=	-I. -I$(TOPDIR)/include  -DHAVE_ALLOCA_H=1 -DHAVE_ALLOCA=1 -DHAVE_STDARG_H=1 -DHAVE_LOCALE_H=1 -DHAVE_SETLOCALE=1 -DNO_IEEEFP_H=1 -DHAVE_STRDUP=1 -DHAVE_SYS_TIME_H=1 -DTIME_WITH_SYS_TIME=1 -DSTDC_HEADERS=1 -DHAS_LONGLONG=1 -DHAS_LONGDOUBLE=1 -DSIZEOF_UNSIGNED_SHORT=2 -DSIZEOF_UNSIGNED_INT=4 -DSIZEOF_UNSIGNED_LONG=4 -DSIZEOF_UNSIGNED_LONG_LONG=8 -DSIZEOF_FLOAT=4 -DSIZEOF_DOUBLE=8 -DSIZEOF_LONG_DOUBLE=12 -DSIZEOF_VOID_P=4 -DCHAR_ALIGN=1 -DSHORT_ALIGN=2 -DINT_ALIGN=4 -DLONG_ALIGN=4 -DLONGLONG_ALIGN=4 -DFLOAT_ALIGN=4 -DDOUBLE_ALIGN=4 -DLONGDOUBLE_ALIGN=4 -DHAS_INT16=1 -DHAS_INT32=1 -DHAS_INT64=1 -DHAS_QUAD_PRINT=1   -DOMNI_CPU_I386 -DOMNI_OS_LINUX -DNDEBUG   
THREAD_FLAGS	=	-DUSE_PTHREAD -DUSE_SPIN_LOCK
CFLAGS		=	$(CC_OPT_SWITCH) $(CPPFLAGS) $(THREAD_FLAGS)

GCC		=	/usr/local/bin/gcc
CC		=	/home/adnan/sthreads/bin/stgcc 
AR		=	ar cr
RANLIB		=	ranlib

YACC		=	/usr/bin/bison -y

DEPEND_SWITCH	=	$(CPPFLAGS)

SRCS1		= brg_sha1.c
SRCS2		= bots_main.c bots_common.c uts.c
 

OBJS1		= brg_sha1.o
OBJS2		= bots_main.o bots_common.o uts.c

TARGET		= uts

RUNTIME		= ompc_main_st.o
RUNTIME_SRC	= ompc_main.c

all:	$(TARGET) $(RUNTIME)

$(TARGET): $(OBJS)
	@rm -f $(TARGET)
	$(AR) $(TARGET) $(OBJS)
	$(RANLIB) $(TARGET)

$(RUNTIME): $(RUNTIME_SRC)
	$(CC) -c $(CFLAGS) -o $(RUNTIME) $(RUNTIME_SRC)

install: all
	@if [ ! -d $(INSTALLDIR) ] ; then \
		(echo "Making directory $(INSTALLDIR)" ; \
		$(MKDIR) $(INSTALLDIR) ; \
		chmod 775 $(INSTALLDIR)); \
	else \
		true ; \
	fi
	@if [ ! -d $(INSTALLHDRDIR) ] ; then \
		(echo "Making directory $(INSTALLHDRDIR)" ; \
		$(MKDIR) $(INSTALLHDRDIR) ; \
		chmod 775 $(INSTALLHDRDIR)); \
	else \
		true ; \
	fi
	$(INSTALL) -m 644 $(TARGET) $(INSTALLDIR)
	$(RANLIB) $(INSTALLDIR)/$(TARGET)
	$(INSTALL) -m 644 $(RUNTIME) $(INSTALLDIR)

clean:
	rm -f $(TARGET) $(RUNTIME) *.o *~ core

distclean:	clean
	rm -f config.* Makefile .depend*

depend: ompclib.h timer.h $(SRCS)
	touch .depend.tmp
	makedepend -f ./.depend.tmp -a -- $(GCCINC) $(DEPEND_SWITCH) -- $(SRCS)
	sed 's:^$(PLATFORMDIR)/:./:' ./.depend.tmp > ./.depend
	cat Makefile ./.depend > ./Makefile.tmp
	mv Makefile.tmp Makefile
	rm -f ./.depend ./.depend.tmp .depend.tmp.bak

Makefile::
	(cd ../..; sh ./config.status)
