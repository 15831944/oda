# GNU Makefile.  This should work for Windows, Mac OS X, and Solaris with
# Visual C++ 6.0 and GNU GCC.
#
# In multidimensional "make space", these are the dimensions:
#
# OS - Mac OS X, Solaris, Win32
# Lib type - static, shared
# compiler - VC++, GNU GCC
# build type - debug, release
#
# Utilities that must be present: pwd, GNU make, rm, mkdir, ar, ranlib
#
# (on Windows the Cygwin tools must be installed.  They can be obtained from
#  http://sources.redhat.com/cygwin/.  The environment variable $(CYGWIN)
#  should point to the root of the install, e.g. c:\cygwin.)
#


ifeq ($(TARGET),)
	ifeq ($(TARGET_OS),)
		ifeq ($(OS),Windows_NT)
			TARGET_OS=win32
		else
			ifeq ($(OSTYPE),darwin)
				TARGET_OS=darwin
			else
				ifeq ($(OSTYPE),linux)
					TARGET_OS=linux
				else
					ifeq ($(OSTYPE),linux-gnu)
						TARGET_OS=linux
					else
						ifeq ($(OSTYPE),solaris)
							TARGET_OS=solaris
						endif
					endif
				endif
			endif
		endif
	endif
	ifeq ($(TARGET_COMPILER),)
		ifeq ($(TARGET_OS),win32)
			TARGET_COMPILER=vcpp
		else
			TARGET_COMPILER=gcc
		endif
	endif
	ifeq ($(TARGET_LIBTYPE),)
		TARGET_LIBTYPE=static
	endif
	ifeq ($(TARGET_BUILDTYPE),)
		TARGET_BUILDTYPE=debug
	endif
	
	ifeq ($(TARGET_VERSION),)
		TARGET_VERSION=Standard
	endif

	TARGET=$(TARGET_OS)-$(TARGET_COMPILER)-$(TARGET_LIBTYPE)-$(TARGET_BUILDTYPE)-$(TARGET_VERSION)
endif

ifeq ($(TARGET_OS),win32)
	BIN_EXT=.exe
	ifeq ($(CYGWIN),)
		CYGWIN=c:/cygwin
		MKDIR=c:\cygwin\bin\mkdir.exe
	endif
	BASE_DIR=$(subst \,/,$(shell cmd /c cd))
	OS_MACRO_NAME=SSTG_WIN32
else
	# All other supported systems are UNIX-like, so far
	
	BIN_EXT=
	BASE_DIR=$(shell pwd)
	C_OS_FLAGS=-D_FILE_OFFSET_BITS=64
	ifeq ($(TARGET_OS),darwin)
		OS_MACRO_NAME=SSTG_MACOSX
	else
		ifeq ($(TARGET_OS),linux)
			OS_MACRO_NAME=SSTG_LINUX
		else
			OS_MACRO_NAME=SSTG_SOLARIS
		endif
	endif
endif

OBJ_DIR=$(BASE_DIR)/$(TARGET)
INC_DIRS=;$(BASE_DIR);$(BASE_DIR)/inc


	
ifeq ($(TARGET_COMPILER),gcc)
	ifeq ($(CC),)
		CC=gcc
	endif
	
	ifeq ($(TARGET_BUILDTYPE),debug)
		C_BUILDTYPE_FLAGS=-O0 -g -DDEBUG -D_DEBUG
		LINK_BUILDTYPE_FLAGS=-O0 -g -DDEBUG -D_DEBUG
	else
		C_BUILDTYPE_FLAGS=-O3 -DNDEBUG
		LINK_BUILDTYPE_FLAGS=-O3 -DNDEBUG
	endif
		
	ifeq ($(TARGET_LIBTYPE),static)
		C_LIBTYPE_FLAGS=
		LIB_TOOL=ar
		LIB_EXT=.a
		LIB_OUT_FILE_FLAG= 
		LIB_FLAGS=-r
		LIB_POST_STEP=ranlib
	else
		C_LIBTYPE_FLAGS=-fpic -DSHARED_MODULE
		LIB_TOOL=ld
		LIB_EXT=.so
		LIB_OUT_FILE_FLAG=-o 
		LIB_FLAGS=-Bshareable
		LIB_POST_STEP=echo
	endif
	
	OBJ_EXT=.o
	
	OS_MACRO_FLAGS=-D$(OS_MACRO_NAME)
	C_DONTLINK_FLAG=-c
	C_OBJOUT_FLAG=-o 
	C_ADD_INC_DIR_FLAG=-I 
	C_ADD_LIB_DIR_FLAG=-L 
	ifeq ($(TARGET_OS),win32)
		C_STD_INC_DIRS=;$(CYGWIN)/usr/include
		C_STD_LIB_DIRS=;$(CYGWIN)/lib
	else
		C_STD_INC_DIRS=
		C_STD_LIB_DIRS=
	endif
	C_LANGUAGE_FLAG=-x c -ansi
	C_ALL_WARNINGS_FLAG=-Wall
	C_INCLUDE_VERSION_FLAG=$(C_ADD_INC_DIR_FLAG)$(TARGET_VERSION)
	CFLAGS=$(C_DONTLINK_FLAG) $(subst ;, $(C_ADD_INC_DIR_FLAG),$(INC_DIRS)$(C_STD_INC_DIRS)) $(C_LANGUAGE_FLAG) $(C_BUILDTYPE_FLAGS) $(C_LIBTYPE_FLAGS) $(OS_MACRO_FLAGS) $(C_ALL_WARNINGS_FLAG) $(C_INCLUDE_VERSION_FLAG) $(C_OS_FLAGS)
	
	LINK=$(CC)
	LINK_BINOUT_FLAG=-o 
	LINK_BUILDTYPE_FLAGS=
	LINKFLAGS=$(LINK_BUILDTYPE_FLAGS) $(subst ;, $(C_ADD_LIB_DIR_FLAG),$(LIB_DIRS)$(C_STD_LIB_DIRS)) 
	
