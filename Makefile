BASENAME =		JsonData\
				JsonParser\
				main
SRCS =			$(addsuffix .cpp, $(addprefix ./srcs/, $(BASENAME)))
OBJS =			$(addsuffix .o, $(addprefix ./objs/, $(BASENAME)))
CXXFLAGS =		-Wall -Werror -Wextra -std=c++98 -g3 -fno-limit-debug-info
CPPFLAGS =		-I./includes
CXX =			c++
NAME =			webserv

all:			$(NAME)

./objs/%.o:		./srcs/%.cpp
				@mkdir -p ./objs
				$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(NAME):		$(OBJS)
				$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
				rm -drf ./objs/

fclean:			clean
				rm -drf $(NAME)

re:				fclean $(NAME)

