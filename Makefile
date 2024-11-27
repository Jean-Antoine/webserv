NAME =						webserv
SRCS_F =					main.cpp\
							logs.cpp\
							Server.cpp\
							JsonData.cpp\
							JsonParser.cpp\
							Config.cpp\
							utils.cpp
OBJS_F = 					$(SRCS_F:.cpp=.o)
SRCS_D = 					./srcs/
OBJS_D =					./objs/
SRCS =						$(addprefix $(SRCS_D), $(SRCS_F))
OBJS =						$(addprefix $(OBJS_D), $(OBJS_F))
DEP =						$(OBJS:%.o=%.d)
CC = 						c++
CPPFLAGS =					-MMD -I./include
CFLAGS =					-Wall -Werror -Wextra -g3 -std=c++98 -fno-limit-debug-info
OBJ_COLOR =					\033[0;34m
LIB_COLOR =					\033[1;36m
NO_COLOR =					\033[0m
BOLD =						\033[1m

all:						$(NAME)

$(NAME):					$(OBJS)
							@echo "$(LIB_COLOR)[$(NAME)] Compiling$(NO_COLOR) $(LIB_COLOR)binary$(NO_COLOR)"
							@$(CC) $(CPPFLAGS) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJS_D)%.o:				$(SRCS_D)%.cpp
							@mkdir -p $(OBJS_D)
							@echo "$(LIB_COLOR)[$(NAME)] $(NO_COLOR)Compiling $(OBJ_COLOR)$(notdir $<)$(NO_COLOR)"
							@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:						
							@rm -rfd $(OBJS_D)

fclean:						clean
							@rm -f $(NAME)

re:							fclean all

echo:
							echo $(OBJS)

-include $(DEP)

.PHONY:						all clean fclean re
