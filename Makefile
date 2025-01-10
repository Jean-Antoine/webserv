NAME =						webserv
SRCS_F =					$(addprefix /json/,		JsonData.cpp\
													JsonParser.cpp)\
							$(addprefix /engine/,	Server.cpp\
													Client.cpp\
													CGI.cpp\
													Logs.cpp)\
							$(addprefix /message/,	Chunk.cpp\
													Message.cpp\
													Request.cpp\
													URI.cpp\
													Ressource.cpp\
													Response.cpp)\
							$(addprefix /config/,	Config.cpp\
													Route.cpp\
													Path.cpp)\
							$(addprefix /methods/,	AMethod.cpp\
													Get.cpp\
													Post.cpp\
													Delete.cpp)\
							utils.cpp\
							main.cpp
OBJS_F = 					$(SRCS_F:.cpp=.o)
SRCS_D = 					./srcs/
OBJS_D =					./.objs/
SRCS =						$(addprefix $(SRCS_D), $(SRCS_F))
OBJS =						$(addprefix $(OBJS_D), $(OBJS_F))
DEP =						$(OBJS:%.o=%.d)
CC = 						c++
CPPFLAGS =					-MMD\
							-I./include \
							-I./include/json\
							-I./include/config\
							-I./include/engine\
							-I./include/methods\
							-I./include/message
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
							@mkdir -p $(dir $@)
							@echo "$(LIB_COLOR)[$(NAME)] $(NO_COLOR)Compiling $(OBJ_COLOR)$(notdir $<)$(NO_COLOR)"
							@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


clean:						
							@rm -rfd $(OBJS_D)

fclean:						clean
							@rm -f $(NAME)

re:							fclean all

cp_env_test:				
							cp -r ./tests/www /tmp/
							cp ./tests/www/sensitivefile /tmp
							chmod -r /tmp/www/kapouet/nopermission.html
							chmod -r /tmp/www/kapouet/dir1/dir2/nopermission

exec_tests:					all
							echo test
							bash tests/test_get.sh
							bash tests/test_chunk.sh

echo:
							echo $(OBJS)

-include $(DEP)

.PHONY:						all clean fclean re
