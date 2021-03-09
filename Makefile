# Set project directory one level above of Makefile directory. $(CURDIR) is a GNU make variable containing the path to the current working directory
PROJDIR := $(realpath $(CURDIR)/)
INCLUDEDIR := $(PROJDIR)/include
SOURCEDIR := $(PROJDIR)/src
CLASSDIR := $(PROJDIR)/class
BUILDDIR := $(PROJDIR)/obj
VCPKGDIR = ./vcpkg/installed/x64-linux/

# Name of the final executable
TARGET = main

# Decide whether the commands will be shwon or not
VERBOSE = TRUE

# Create the list of directories
DIRS = . class
CLASSES = camera daeload datatype sdlwrapper shader
LIBS = m stdc++ GL SDL2 tinyobjloader BulletDynamics BulletCollision LinearMath jsoncpp tinyxml2 pthread dl
VCPKGIDIRS = . bullet json SDL2 glm
LIBSPATH = -L$(VCPKGDIR)/lib
SOURCEDIRS = $(foreach dir, $(CLASSES), $(addprefix $(CLASSDIR)/, $(dir)))
SOURCEDIRS += $(foreach dir, $(DIRS), $(addprefix $(SOURCEDIR)/, $(dir)))
TARGETDIRS = $(foreach dir, $(DIRS), $(addprefix $(BUILDDIR)/, $(dir)))
INCLUDES = -I$(CLASSDIR)/
INCLUDES += $(foreach dir, $(DIRS), $(addprefix -I$(INCLUDEDIR)/, $(dir)))
INCLUDES += $(foreach dir, $(VCPKGIDIRS), $(addprefix -I$(VCPKGDIR)/include/, $(dir)))
LLIBS = $(foreach dir, $(LIBS), $(addprefix -l, $(dir)))

# Generate the GCC includes parameters by adding -I before each source folder
#INCLUDES = $(foreach dir, $(SOURCEDIRS), $(addprefix -I, $(dir))) -I/usr/include/bullet

# Add this list to VPATH, the place make will look for the source files
VPATH = $(SOURCEDIRS)

# Create a list of *.c sources in DIRS
SOURCES = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.cpp))

# Define objects for all sources
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCES:.cpp=.o))

# Define dependencies files for all objects
DEPS = $(OBJS:.o=.d)

# Name the compiler
CC = g++

# OS specific part
ifeq ($(OS),Windows_NT)
	RM = del /F /Q 
	RMDIR = -RMDIR /S /Q
	MKDIR = -mkdir
	ERRIGNORE = 2>NUL || true
	SEP=\\
else
	RM = rm -rf 
	RMDIR = rm -rf 
	MKDIR = mkdir -p
	ERRIGNORE = 2>/dev/null
	SEP=/
endif

# Remove space after separator
PSEP = $(strip $(SEP))

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
	HIDE =  
else
	HIDE = @
endif

# Define the function that will generate each rule
define generateRules
$(1)/%.o: %.cpp
	@echo Building $$@
	$(HIDE)$(CC) -c $$(INCLUDES) -o $$(subst /,$$(PSEP),$$@) $$(subst /,$$(PSEP),$$<) -MMD
endef

.PHONY: all clean directories 

all: directories $(TARGET)

$(TARGET): $(OBJS)
	$(HIDE)echo Linking $@
	$(HIDE)$(CC) $(OBJS) -o $(TARGET) $(LIBSPATH) $(LLIBS)

# Include dependencies
-include $(DEPS)

# Generate rules
$(foreach targetdir,$(TARGETDIRS),$(eval $(call generateRules,$(targetdir))))

directories: 
	$(HIDE)$(MKDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)

# Remove all objects, dependencies and executable files generated during the build
clean:
	$(HIDE)$(RMDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)
	$(HIDE)$(RM) $(TARGET) $(ERRIGNORE)
	@echo Cleaning done ! 
