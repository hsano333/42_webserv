NAME	:= webserv
LIB		:= ./lib
OBJDIR	:= ./obj
SRCDIR	:= ./srcs


CONFIGDIR			:= config/
CONFIGSRC 			:= config.cpp config_factory.cpp config_parser.cpp config_raw_loader.cpp  config_http.cpp config_server.cpp config_location.cpp config_limit.cpp config_cgi.cpp config_parsered_data.cpp  body_size.cpp
CONFIG 				:= $(addprefix $(CONFIGDIR)/, $(CONFIGSRC))

FILESYSTEMDIR		:= file/
FILESYSTEMSRC 		:= webserv_file.cpp webserv_file_factory.cpp normal_file.cpp directory_file.cpp error_file.cpp socket_file.cpp normal_reader.cpp socket_reader.cpp normal_writer.cpp socket_writer.cpp stream_reader.cpp stream_writer.cpp vector_file.cpp file_manager.cpp buffer_controller.cpp
FILESYSTEM 			:= $(addprefix $(FILESYSTEMDIR)/, $(FILESYSTEMSRC))

SERVERDIR			:= server/
SERVERSRC 			:= webserv.cpp webserv_waiter.cpp webserv_cleaner.cpp webserv_entity.cpp webserv_io.cpp
SERVER 				:= $(addprefix $(SERVERDIR)/, $(SERVERSRC))

EVENTDIR			:= event/
EVENTSRC 			:= webserv_event.cpp  webserv_event_factory.cpp webserv_make_request_event.cpp webserv_make_response_event.cpp webserv_application_without_cgi_event.cpp  webserv_application_with_cgi_event.cpp webserv_clean_event.cpp webserv_nothing_event.cpp webserv_timeout_event.cpp webserv_keep_alive_event.cpp event_manager.cpp event_controller.cpp webserv_io_socket_event.cpp webserv_io_cgi_event.cpp webserv_application_upload_event.cpp

EVENT 				:= $(addprefix $(EVENTDIR)/, $(EVENTSRC))


APPLICATIONDIR		:= application/
APPLICATIONSRC 		:= application_factory.cpp get_application.cpp post_application.cpp delete_application.cpp  cgi.cpp  get_cgi_application.cpp  post_cgi_application.cpp application_result.cpp
APPLICATION 		:= $(addprefix $(APPLICATIONDIR)/, $(APPLICATIONSRC))


HTTPDIR				:= http/
HTTPSRC 			:= method.cpp status_code.cpp request.cpp response.cpp http_version.cpp request_line.cpp uri.cpp header.cpp
HTTP 				:= $(addprefix $(HTTPDIR)/, $(HTTPSRC))

NETWORKDIR			:= network/
NETWORKSRC 			:= ip_address.cpp cidr.cpp  port.cpp
NETWORK 			:= $(addprefix $(NETWORKDIR)/, $(NETWORKSRC))

SOCKETDIR			:= socket/
SOCKETSRC 			:= socket.cpp socket_factory.cpp socket_repository.cpp epoll.cpp epoll_controller.cpp  fd_manager.cpp file_discriptor.cpp socket_controller.cpp 
SOCKET 				:= $(addprefix $(SOCKETDIR)/, $(SOCKETSRC))

EXCEPTIONDIR		:= exception/
EXCEPTIONSRC 		:= timeout_exception.cpp http_exception.cpp connection_exception.cpp
EXCEPTION 			:= $(addprefix $(EXCEPTIONDIR)/, $(EXCEPTIONSRC))

UTILITYDIR			:= utility/
UTILITYSRC 			:= split.cpp  utility.cpp endian.cpp log.cpp process_id.cpp
UTILITY 			:= $(addprefix $(UTILITYDIR)/, $(UTILITYSRC))

#socket_data.cpp request.cpp response.cpp socket.cpp tcp_socket.cpp uri.cpp  fd_manager.cpp 


CGI 				:= #cgi.cpp base64.cpp response_cgi.cpp request_cgi.cpp
DESIGN				:= 
SRC					:= $(CONFIG) $(FILESYSTEM) $(SERVER) $(EVENT) $(APPLICATION) $(HTTP) $(NETWORK) $(SOCKET) $(EXCEPTION) $(CGI) $(UTILITY)  $(DESIGN)
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

INCDIRS			:= $(FILESYSTEMDIR) $(SERVERDIR) $(EVENTDIR) $(HTTPDIR) $(NETWORKDIR) $(SOCKETDIR) $(EXCEPTIONDIR) $(CONFIGDIR) $(APPLICATIONDIR) $(UTILITYDIR)
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
