NAME	:= webserv
LIB		:= ./lib
OBJDIR	:= ./obj
SRCDIR	:= ./srcs


CONFIGDIR			:= config/
CONFIGSRC 			:= config.cpp config_factory.cpp config_parser.cpp config_raw_loader.cpp  config_http.cpp config_server.cpp config_location.cpp config_limit.cpp  config_parsered_data.cpp  body_size.cpp
CONFIG 				:= $(addprefix $(CONFIGDIR)/, $(CONFIGSRC))

FILESYSTEMDIR			:= filesystem/
FILESYSTEMSRC 			:= file.cpp read_raw.cpp 
FILESYSTEM 				:= $(addprefix $(FILESYSTEMDIR)/, $(FILESYSTEMSRC))

SERVERDIR			:= server/
SERVERSRC 			:= webserv.cpp header.cpp method.cpp  port.cpp status_code.cpp
SERVER 				:= $(addprefix $(SERVERDIR)/, $(SERVERSRC))

NETWORKDIR			:= network/
NETWORKSRC 			:= ip_address.cpp cidr.cpp
NETWORK 			:= $(addprefix $(NETWORKDIR)/, $(NETWORKSRC))

SOCKETDIR			:= socket/
SOCKETSRC 			:= fd_manager.cpp socket_data.cpp request.cpp response.cpp socket.cpp tcp_socket.cpp uri.cpp
SOCKET 				:= $(addprefix $(SOCKETDIR)/, $(SOCKETSRC))


CGI 				:= cgi.cpp base64.cpp response_cgi.cpp request_cgi.cpp
UTILITY 			:= split.cpp  utility.cpp epoll_manager.cpp endian.cpp log.cpp
EXCEPTION			:= no_request_line.cpp
DESIGN				:= iread.cpp
SRC					:= $(CONFIG) $(FILESYSTEM) $(SERVER) $(NETWORK) $(SOCKET) $(CGI) $(UTILITY) $(EXCEPTION) $(DESIGN)
MANDATORY			:= main.cpp
BONUS				:= main_bonus.cpp
UNIT_TEST_SRCS			:= unit_test/unit_main.cpp $(SRC)
UNIT_SRCS 			:= $(addprefix $(SRCDIR)/, $(UNIT_TEST_SRCS))



ifdef WITH_BONUS
SRC	+= $(BONUS)
DELENTRY	:= $(addprefix $(OBJDIR)/, $(MANDATORY))
else
SRC	+= $(MANDATORY)
DELENTRY	:= $(addprefix $(OBJDIR)/, $(BONUS))
endif

INCDIRS			:= $(CONFIGDIR) $(FILESYSTEMDIR) $(SERVERDIR) $(NETWORKDIR) $(SOCKETDIR)
INCDIR			:= $(addprefix $(SRCDIR)/, $(INCDIRS))
INCS			:= ./include ./srcs/unit_test $(INCDIR)
IFLAGS			:= $(addprefix -I,$(INCS))
SRCS			:= $(addprefix $(SRCDIR)/, $(SRC))
OBJS			:= $(SRCS:.cpp=.o)
OBJECTS			:= $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))
OBJSDIR			:= $(addprefix $(OBJDIR)/, $(INCDIRS))
DEPS			:= $(OBJECTS:.o=.d)

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98 -g3

all:
	@make $(NAME)

$(NAME)	:	$(OBJECTS) | $(OBJDIR)
		$(CXX)  $(OBJECTS) -o $@
		$(RM) $(DELENTRY:.cpp=.o)
		$(RM) $(DELENTRY:.cpp=.d)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJSDIR)
	$(CXX) $(CXXFLAGS) $(IFLAGS) -c $< -MMD -MP -MF $(OBJDIR)/$*.d  -o $@

$(OBJSDIR) : 
	@mkdir -p $(OBJSDIR)

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


UNIT_CXXFLAGS := -Wall -Wextra -Werror  -g3 -D UNIT_TEST
ut: unit_test
unit_test: $(UNIT_SRCS)
	$(CXX) $(UNIT_CXXFLAGS) $(IFLAGS) -I./srcs/unit_test/ -D UNIT_TEST  -o unit_test  $(UNIT_SRCS)
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
