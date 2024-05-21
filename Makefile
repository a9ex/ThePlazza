##
## EPITECH PROJECT, 2024
## Plazza
## File description:
## Makefile
##

NAME			:=	plazza
CC				:=	g++
CPPFLAGS		:=	-Iinclude/
CXXFLAGS		:=	-Wall -Wextra -Werror -pedantic -std=c++20 -fPIE 	 \
					-fno-delete-null-pointer-checks -fno-strict-overflow \
					-fno-strict-aliasing -ftrivial-auto-var-init=zero    \
					-Wformat -Wimplicit-fallthrough 					 \
					-U_FORTIFY_SOURCE -D_GLIBCXX_ASSERTIONS 			 \
					-fstack-protector-strong -g

ifeq ($(PLATFORM),x86_64)
	CXXFLAGS	+=	-fcf-protection=full -Wl,-z,nodlopen -Wl,-z,noexecstack \
					-Wl,-z,relro -Wl,-z,now -fstack-clash-protection 		\
					-fstrict-flex-arrays=3 -Wtrampolines
endif

ifeq ($(PLATFORM),aarch64)
	CXXFLAGS	+=	-mbranch-protection=standard
endif

SRCS			:=	$(shell find src -name '*.cpp')
OBJS			:=	$(SRCS:.cpp=.o)

TESTS_CXXFLAGS	:=	-g -Wall -Wextra -std=c++11
TESTS_SRCS		:=	$(shell find tests -type f -name 'Tests*.cpp')
TESTS_OBJS		:=	$(TESTS_SRCS:.cpp=.o)

VALGRIND_FLAGS	:=	-s							\
					--leak-check=full			\
					--track-origins=yes			\
					--read-var-info=yes			\
					--trace-children=yes		\
					--show-leak-kinds=all		\
					--read-inline-info=yes		\
					--errors-for-leak-kinds=all

RM				:=	rm -rf

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(OBJS) -o $@

tests/%.o:	tests/%.cpp
	@$(CC) $(CPPFLAGS) $(TESTS_CXXFLAGS) -o $@ -c $<

tests_run:	CXXFLAGS+= -g --coverage -DCRITERION
tests_run:	fclean	$(OBJS)	$(TESTS_OBJS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(OBJS) $(TESTS_OBJS) -o unit_tests \
	-lcriterion
	CRITERION_NO_EARLY_EXIT=1 ./unit_tests
	gcovr -e tests .

valgrind:	tests_run
	valgrind $(VALGRIND_FLAGS) ./unit_tests

clean:
	@$(RM) $(OBJS)
	@$(RM) $(TESTS_OBJS)
	@$(RM) $(shell find . -type f -name '*.gcno')
	@$(RM) $(shell find . -type f -name '*.gcda')

fclean:	clean
	@$(RM) $(NAME)
	@$(RM) unit_tests

re:	fclean	all
