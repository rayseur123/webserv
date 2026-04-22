NAME := webServ

# ---------------------------------------------------------#
#                       DIRECTORIES                        #
# ---------------------------------------------------------#

SRCS_DIR := srcs/

# ---------------------------------------------------------#
#                       SOURCE FILES                       #s
# ---------------------------------------------------------#

SRCS_CONTENT :=		main.cpp \
					ASocket.cpp \
					Block.cpp \
					Request.cpp \
					Method.cpp \
					Uri.cpp \
					Version.cpp \
					Header.cpp \
					Body.cpp \
					Error.cpp \
					Connection.cpp \
					EpollManager.cpp \
					Listener.cpp \
					Location.cpp \
					utils.cpp \

# ---------------------------------------------------------#
#                       BUILD SOURCE                       #
# ---------------------------------------------------------#

SRCS =	$(addprefix $(SRCS_DIR), $(SRCS_CONTENT)) \

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

BUILD_DIR := .build/

OBJS_DIR := $(addprefix $(BUILD_DIR), objs/)
OBJS := $(addprefix $(OBJS_DIR), $(notdir $(SRCS:.cpp=.o)))

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

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp $(SRCS_DIR)%.cpp
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
