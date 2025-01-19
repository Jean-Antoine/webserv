NAME =						webserv
SRCS_F =					$(addprefix /json/,		JsonData.cpp\
													JsonParser.cpp)\
							$(addprefix /engine/,	Server.cpp\
													Client.cpp\
													CGI.cpp\
													Logs.cpp\
													Ressource.cpp)\
							$(addprefix /message/,	Chunk.cpp\
													Message.cpp\
													Request.cpp\
													URI.cpp\
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
							-rm -rf /tmp/www  /tmp/www_archive
							cp -r ./tests/www ./tests/www_archive /tmp/
							cp ./tests/sensitivefile /tmp
							chmod -r /tmp/www_archive/kapouet/nopermission.html
							chmod -r /tmp/www_archive/kapouet/dir1/dir2/nopermission
							chmod -r /tmp/www/webserv_test/edge_cases/nopermission.html
							chmod -rw /tmp/www/images/forbidden

exec_tests:					all
							echo test
							bash tests/test_get.sh
							bash tests/test_chunk.sh

# Tests avec nginx
IMAGE_NAME=nginx
CONTAINER_NAME=nginx_webserv
PORT=9999
HOST_DIR=./tests/www
CONTAINER_DIR=/usr/share/nginx/html
DOCKERFILE_DIR=./tests/nginx

launch_nginx:
							docker build -t $(IMAGE_NAME) $(DOCKERFILE_DIR)
							docker run -d -p $(PORT):$(PORT) --name $(CONTAINER_NAME) -v $(HOST_DIR):$(CONTAINER_DIR) $(IMAGE_NAME)

stop_nginx:
							docker stop $(CONTAINER_NAME)
							docker rm $(CONTAINER_NAME)

restart_nginx:				stop_nginx launch_nginx

echo:
							echo $(OBJS)

-include $(DEP)

.PHONY:						all clean fclean re