else
	ifeq ($(TARGET_COMPILER),vcpp)
		CC=cl
		
		ifeq ($(TARGET_BUILDTYPE),debug)
			C_BUILDTYPE_FLAGS=/Zi /Od /GZ /MTd /DDEBUG /D_DEBUG
			LINK_BUILDTYPE_FLAGS=/Zi /Od /GZ /MTd /DDEBUG /D_DEBUG
		else
			C_BUILDTYPE_FLAGS=/O1 /DNDEBUG
			LINK_BUILDTYPE_FLAGS=/O1 /DNDEBUG
		endif
		
		ifeq ($(TARGET_LIBTYPE),static)
			C_LIBTYPE_FLAGS=
			LIB_TOOL=lib
			LIB_EXT=.lib
			LIB_OUT_FILE_FLAG=/out:
			LIB_FLAGS=
			LIB_POST_STEP=echo
		endif
		
		OBJ_EXT=.obj
	
		OS_MACRO_FLAGS=/D$(OS_MACRO_NAME)
		C_DONTLINK_FLAG=/c
		C_OBJOUT_FLAG=/Fo
		C_ADD_INC_DIR_FLAG=/I
		C_ADD_LIB_DIR_FLAG=
		C_STD_INC_DIRS=
		C_STD_LIB_DIRS=
		C_LANGUAGE_FLAG=/TC
		C_ALL_WARNINGS_FLAG=/W3
		
		C_INCLUDE_VERSION_FLAG=$(C_ADD_INC_DIR_FLAG)$(TARGET_VERSION)
		CFLAGS=$(C_DONTLINK_FLAG) $(subst ;, $(C_ADD_INC_DIR_FLAG),$(INC_DIRS)$(C_STD_INC_DIRS)) $(C_LANGUAGE_FLAG) $(C_BUILDTYPE_FLAGS) $(C_LIBTYPE_FLAGS) $(OS_MACRO_FLAGS) $(C_ALL_WARNINGS_FLAG) $(C_INCLUDE_VERSION_FLAG) $(C_OS_FLAGS)
		
		LINK=cl
		LINK_BINOUT_FLAG=/Fe
		LINKFLAGS=$(LINK_BUILDTYPE_FLAGS) $(subst ;, $(C_ADD_LIB_DIR_FLAG),$(LIB_DIRS)$(C_STD_LIB_DIRS)) 
		
	endif
endif


# Common to all OSs supported.  Move these to conditional blocks above if we
# discover a target OS where they are not supported.
RM=rm
RMFLAGS=-f
MKDIR?=mkdir
ECHO=echo

HEADERS=Common.h \
		Debug.h \
        Directory.h \
        DocumentProperties.h \
        Fat.h \
        Header.h \
        LEReadWrite.h \
        OpenList.h \
		PropertySet.h \
        RootStorage.h \
        Storage.h \
        Stream.h \
        SsrwInputSourceCommon.h \
        SsrwInputSourceFile.h \
        SsrwInputSourceMem.h \
        inc/SsrwInputSource.h \
        inc/StructuredStorage.h \
        TOC.h \
        Types.h \
        UTF8.h \
        $(TARGET_VERSION)/Version.h

