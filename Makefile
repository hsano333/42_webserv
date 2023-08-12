NAME	:= webserv
LIB		:= ./lib
OBJDIR	:= ./obj
SRCDIR	:= ./srcs

FILESYSTEMDIR			:= filesystem/
FILESYSTEMSRC 			:= file.cpp read_raw.cpp 
FILESYSTEM 				:= $(addprefix $(FILESYSTEMDIR)/, $(FILESYSTEMSRC))


CONFIGDIR			:= config/
CONFIGSRC 			:= config.cpp config_factory.cpp config_raw_loader.cpp  config_http.cpp config_server.cpp config_location.cpp
CONFIG 				:= $(addprefix $(CONFIGDIR)/, $(CONFIGSRC))

SOCKET 				:= fd_manager.cpp socket_data.cpp request.cpp response.cpp socket.cpp tcp_socket.cpp content_type.cpp uri.cpp
CGI 				:= cgi.cpp base64.cpp response_cgi.cpp request_cgi.cpp
SERVER 				:= webserv.cpp header.cpp method.cpp get_method.cpp
UTILITY 			:= split.cpp  utility.cpp epoll_manager.cpp endian.cpp log.cpp
EXCEPTION			:= no_request_line.cpp
DESIGN				:= iread.cpp
SRC					:= $(CONFIG) $(FILESYSTEM) $(SOCKET) $(CGI) $(SERVER) $(UTILITY) $(EXCEPTION) $(DESIGN)
UNIT_SRCS 			:= $(addprefix $(SRCDIR)/, $(SRC))
MANDATORY			:= main.cpp
BONUS				:= main_bonus.cpp



ifdef WITH_BONUS
SRC	+= $(BONUS)
DELENTRY	:= $(addprefix $(OBJDIR)/, $(MANDATORY))
else
SRC	+= $(MANDATORY)
DELENTRY	:= $(addprefix $(OBJDIR)/, $(BONUS))
endif

INCDIRS			:= $(CONFIGDIR) $(FILESYSTEMDIR)
INCDIR			:= $(addprefix $(SRCDIR)/, $(INCDIRS))
INCS			:= ./include  $(INCDIR)
IFLAGS			:= $(addprefix -I,$(INCS))
SRCS			:= $(addprefix $(SRCDIR)/, $(SRC))
OBJS			:= $(SRCS:.cpp=.o)
OBJECTS			:= $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))
OBJSDIR			:= $(addprefix $(OBJDIR)/, $(INCDIRS))
DEPS			:= $(OBJECTS:.o=.d)

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98 -g3
UNIT_CXXFLAGS := -Wall -Wextra -Werror -std=c++11 -g3 -D UNIT_TEST

all:
	@make $(NAME)

$(NAME)	:	$(OBJECTS) | $(OBJDIR)
		$(CXX)  $(OBJECTS) -o $@
		$(RM) $(DELENTRY:.cpp=.o)
		$(RM) $(DELENTRY:.cpp=.d)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(IFLAGS) -c $< -MMD -MP -MF $(OBJDIR)/$*.d  -o $@

$(OBJDIR) :
	@mkdir $(OBJDIR)
	@mkdir $(OBJSDIR)

clean	:
			$(RM) -r $(OBJDIR)
			$(RM) $(DELENTRY:.cpp=.o)
			$(RM) $(DELENTRY:.cpp=.d)

fclean	:	clean
			$(RM) $(NAME)

re	:		fclean all

bonus	:
			@make WITH_BONUS=1

r: run
run:
	./webserv ./srcs/config/config/mini.nginx.conf

mr: make_run
make_run:
	@make re
	@make run

ut: unit_test
unit_test: $(UNIT_SRCS)
	$(CXX) $(UNIT_CXXFLAGS) $(IFLAGS) -D UNIT_TEST -o unit_test $(UNIT_SRCS)
	touch statics/no_permission
	chmod 000 statics/no_permission
	mkdir -p statics/no_permission_dir
	chmod 000 statics/no_permission_dir
	touch statics/empty_file
	./unit_test

ifeq ($(findstring clean,$(MAKECMDGOALS)),)
-include $(DEPS)
endif

.PHONY: all clean fclean bonus re
