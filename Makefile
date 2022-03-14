CC = g++
FLAGS = -framework OpenCL -I/usr/local/include/SDL2 -I/usr/X11R6/include -I/usr/X11/include -L/usr/local/lib -lSDL2 -I
EXEC = Frac

SRC_PATH = ./src/
SRC_FILES = Camera.cpp init.cpp loop.cpp main.cpp Ray.cpp Viewpoint.cpp Window.cpp calc.cpp initCL.cpp calc3d.cpp Sphere.cpp move.cpp Light.cpp Plan.cpp
SRC = $(addprefix $(SRC_PATH),$(SRC_FILES))

INCLUDE_PATH = include/
INCLUDE_FILES = frac.h
INCLUDE = $(addprefix $(INCLUDE_PATH),$(INCLUDE_FILES))

OBJ_PATH = ./obj/
OBJ_FILES = $(SRC_NAME:.c=.o)
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_FILES))

all:$(EXEC)
	@printf "\033[34m\033[1m3D PROJET\n\033[0m\033[31mCompilation\033[37m -------------------------------------------- \033[32m[OK]\033[0m\n"

$(EXEC): $(INCLUDE) $(OBJ)
	@$(CC) $(FLAGS)$(INCLUDE_PATH) -o $(EXEC) $(SRC)

$(OBJ_PATH)%.o:$(SRC_PATH)%.c $(INC_FILE)
	$(CC) -c -o $@ $< $(INC)

clean:
	@rm -rf $(OBJ)
fclean : clean
	@rm -rf $(EXEC)
re : fclean all