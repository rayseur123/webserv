NAME := webserv

# ---------------------------------------------------------#
#                       DIRECTORIES                        #
# ---------------------------------------------------------#

MAKEFLAGS += --jobs

SRCS_DIR        := srcs/
EPOLL_DIR       := $(SRCS_DIR)epoll/
HTTP_DIR        := $(SRCS_DIR)http/
HTTP_PARS_DIR   := $(SRCS_DIR)http/parsing/
PARS_DIR        := $(SRCS_DIR)parsing/
SOCKET_DIR      := $(SRCS_DIR)socket/
UTILS_DIR       := $(SRCS_DIR)utils/

# ---------------------------------------------------------#
#                       SOURCE FILES                       #s
# ---------------------------------------------------------#

SRCS_CONTENT :=	$(SRCS_DIR)main.cpp \
				$(EPOLL_DIR)EpollManager.cpp \
				$(EPOLL_DIR)Signal.cpp \
				$(HTTP_DIR)AResponse.cpp \
				$(HTTP_DIR)Code.cpp \
				$(HTTP_DIR)ResponseGet.cpp \
				$(HTTP_DIR)ResponseDelete.cpp \
				$(HTTP_DIR)ResponsePost.cpp \
				$(HTTP_DIR)Cgi.cpp \
				$(HTTP_PARS_DIR)Body.cpp \
				$(HTTP_PARS_DIR)Header.cpp \
				$(HTTP_PARS_DIR)Method.cpp \
				$(HTTP_PARS_DIR)ParsingRequest.cpp \
				$(HTTP_PARS_DIR)Request.cpp \
				$(HTTP_PARS_DIR)Uri.cpp \
				$(HTTP_PARS_DIR)Version.cpp \
				$(PARS_DIR)Block.cpp \
				$(PARS_DIR)Location.cpp \
				$(SOCKET_DIR)ASocket.cpp \
				$(SOCKET_DIR)Connection.cpp \
				$(SOCKET_DIR)SocketCgi.cpp \
				$(SOCKET_DIR)Listener.cpp \
				$(UTILS_DIR)utils.cpp \

# ---------------------------------------------------------#
#                       BUILD SOURCE                       #
# ---------------------------------------------------------#

SRCS = $(SRCS_CONTENT) \

# ---------------------------------------------------------#
#                         HEADERS                          #
# ---------------------------------------------------------#

HEAD_DIR := includes/

CPPFLAGS += $(addprefix -I, $(HEAD_DIR))

# ---------------------------------------------------------#
#                           LIBS                           #
# ---------------------------------------------------------#

LIBS_DIR :=

LIBS_TARGET :=

LIBS_NAMES := $(patsubst %.a,%,$(notdir $(LIBS_TARGET)))

LIBS_INCLUDES := $(addprefix $(LIBS_DIR), $(addsuffix /includes,$(LIBS_NAMES))) # si pas de /include ajouter manuellement

CPPFLAGS += $(addprefix -I, $(LIBS_INCLUDES))

SYS_LIBS =
SYS_LIBS := $(addprefix -l, $(SYS_LIBS))

# ---------------------------------------------------------#
#                           OBJS                           #
# ---------------------------------------------------------#

BUILD_DIR   := .build/
OBJS_DIR    := $(BUILD_DIR)objs/
OBJS := $(patsubst $(SRCS_DIR)%.cpp, $(OBJS_DIR)%.o, $(SRCS))

# ---------------------------------------------------------#
#                           DEPS                           #
# ---------------------------------------------------------#

DEPS := $(OBJS:.o=.d)

CPPFLAGS += -MMD -MP

# ---------------------------------------------------------#
#                   CONFIG COMPILATION                     #
# ---------------------------------------------------------#

CFLAGS += -std=c++98 -Wall -Wextra -Werror -g3

CC = c++

# ---------------------------------------------------------#
#                           MOD                            #
# ---------------------------------------------------------#

ifeq ($(MOD), debug)
	CFLAGS += -g3
endif

# ---------------------------------------------------------#
#                          RULES                           #
# ---------------------------------------------------------#

all: $(NAME)

$(NAME): $(LIBS_TARGET) $(OBJS)
	$(CC) $^ -o $@ $(SYS_LIBS)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

$(LIBS_TARGET): force
	$(MAKE) -C $(dir $@)

debug:
	$(MAKE) MOD=debug

force:

# ---------------------------------------------------------#
#                       CLEAN                              #
# ---------------------------------------------------------#

clean:
	rm -rf $(BUILD_DIR)
	$(foreach lib,$(LIBS_TARGET),$(MAKE) -C $(dir $(lib)) clean;)

fclean: clean
	rm -rf $(NAME)
	$(foreach lib,$(LIBS_TARGET),$(MAKE) -C $(dir $(lib)) fclean;)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY : clean fclean all re force

-include $(DEPS)
