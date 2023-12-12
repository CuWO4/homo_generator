TARGET := homo.exe
EXTERN := cpp
COMPILER := g++

COMPILE_OPTION := -Wall -O2
# to generate dependent files #
COMPILE_OPTION_DES := -MMD -MP 

# store .o and .d files #
TMPDIR := tmp
# store the target file #
DEBUGDIR := debug

# sources, objects and dependencies #
SRCS := $(wildcard *.$(EXTERN))
OBJS := $(patsubst %.$(EXTERN), $(TMPDIR)/%.o, $(SRCS))
DEPS := $(patsubst %.$(EXTERN), $(TMPDIR)/%.d, $(SRCS))

# link #
$(DEBUGDIR)/$(TARGET) : $(OBJS) | $(DEBUGDIR)
	@ echo linking...
	$(COMPILER) $(OBJS) -o $(DEBUGDIR)/$(TARGET)
	@ echo completed!

# compile #
$(TMPDIR)/%.o : %.$(EXTERN) | $(TMPDIR)
	@ echo compiling $<...
	$(COMPILER) $< -o $@ -c $(COMPILE_OPTION) $(COMPILE_OPTION_DES)

# create TMPDIR when it does not exist #
$(TMPDIR) :
	@ mkdir $(patsubst ./%, %, $(TMPDIR))

$(DEBUGDIR) :
	@ mkdir $(DEBUGDIR)

.gitignore :
	@ cd . > .gitignore
	@ echo $(DEBUGDIR) >> .gitignore
	@ echo $(TMPDIR) >> .gitignore
	@ echo. >> .gitignore
	@ echo *.exe >> .gitignore

README.md :
	@ cd . > README.md

main.$(EXTERN) :
	@ cd . > main.$(EXTERN)
	@ echo #include ^<iostream^> >> main.$(EXTERN)
	@ echo. >> main.$(EXTERN)
	@ echo int main(int argc, char **argv) { >> main.$(EXTERN)
	@ echo. >> main.$(EXTERN)
	@ echo     return 0; >> main.$(EXTERN)
	@ echo } >> main.$(EXTERN)

# files dependecies #
-include $(DEPS)

# run command #
.PHONY : run
run :
	./$(DEBUGDIR)/$(TARGET)

# clean command #
.PHONY : clean
clean :
	@ echo try to clean...
	rm -r $(DEBUGDIR)/$(TARGET) $(OBJS) $(DEPS)
	@ echo completed!

# initialize command #
.PHONY : init
init : | $(TMPDIR) $(DEBUGDIR) .gitignore README.md main.$(EXTERN)
	@ echo completed!