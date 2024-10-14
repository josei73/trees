CC		= g++
CFLAGS		= -std=c++17 -Wall -Weffc++ -Wextra -pedantic -Wfatal-errors -fPIE
C_DEBUG_FLAGS	= -g
RM		= /bin/rm -f
EXEC_NAME	= ISCAS
LIB_NAME	= lib$(EXEC_NAME).a
LIBSOURCES	= $(EXEC_NAME).cpp
SOURCES		= main.cpp ROBDD.cpp ISCASCompile.cpp
LIBOBJECTS	= $(LIBSOURCES:.cpp=.o)
OBJECTS		= $(SOURCES:.cpp=.o)
default: $(EXEC_NAME)

$(EXEC_NAME): $(OBJECTS)
	$(CC) -o $(EXEC_NAME) $(OBJECTS) -L. -l $(EXEC_NAME)

%.o: %.cpp
	$(CC) $(C_DEBUG_FLAGS) $(CFLAGS) -MMD -c $<

-include $(SOURCES:.cpp=.d)

clean:
	$(RM) $(EXEC_NAME) *.d *.o