OBJS=$(OBJ_DIR)/Common$(OBJ_EXT) \
     $(OBJ_DIR)/Debug$(OBJ_EXT) \
     $(OBJ_DIR)/Directory$(OBJ_EXT) \
     $(OBJ_DIR)/DocumentProperties$(OBJ_EXT) \
	 $(OBJ_DIR)/Fat$(OBJ_EXT) \
     $(OBJ_DIR)/Header$(OBJ_EXT) \
     $(OBJ_DIR)/LEReadWrite$(OBJ_EXT) \
     $(OBJ_DIR)/OpenList$(OBJ_EXT) \
     $(OBJ_DIR)/PropertySet$(OBJ_EXT) \
     $(OBJ_DIR)/RootStorage$(OBJ_EXT) \
     $(OBJ_DIR)/Storage$(OBJ_EXT) \
     $(OBJ_DIR)/Stream$(OBJ_EXT) \
     $(OBJ_DIR)/StructuredStorage$(OBJ_EXT) \
     $(OBJ_DIR)/TOC$(OBJ_EXT) \
     $(OBJ_DIR)/UTF8$(OBJ_EXT) \
     $(OBJ_DIR)/Version$(OBJ_EXT) \
     $(OBJ_DIR)/SsrwInputSource$(OBJ_EXT) \
     $(OBJ_DIR)/SsrwInputSourceFile$(OBJ_EXT) \
     $(OBJ_DIR)/SsrwInputSourceMem$(OBJ_EXT)

LIB_BIN_NAME=$(OBJ_DIR)/libSSRW2C$(LIB_EXT)

ifeq ($(TARGET_TEST),)
    TARGET_TEST=CTest
endif

TEST_OBJS=$(OBJ_DIR)/CTest$(OBJ_EXT) \
	$(OBJ_DIR)\Shell$(OBJ_EXT)

TEST_BIN_NAMES=$(OBJ_DIR)/CTest$(BIN_EXT) \
	$(OBJ_DIR)/Shell$(BIN_EXT)



# End of setup, start of make rules

# Master rule
all: $(LIB_BIN_NAME) $(TEST_BIN_NAMES)

# Rule for creating output directory
$(OBJ_DIR)/date:
	$(MKDIR) $(OBJ_DIR)
	$(ECHO) Please do not delete this file >$(OBJ_DIR)/date
	
# Pattern rules for building .c files into object files
$(OBJ_DIR)/%$(OBJ_EXT): $(BASE_DIR)/%.c $(OBJ_DIR)/date $(HEADERS)
	$(CC) $(CFLAGS) $(C_OBJOUT_FLAG)$@ $<
	
$(OBJ_DIR)/%$(OBJ_EXT): $(BASE_DIR)/CTest/%.c $(OBJ_DIR)/date $(HEADERS)
	$(CC) $(CFLAGS) $(C_OBJOUT_FLAG)$@ $<

$(OBJ_DIR)/%$(OBJ_EXT): $(BASE_DIR)/$(TARGET_VERSION)/%.c $(OBJ_DIR)/date $(HEADERS)
	$(CC) $(CFLAGS) $(C_OBJOUT_FLAG)$@ $<

# Rule for building a library (.a, .so, .lib, .dll) file
$(LIB_BIN_NAME): $(OBJS)
	$(LIB_TOOL) $(LIB_FLAGS) $(LIB_OUT_FILE_FLAG)$@ $(OBJS)
	$(LIB_POST_STEP) $@

# Rules for building test binaries
$(OBJ_DIR)/CTest$(BIN_EXT): $(OBJ_DIR)/CTest$(OBJ_EXT) $(LIB_BIN_NAME) 
	$(LINK) $(LINKFLAGS) $(LINK_BINOUT_FLAG)$@ $< $(LIB_BIN_NAME)

$(OBJ_DIR)/Shell$(BIN_EXT): $(OBJ_DIR)/Shell$(OBJ_EXT) $(LIB_BIN_NAME) 
	$(LINK) $(LINKFLAGS) $(LINK_BINOUT_FLAG)$@ $< $(LIB_BIN_NAME)

# Smoke testing -- not an official target!  Just a quick script to help test
testrw:
	$(RM) $(RMFLAGS) ../testfiles/*.cpy
	$(RM) $(RMFLAGS) ../testfiles/vssver.scc
	- find ../testfiles -name '*.*' | xargs -n 1 $(TEST_BIN_NAME) -rw | grep 'status[ ]*=[ ]*[1-9]'
	
# Rules for cleaning up after ourselves
clean:
	$(RM) $(RMFLAGS) $(OBJS) $(LIB_BIN_NAME) $(TEST_BIN_NAME) $(TEST_OBJS)
